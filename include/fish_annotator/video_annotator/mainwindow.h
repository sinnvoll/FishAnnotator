/// @file
/// @brief Defines MainWindow class for video annotator.

#ifndef VIDEO_ANNOTATOR_MAINWINDOW_H
#define VIDEO_ANNOTATOR_MAINWINDOW_H

#include <memory>
#include <atomic>

#include <QWidget>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>

#include "fish_annotator/common/species_controls.h"
#include "fish_annotator/common/annotatedregion.h"
#include "fish_annotator/video_annotator/video_annotation.h"
#include "fish_annotator/video_annotator/player.h"
#include "ui_mainwindow.h"

#ifndef NO_TESTING
class TestVideoAnnotator;
#endif

namespace fish_annotator { namespace video_annotator {

/// @brief Video annotation GUI.
class MainWindow : public QWidget {
  Q_OBJECT
#ifndef NO_TESTING
  friend class ::TestVideoAnnotator;
#endif
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit MainWindow(QWidget *parent = 0);
protected:
  /// @brief Resizes the video and scene.
  ///
  /// @param event Qt resize event pointer.
  void resizeEvent(QResizeEvent *event) override final;
public slots:
  /// @brief Displays a video frame.
  ///
  /// @param image Video frame to display.
  /// @param frame Frame of the image.
  void showFrame(QImage image, qint64 frame);

  /// @brief Adds an individual and enables bounding box drawing.
  ///
  /// @param species Species name of the new individual.
  /// @param subspecies Subspecies name of the new individual.
  void addIndividual(std::string species, std::string subspecies);

  /// @brief Handles player duration change.
  ///
  /// @param duration New video duration in total frames.
  void handlePlayerDurationChanged(qint64 duration);

  /// @brief Handles player playback rate change.
  ///
  /// @param rate New playback rate in fps.
  void handlePlayerPlaybackRateChanged(double rate);

  /// @brief Handles player resolution change.
  ///
  /// @param width Width of new video resolution.
  /// @param height Height of new video resolution.
  void handlePlayerResolutionChanged(qint64 width, qint64 height);

  /// @brief Handles player state change.
  ///
  /// @param stopped True if player stopped, false otherwise.
  void handlePlayerStateChanged(bool stopped);

  /// @brief Handles new media loaded.
  ///
  /// @param video_path Path to loaded video.
  /// @param native_rate Native playback rate of the video.
  void handlePlayerMediaLoaded(QString video_path, qreal native_rate);

  /// @brief Handles media player errors.
  ///
  /// @param err Error message.
  void handlePlayerError(QString err);
signals:
  /// @brief Requests play.
  void requestPlay();

  /// @brief Requests stop.
  void requestStop();

  /// @brief Requests load video.
  ///
  /// @param file Full path to file to be loaded.
  void requestLoadVideo(QString file);

  /// @brief Requests speed up.
  void requestSpeedUp();

  /// @brief Requests slow down.
  void requestSlowDown();

  /// @brief Requests set frame.
  ///
  /// @param value Frame to go to.
  void requestSetFrame(qint64 value);

  /// @brief Requests next frame.
  void requestNextFrame();

  /// @brief Requests previous frame.
  void requestPrevFrame();
private slots:
  /// @brief Plays/pauses the video.
  void on_play_clicked();

  /// @brief Increases the playback speed of the video by a factor of two.
  void on_faster_clicked();

  /// @brief Decreases the playback speed of the video by a factor of two.
  void on_slower_clicked();

  /// @brief Rewinds the video by one second.
  void on_minusOneSecond_clicked();

  /// @brief Rewinds the video by three seconds.
  void on_minusThreeSecond_clicked();

  /// @brief Increments the video by one frame.
  void on_plusOneFrame_clicked();

  /// @brief Decrements the video by one frame.
  void on_minusOneFrame_clicked();

  /// @brief Loads a video file.
  void on_loadVideo_clicked();

  /// @brief Loads an annotation file.
  void on_loadAnnotationFile_clicked();

  /// @brief Saves an annotation file.
  void on_saveAnnotationFile_clicked();

  /// @brief Writes the current frame to file.
  void on_writeImage_clicked();

  /// @brief Pauses the video and stores the play/pause state.
  void on_videoSlider_sliderPressed();

  /// @brief Continues the video if it was previously playing.
  void on_videoSlider_sliderReleased();

  /// @brief Sets the current frame to a new position.
  ///
  /// @param action Action that was done.
  void on_videoSlider_actionTriggered(int action);

  /// @brief Updates the current fish with a new species and updates the
  ///        subtype options.
  ///
  /// @param text Selected species.
  void on_typeMenu_activated(const QString &text);

  /// @brief Updates the current fish with a new subspecies.
  ///
  /// @param text Selected subspecies.
  void on_subTypeMenu_activated(const QString &text);

  /// @brief Sets current fish to next lowest ID.
  void on_prevFish_clicked();

  /// @brief Sets current fish to next highest ID.
  void on_nextFish_clicked();

  /// @brief Removes the current fish.
  void on_removeFish_clicked();

  /// @brief Goes to the first frame where a detection for this fish
  ///        occurs, if any.
  void on_goToFrame_clicked();

  /// @brief Updates the current fish to the specified ID.
  void on_goToFishVal_returnPressed();

  /// @brief Adds a region for the current fish.  If a region already 
  ///        exists for this fish and frame, an error is raised.
  void on_addRegion_clicked();

  /// @brief Removes a region for the current fish and frame.  If a 
  ///        region does not exist an error is raised.
  void on_removeRegion_clicked();

  /// @brief Goes to next frame and copies the region corresponding to
  ///        the current fish and frame.
  void on_nextAndCopy_clicked();

  /// @brief Sets the degraded state for the current frame.
  void on_degradedStatus_stateChanged(int state);
private:
  /// @brief Annotations associated with this video.
  std::unique_ptr<VideoAnnotation> annotation_;

  /// @brief Scene for displaying video.
  std::unique_ptr<QGraphicsScene> scene_;

  /// @brief Pixmap item for displaying video frames.
  QGraphicsPixmapItem *pixmap_item_;

  /// @brief Border rect for indicating degraded state.
  QGraphicsRectItem *visibility_box_;

  /// @brief Widget loaded from the ui file.
  std::unique_ptr<Ui::MainWidget> ui_;

  /// @brief Species controls widget.
  std::unique_ptr<SpeciesControls> species_controls_;

  /// @brief Path to loaded video.
  QString video_path_;

  /// @brief Video width.
  qint64 width_;

  /// @brief Video height.
  qint64 height_;

  /// @brief Last frame displayed by player.
  QImage last_frame_;

  /// @brief Last video position (frames).
  qint64 last_position_;

  /// @brief Whether player is stopped.
  bool stopped_;

  /// @brief Whether player was stopped before slider press.
  bool was_stopped_;

  /// @brief Current playback rate.
  double rate_;

  /// @brief Native playback rate.
  double native_rate_;

  /// @brief Currently selected fish ID.
  qint64 fish_id_;

  /// @brief Current annotations.
  std::list<AnnotatedRegion<DetectionAnnotation>*> current_annotations_;

  /// @brief Updates counts of each species in species controls.
  void updateSpeciesCounts();

  /// @brief Updates displayed fish statistics.
  void updateStats();

  /// @brief Draws annotations for the last displayed frame.
  void drawAnnotations();
};

}} // namespace fish_annotator::video_annotator

#endif //VIDEO_ANNOTATOR_MAINWINDOW_H
