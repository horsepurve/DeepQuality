# DeepQuality: Mass Spectra Quality Assessment via Compressed Sensing and Deep Learning

<!-- ## Preprint -->
<!-- [arXiv:1710.11430](https://arxiv.org/abs/1710.11430) (31 Oct 2017) -->
## Citation
Please cite as:
@article{ma2017deepquality,
</br>&nbsp;&nbsp;title={DeepQuality: Mass Spectra Quality Assessment via Compressed Sensing and Deep Learning},
</br>&nbsp;&nbsp;author={Ma, Chunwei},
</br>&nbsp;&nbsp;journal={arXiv preprint [arXiv:1710.11430](https://arxiv.org/abs/1710.11430)},
</br>&nbsp;&nbsp;year={2017}
}

## Brief Introduction
The following figure shows the pipeline of DeepQuality algorithm. Generally, modern mass spectrometers have a mass range of 0~2000 Dalton and accuracy of 0.01 Dalton, and generate centroid mass spectra each containing roughly several hundred peaks, resulting in highly sparse signals (left panel). Traditional methods used machine learning upon handcrafted features to distinguish between spectra of high and low quality. However, the handcrafted features are difficult to be optimized. Compressed Sensing (CS) theory, mathematically, can recover signals with certain sparsity from far few samples then acquired by the Nyquist rate, and thus we combine CS and deep learning for end-to-end mass spectrum quality assessment (central panel). On two publicly available datasets, DeepQuality achieved AUC of **0.96** and **0.92**, significantly surpasses other software (right panel).
 </br><img src="https://github.com/horsepurve/DeepQuality/blob/master/img/figure_1.png" alt="figure_1" align=center />

## Usage

### Datasets
The two datasets used here is from the [paper](http://onlinelibrary.wiley.com/doi/10.1002/pmic.200500309/abstract) and can be downloaded [here](http://services.cbu.uib.no/software//spectrumquality).

### Build from source
Four requirements are needed to compile the source code and run the software:
1. rapidjson: https://github.com/miloyip/rapidjson
2. mshadow: https://github.com/dmlc/mshadow
3. MXNet: https://github.com/apache/incubator-mxnet
4. OpenBLAS: http://www.openblas.net/

### Binary
The precompiled binary can be found in bin directory.

### Running
Fill in the Config.json file and run as:
```
CompressedSensing.exe Config.json
```

## contact
DeepQuality is a Chunwei Ma production and is free for non-commercial use. (machunwei@genomics.cn)
