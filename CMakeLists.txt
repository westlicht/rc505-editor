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
    extern/rubberband/single/RubberBandSingle.cpp
)
target_compile_features(rubberband PUBLIC cxx_std_11)
target_include_directories(rubberband PRIVATE extern/rubberband/src PUBLIC extern/rubberband)

juce_add_gui_app(
    editor
    PRODUCT_NAME "RC-505 Editor"
    VERSION "0.3.0"
    BUNDLE_ID "ch.westlicht.rc-505-editor"
    COMPANY_NAME "westlicht"
    COMPANY_WEBSITE "westlicht.ch"
    COMPANY_EMAIL "simon.kallweit@gmail.com"
    ICON_BIG "Resources/icon-large.png"
    ICON_SMALL "Resources/icon-small.png"
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
        juce::juce_recommended_warning_flags
)
