#include <boost/filesystem.hpp>

#include "rect.h"
#include "annotation_scene.h"
#include "image_annotation.h"
#include "test_image_annotation.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

namespace fa = fish_annotator;
namespace ia = fish_annotator::image_annotator;

void TestImageAnnotation::testSerialize() {
  std::vector<boost::filesystem::path> image_list = {
    "ceres/ceres1.png",
    "ceres/ceres2.jpg",
    "ceres/ceres3.jpg",
    "ceres/ceres4.jpg"};
  ia::ImageAnnotationList list;
  list.insert(std::make_shared<ia::ImageAnnotation>(
    image_list[0].filename().string(), "asdfa", "ljklg", 0, 
    fish_annotator::Rect(1, 2, 3, 4), fa::kBox));
  list.insert(std::make_shared<ia::ImageAnnotation>(
    image_list[0].filename().string(), "ahsdifuasega", "ljsdasdhaklg", 1, 
    fish_annotator::Rect(3, 2, 1, 9), fa::kBox));
  list.insert(std::make_shared<ia::ImageAnnotation>(
    image_list[1].filename().string(), "ahspda8", "asha", 0, 
    fish_annotator::Rect(4, 3, 2, 8), fa::kBox));
  list.insert(std::make_shared<ia::ImageAnnotation>(
    image_list[3].filename().string(), "ahspda8", "ashaahsgha", 0, 
    fish_annotator::Rect(4, 3, 2, 8), fa::kBox));
  QVERIFY(list.getAllSpecies().size() == 3);
  QVERIFY(list.by_file_.left.count(image_list[0].filename().string()) == 2);
  list.write(image_list);
  ia::ImageAnnotationList deserialized_list;
  deserialized_list.read(image_list);
  QVERIFY(list == deserialized_list);
  list.remove(image_list[0], 1);
  list.write(image_list);
  ia::ImageAnnotationList deserialized_list_1;
  deserialized_list_1.read(image_list);
  QVERIFY(list == deserialized_list_1);
  QVERIFY(list.by_file_.left.count(image_list[0].filename().string()) == 1);
}

QTEST_MAIN(TestImageAnnotation)
#include "test_image_annotation.moc"

