# Run OpenVINO model via OpenCV DNN C++ and Python API

This project present how to:
- build opencv with openvino support
- do inference openvino model via OpenCV DNN C++ API

Also, it compares model throughput on C++ and Python. 

Usefull links:
- [OpenCV: Open Source Computer Vision Library](https://github.com/opencv/opencv)
- [OpenVINO™ Toolkit](https://github.com/openvinotoolkit/openvino)
- [OpenCV installation overview ](https://docs.opencv.org/4.x/d0/d3d/tutorial_general_install.html)
- [OpenCV configuration options reference](https://docs.opencv.org/4.x/db/d05/tutorial_config_reference.html)
- [Intel's Deep Learning Inference Engine backend](https://github.com/opencv/opencv/wiki/Intel%27s-Deep-Learning-Inference-Engine-backend)
- [BuildOpenCV4OpenVINO](https://github.com/opencv/opencv/wiki/BuildOpenCV4OpenVINO)
- [Using OpenCV with gcc and CMake](https://docs.opencv.org/4.x/db/df5/tutorial_linux_gcc_cmake.html)
- [Mat - The Basic Image Container](https://docs.opencv.org/4.x/d6/d6d/tutorial_mat_the_basic_image_container.html)
- [YOLO DNNs](https://docs.opencv.org/4.x/da/d9d/tutorial_dnn_yolo.html)

## Prerequisites
```bash
sudo apt remove --purge cmake
sudo apt update
sudo apt install -y cmake unzip wget
```

## Clone repo
```bash
git clone git@github.com:DanilZittser/opencv-python-openvino-backend.git
cd opencv-python-openvino-backend
```

## Declare environment variables
```bash
OPENCV_DIR_NAME=opencv-4.6.0
OPENCV_DIR_PATH=$(pwd)/${OPENCV_DIR_NAME}
OPENVINO_DIR_NAME=openvino-2022.1.1
OPENVINO_DIR_PATH=$(pwd)/${OPENVINO_DIR_NAME}
INSTALL_DIR_PATH=$(pwd)/install
```

## Download OpenCV and OpenVINO
```bash
# download opencv-4.6.0
wget https://github.com/opencv/opencv/archive/refs/tags/4.6.0.zip
unzip 4.6.0.zip
# download openvino-2022.1.1
wget https://github.com/openvinotoolkit/openvino/releases/download/2022.1.1/l_openvino_toolkit_ubuntu20_2022.1.1.7030.39aba80957e.tgz
tar -xf l_openvino_toolkit_ubuntu20_2022.1.1.7030.39aba80957e.tgz
mv l_openvino_toolkit_ubuntu20_2022.1.1.7030.39aba80957e/ ${OPENVINO_DIR_NAME}
```

## Build and install
```bash
mkdir build && cd build

cmake \
  -DWITH_OPENVINO=ON \
  -DINF_ENGINE_RELEASE=2022010100 \
  -DInferenceEngine_DIR=${OPENVINO_DIR_PATH}/runtime/cmake \
  -DOpenVINO_DIR=${OPENVINO_DIR_PATH}/runtime/cmake \
  -Dngraph_DIR=${OPENVINO_DIR_PATH}/runtime/cmake \
  -DOPENCV_GAPI_WITH_OPENVINO=OFF \
  -DOPENCV_DNN_OPENVINO=ON \
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR_PATH} \
  ${OPENCV_DIR_PATH}

make -j$(nproc) && make install

cd ..
```

## Do model inference
#### Download model

```bash
# download OpenVINO person-detection-0200 model
wget https://storage.openvinotoolkit.org/repositories/open_model_zoo/2022.1/models_bin/3/person-detection-0200/FP32/person-detection-0200.xml
wget https://storage.openvinotoolkit.org/repositories/open_model_zoo/2022.1/models_bin/3/person-detection-0200/FP32/person-detection-0200.bin
```

For details of model output read model description - [person-detection-0200](https://github.com/openvinotoolkit/open_model_zoo/tree/master/models/intel/person-detection-0200).

The results of launches on [HP OMEN Laptop 15-en1021ur](https://support.hp.com/rs-en/document/c07994653).

#### Python inference
```bash
export PYTHONPATH=${INSTALL_DIR_PATH}/lib/python3.8/site-packages/cv2/python-3.8/:$PYTHONPATH
python3 samples/main.py --xml person-detection-0200.xml --bin person-detection-0200.bin --video person.mp4

>>> Throughput = 259.16 fps
```

#### C++ inference
```bash
export OpenCV_DIR=${INSTALL_DIR_PATH}/lib/cmake/opencv4

mkdir cmake-build && cd $_
cmake ../
make -j$(nproc)
./opencv_python_openvino_backend --xml ../person-detection-0200.xml --bin ../person-detection-0200.bin --video ../person.mp4

>>> Throughput = 269.25 fps
```


