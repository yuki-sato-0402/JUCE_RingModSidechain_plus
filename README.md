# JUCE_RingModSidechain_plus
In addition to the Ringmod Sidechain functions, the clip parameter controls the amplitude of the sidechain track, and the feedback coefficient parameter of the one-pole filter applied to the sidechain track.


## 🛠️ Build Instructions
```
cd JUCE_FirFilter
git submodule update --init --recursive
cd build
cmake ..
cmake --build .
```

## Reference
- [Tutorial: Configuring the right bus layouts for your plugins](https://juce.com/tutorials/tutorial_audio_bus_layouts/)

- [Tutorial: Plugin examples](https://juce.com/tutorials/tutorial_plugin_examples/)