# DeepQuality: Mass Spectra Quality Assessment via Compressed Sensing and Deep Learning

## Preprint
arxiv:

## Introduction
The following figure shows the pipeline of DeepQuality algorithm. Generally, modern mass spectrometers have a mass range of 0~2000 Dalton and accuracy of 0.01 Dalton, and generate mass spectra containing roughly several hundred peaks, resulting in highly sparse signals (left panel). Compressed Sensing (CS) theory, mathematically, can recover signals with certain sparsity from far few samples then acquired by the Nyquist rate, and thus we combine CS and deep learning for end-to-end mass spectrum quality assessment (central panel). On two publicly  datasets, DeepQuality achieved AUC of **0.96** and **0.92**, significantly surpass other software (right panel).
 <img src="https://github.com/horsepurve/DeepQuality/blob/master/img/figure_1.png" alt="figure_1" align=center />

## Usage

### Build from source
Four requirements are needed to compile the source code:
1. rapidjson: https://github.com/miloyip/rapidjson
2. mshadow: https://github.com/dmlc/mshadow
3. MXNet: https://github.com/apache/incubator-mxnet
4. OpenBLAS: http://www.openblas.net/

### Binary
The precompiled binary can be found in bin directory.

### Running
```
CompressedSensing.exe Config.json
```

## contact
machunwei@genomics.cn
