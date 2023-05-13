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
target_compile_definitions(samplerate PRIVATE HAVE_LRINT HAVE_LRINTF)

add_library(rubberband STATIC
    extern/rubberband/single/RubberBandSingle.cpp
)
target_compile_features(rubberband PUBLIC cxx_std_11)
target_compile_definitions(rubberband PRIVATE NOMINMAX)
target_include_directories(rubberband PRIVATE extern/rubberband/src PUBLIC extern/rubberband)

juce_add_gui_app(
    editor
    PRODUCT_NAME "RC-505 Editor"
    VERSION "0.3.0"
    BUNDLE_ID "ch.westlicht.rc-505-editor"
    COMPANY_NAME "westlicht"
    COMPANY_WEBSITE "westlicht.ch"
    COMPANY_EMAIL "simon.kallweit@gmail.com"
    ICON_BIG "resources/icon-large.png"
    ICON_SMALL "resources/icon-small.png"
)

juce_generate_juce_header(editor)

juce_add_binary_data(
    editor_data
    HEADER_NAME BinaryData.h
    SOURCES
    resources/icon-large.png
    resources/Matrix.ttf
    resources/MEMORY.RC0
    resources/SYSTEM.RC0
)

target_sources(editor PRIVATE
    src/AudioEngine.cpp
    src/CustomLookAndFeel.cpp
    src/LibraryView.cpp
    src/LooperEngine.cpp
    src/Main.cpp
    src/MainComponent.cpp
    src/MemoryView.cpp
    src/PatchTreeView.cpp
    src/PatchView.cpp
    src/PropertySetDialog.cpp
    src/PropertyTreeView.cpp
    src/PropertyView.cpp
    src/RC505.cpp
    src/SystemView.cpp
    src/TooltipPanel.cpp
    src/TrackLabel.cpp
    src/TrackView.cpp
    src/Utils.cpp
    src/WaveformView.cpp
)

target_compile_definitions(editor
    PRIVATE
        JUCE_MODAL_LOOPS_PERMITTED
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)

target_link_libraries(editor
    PRIVATE
        editor_data
        samplerate
        rubberband
        juce::juce_gui_basics
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
