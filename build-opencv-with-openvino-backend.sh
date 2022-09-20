#!/bin/bash

OPENCV_DIR_NAME=opencv-4.6.0
OPENCV_DIR_PATH=$(pwd)/${OPENCV_DIR_NAME}
OPENVINO_DIR_NAME=openvino-2022.1.1
OPENVINO_DIR_PATH=$(pwd)/${OPENVINO_DIR_NAME}
INSTALL_DIR_PATH=$(pwd)/install

# download opencv-4.6.0
wget https://github.com/opencv/opencv/archive/refs/tags/4.6.0.zip
unzip 4.6.0.zip
# download openvino-2022.1.1
wget https://github.com/openvinotoolkit/openvino/releases/download/2022.1.1/l_openvino_toolkit_ubuntu20_2022.1.1.7030.39aba80957e.tgz
tar -xf l_openvino_toolkit_ubuntu20_2022.1.1.7030.39aba80957e.tgz
mv l_openvino_toolkit_ubuntu20_2022.1.1.7030.39aba80957e/ ${OPENVINO_DIR_NAME}

mkdir build
cd build || exit

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

# download OpenVINO person-detection-0200 model
wget https://storage.openvinotoolkit.org/repositories/open_model_zoo/2022.1/models_bin/3/person-detection-0200/FP32/person-detection-0200.xml
wget https://storage.openvinotoolkit.org/repositories/open_model_zoo/2022.1/models_bin/3/person-detection-0200/FP32/person-detection-0200.bin

export PYTHONPATH=${INSTALL_DIR_PATH}/lib/python3.8/site-packages/cv2/python-3.8/:$PYTHONPATH
python3 check.py
