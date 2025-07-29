# JUCE_RingModSidechain_plus
In addition to the Ringmod Sidechain functions, the clip parameter controls the amplitude of the sidechain track, and the feedback coefficient parameter of the one-pole filter applied to the sidechain track.

## Demonstration
[Youtube<img width="1267" height="616" alt="Screenshot 2025-07-29 at 11 46 29" src="https://github.com/user-attachments/assets/3195d5af-96ba-4db5-9a66-fdd2502d9490" />
](https://youtu.be/vp6g_kOnx8Y?si=7ig8t-EMjw8FdxSF)



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
