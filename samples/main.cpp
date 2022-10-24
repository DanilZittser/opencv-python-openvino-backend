#include <fstream>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


static const cv::Scalar GREEN(0, 255, 0);

static bool file_exists(const std::string & filepath) {
    return std::ifstream(filepath.c_str()).good();
}

template <typename T>
static T mean(std::vector<T> & vector) {
    T sum = 0;
    for (T x : vector) sum += x;
    return sum / vector.size();
}

static void show_usage(const std::string & name)
{
    std::cerr << "Usage: " << name << "\n"
              << "Options:\n"
              << "\t--help \t\tShow this help message\n"
              << "\t--xml \t\tPath to model *.xml file\n"
              << "\t--bin \t\tPath to model *.bin file\n"
              << "\t--video \tPath to video\n"
              << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 7) {
        show_usage(argv[0]);
        return 1;
    }

    std::string xml_path, bin_path, video_path;
    std::vector<std::string> paths;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--xml") {
            xml_path = argv[++i];
            paths.push_back(xml_path);
        } else if (arg == "--bin") {
            bin_path = argv[++i];
            paths.push_back(bin_path);
        } else if (arg == "--video") {
            video_path = argv[++i];
            paths.push_back(video_path);
        }
    }

    bool all_files_exist = true;
    for (const std::string & filepath : paths) {
        if (!file_exists(filepath)) {
            printf("File \"%s\" not found\n", filepath.data());
            all_files_exist = false;
        };
    }

    if (!all_files_exist) return 1;

    cv::dnn::Net net = cv::dnn::readNetFromModelOptimizer(xml_path, bin_path);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_INFERENCE_ENGINE);

    cv::VideoCapture capture(video_path);

    cv::Mat frame;
    std::vector<double> fps;

    while (true) {
        bool success = capture.read(frame);

        if (!success) break;

        net.setInput(cv::dnn::blobFromImage(frame, 1.0, cv::Size(256, 256)));

        int64 tic = cv::getTickCount();
        auto outputs = (float*) net.forward().data;
        int64 toc = cv::getTickCount();
        fps.push_back(cv::getTickFrequency() / (toc - tic));

        float score = outputs[2];
        cv::Point p1(int(outputs[3] * frame.cols), int(outputs[4] * frame.rows));
        cv::Point p2(int(outputs[5] * frame.cols), int(outputs[6] * frame.rows));
        cv::rectangle(frame, p1, p2, GREEN, 3);
        cv::putText(frame, cv::format("score = %.3f", score), p1, 0, 1, GREEN, 2);
        cv::putText(frame, cv::format("fps = %d", int(mean(fps))), cv::Point(0, 30), 0, 1, GREEN, 2);

        cv::imshow("Press \"q\" for exit", frame);

        if (char(cv::waitKey(42)) == 'q') break;
    }

    printf("Throughput = %.2f fps", mean(fps));
}