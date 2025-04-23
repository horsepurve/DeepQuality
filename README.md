# DeepQuality: Mass Spectra Quality Assessment via Compressed Sensing and Deep Learning

## Brief Introduction
The following figure shows the DeepQuality pipeline. Generally, modern mass spectrometers have a mass range of 0~2000 Dalton and an accuracy of 0.01 Dalton, and generate centroid mass spectra, each containing roughly several hundred peaks, resulting in highly sparse signals (left panel). Traditional methods used machine learning on handcrafted features to distinguish between spectra of high and low quality. However, the handcrafted features are difficult to optimize. Compressed Sensing (CS) theory, mathematically, can recover signals with certain sparsity from far fewer samples than acquired by the Nyquist rate, and thus we combine CS and deep learning for end-to-end mass spectrum quality assessment (central panel). On two publicly available datasets, DeepQuality achieved AUC of **0.96** and **0.92**, significantly surpassing other software (right panel).
 </br><img src="https://github.com/horsepurve/DeepQuality/blob/master/img/figure_1.png" alt="figure_1" align=center />
By virtue of the mshadow library, DeepQuality can run seamlessly on both CPU and GPU.

## Usage

### Datasets
The two datasets used here are from the [paper](http://onlinelibrary.wiley.com/doi/10.1002/pmic.200500309/abstract) and can be downloaded [here](http://services.cbu.uib.no/software//spectrumquality).

### Binary
The precompiled binary can be found in the bin directory.

### Running
Fill in the Config.json file and run as:
```
CompressedSensing.exe Config.json
```

### Build from source
Four requirements are needed to compile the source code and run the software:
1. rapidjson: https://github.com/miloyip/rapidjson
2. mshadow: https://github.com/dmlc/mshadow
3. MXNet: https://github.com/apache/incubator-mxnet
4. OpenBLAS: http://www.openblas.net/

## Preprint
[arXiv:1710.11430](https://arxiv.org/abs/1710.11430)

## Questions
[contact](mailto:horsepurve@gmail.com)
