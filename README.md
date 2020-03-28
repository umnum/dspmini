# Moog Ladder Filter

<a href="https://drive.google.com/file/d/16F_a_m2T32haqDgcwwjGefTxDjV848CL/view?usp=sharing">
  <img src="https://i.imgur.com/ih82iuv.png" width="500" href="https://drive.google.com/file/d/16F_a_m2T32haqDgcwwjGefTxDjV848CL/view?usp=sharing">
</a>

### Synopsis
This is a Matlab and JUCE implementation of a Moog Ladder Filter based on A. Huovilainen's 2004 paper [Non-Linear Digital Implementation of the Moog Ladder Filter](https://pdfs.semanticscholar.org/c490/4c04a7be1d675e360409178da71a1253f6d8.pdf).
This plugin includes a GUI interface for easy interaction with the filter's parameters and for visualization of the audio spectrum contents.

### Motivation
This implementation was part of a Signal Processing mini project at Aalborg University as a part of my Master's program in Sound and Music Computing.
I have recently revised this implementation mainly to fix an audio issue, write cleaner and more presentable code, update the program for the current version of JUCE, and provide a better GUI interface.

### Install
The Moog Ladder Filter is available for macOS as an AU and VST3 plugin. Download the package installer <a href="https://github.com/umnum/dspmini/blob/master/Moog/Install/MacOSX/Moog%20Ladder%20Filter.dmg?raw=true" download>here</a>.

## Built With

* [JUCE](https://juce.com) (version 5.4.7)
* [Xcode](https://developer.apple.com/xcode/) (version 11.3.1)
* [JKnobMan](https://www.g200kg.com/jp/software/knobman.html) (version 1.3.3)

## Author

* **Michael Castanieto** - *Initial work* - [umnum](https://github.com/umnum)
