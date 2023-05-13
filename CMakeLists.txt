cmake_minimum_required(VERSION 3.15)

project(rc505-editor)

add_subdirectory(JUCE)

add_library(samplerate STATIC
    extern/libsamplerate/samplerate.c
    extern/libsamplerate/src_linear.c
    extern/libsamplerate/src_sinc.c
    extern/libsamplerate/src_zoh.c
)
target_include_directories(samplerate PUBLIC extern)

add_library(rubberband STATIC
    extern/rubberband/audiocurves/CompoundAudioCurve.cpp
    extern/rubberband/audiocurves/ConstantAudioCurve.cpp
    extern/rubberband/audiocurves/HighFrequencyAudioCurve.cpp
    extern/rubberband/audiocurves/PercussiveAudioCurve.cpp
    extern/rubberband/audiocurves/SilentAudioCurve.cpp
    extern/rubberband/audiocurves/SpectralDifferenceAudioCurve.cpp
    extern/rubberband/base/Profiler.cpp
    extern/rubberband/dsp/AudioCurveCalculator.cpp
    extern/rubberband/dsp/FFT.cpp
    extern/rubberband/dsp/Resampler.cpp
    extern/rubberband/kissfft/kiss_fft.c
    extern/rubberband/kissfft/kiss_fftr.c
    extern/rubberband/RubberBandStretcher.cpp
    extern/rubberband/speex/resample.c    
    extern/rubberband/StretchCalculator.cpp
    extern/rubberband/StretcherChannelData.cpp
    extern/rubberband/StretcherImpl.cpp
    extern/rubberband/StretcherProcess.cpp
    extern/rubberband/system/Allocators.cpp
    extern/rubberband/system/sysutils.cpp
    extern/rubberband/system/Thread.cpp
    extern/rubberband/system/VectorOpsComplex.cpp
)
target_include_directories(rubberband PRIVATE extern/rubberband PUBLIC extern)

juce_add_gui_app(
    editor
    PRODUCT_NAME "RC505 Editor"
    VERSION "0.0.1"
    ICON_BIG Resources/icon-large.png
    ICON_SMALL Resources/icon-small.png
)

juce_generate_juce_header(editor)

juce_add_binary_data(
    editor_data
    HEADER_NAME BinaryData.h
    SOURCES
    Resources/icon-large.png
    Resources/Matrix.ttf
    Resources/MEMORY.RC0
    Resources/SYSTEM.RC0
)

target_sources(editor PRIVATE
    Source/AudioEngine.cpp
    Source/CustomLookAndFeel.cpp
    Source/LibraryView.cpp
    Source/LooperEngine.cpp
    Source/Main.cpp
    Source/MainComponent.cpp
    Source/MemoryView.cpp
    Source/PatchTreeView.cpp
    Source/PatchView.cpp
    Source/PropertySetDialog.cpp
    Source/PropertyTreeView.cpp
    Source/PropertyView.cpp
    Source/RC505.cpp
    Source/SystemView.cpp
    Source/TooltipPanel.cpp
    Source/TrackLabel.cpp
    Source/TrackView.cpp
    Source/Utils.cpp
    Source/WaveformView.cpp
)

target_compile_definitions(editor
    PRIVATE
        JUCE_MODAL_LOOPS_PERMITTED
)

target_link_libraries(editor
    PRIVATE
        editor_data
        samplerate
        rubberband
        juce::juce_gui_extra
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        # juce::juce_recommended_warning_flags
)
