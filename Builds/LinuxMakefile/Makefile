# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_6D53C8B4=1" -D "JUCE_APP_VERSION=0.0.2" -D "JUCE_APP_VERSION_HEX=0x2" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -msse2 -I../../Source/rubberband
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := rc-505-editor
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6D53C8B4=1" -D "JUCE_APP_VERSION=0.0.2" -D "JUCE_APP_VERSION_HEX=0x2" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../JuceLibraryCode/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3 -msse2 -I../../Source/rubberband
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := rc-505-editor
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/samplerate_c3f15f3.o \
  $(OBJDIR)/src_linear_58faa0c9.o \
  $(OBJDIR)/src_sinc_eabd1e6f.o \
  $(OBJDIR)/src_zoh_41c63279.o \
  $(OBJDIR)/CompoundAudioCurve_6227ddc4.o \
  $(OBJDIR)/ConstantAudioCurve_da138ffd.o \
  $(OBJDIR)/HighFrequencyAudioCurve_dc8b4dfd.o \
  $(OBJDIR)/PercussiveAudioCurve_8f452e62.o \
  $(OBJDIR)/SilentAudioCurve_9024cdae.o \
  $(OBJDIR)/SpectralDifferenceAudioCurve_8e7d4d9a.o \
  $(OBJDIR)/Profiler_828588c6.o \
  $(OBJDIR)/AudioCurveCalculator_46475234.o \
  $(OBJDIR)/FFT_45db0585.o \
  $(OBJDIR)/Resampler_a92452c6.o \
  $(OBJDIR)/kiss_fft_6693df97.o \
  $(OBJDIR)/kiss_fftr_6be90c85.o \
  $(OBJDIR)/resample_5c93213c.o \
  $(OBJDIR)/Allocators_5a1aa587.o \
  $(OBJDIR)/sysutils_f49d3023.o \
  $(OBJDIR)/Thread_8316fc9.o \
  $(OBJDIR)/VectorOpsComplex_d473f100.o \
  $(OBJDIR)/RubberBandStretcher_5a4c48c.o \
  $(OBJDIR)/StretchCalculator_3dd0fa06.o \
  $(OBJDIR)/StretcherChannelData_436ad386.o \
  $(OBJDIR)/StretcherImpl_eaa2c951.o \
  $(OBJDIR)/StretcherProcess_eaf48ba8.o \
  $(OBJDIR)/AudioEngine_12afc819.o \
  $(OBJDIR)/CustomLookAndFeel_ab3d772e.o \
  $(OBJDIR)/LibraryView_f7f69861.o \
  $(OBJDIR)/LooperEngine_1aeddefc.o \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/MainComponent_a6ffb4a5.o \
  $(OBJDIR)/MemoryView_f9aa85cf.o \
  $(OBJDIR)/PatchTreeView_100561ac.o \
  $(OBJDIR)/PatchView_448cb1ee.o \
  $(OBJDIR)/PropertySetDialog_1b847cd6.o \
  $(OBJDIR)/PropertyTreeView_5b1bfae1.o \
  $(OBJDIR)/PropertyView_f2e22da3.o \
  $(OBJDIR)/RC505_1d39b82a.o \
  $(OBJDIR)/SystemView_a2cb2cfd.o \
  $(OBJDIR)/TooltipPanel_d75f3bca.o \
  $(OBJDIR)/TrackLabel_3212ed72.o \
  $(OBJDIR)/TrackView_3138cff1.o \
  $(OBJDIR)/Utils_e4b11b92.o \
  $(OBJDIR)/WaveformView_80f7e30b.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(OBJDIR)/juce_audio_devices_a742c38b.o \
  $(OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(OBJDIR)/juce_audio_processors_dea3173d.o \
  $(OBJDIR)/juce_audio_utils_c7eb679f.o \
  $(OBJDIR)/juce_core_75b14332.o \
  $(OBJDIR)/juce_data_structures_72d3da2c.o \
  $(OBJDIR)/juce_events_d2be882c.o \
  $(OBJDIR)/juce_graphics_9c18891e.o \
  $(OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(OBJDIR)/juce_gui_extra_4a026f23.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking RC-505 Editor
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning RC-505 Editor
	@$(CLEANCMD)

strip:
	@echo Stripping RC-505 Editor
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/samplerate_c3f15f3.o: ../../Source/libsamplerate/samplerate.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling samplerate.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/src_linear_58faa0c9.o: ../../Source/libsamplerate/src_linear.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling src_linear.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/src_sinc_eabd1e6f.o: ../../Source/libsamplerate/src_sinc.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling src_sinc.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/src_zoh_41c63279.o: ../../Source/libsamplerate/src_zoh.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling src_zoh.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CompoundAudioCurve_6227ddc4.o: ../../Source/rubberband/audiocurves/CompoundAudioCurve.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CompoundAudioCurve.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ConstantAudioCurve_da138ffd.o: ../../Source/rubberband/audiocurves/ConstantAudioCurve.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ConstantAudioCurve.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/HighFrequencyAudioCurve_dc8b4dfd.o: ../../Source/rubberband/audiocurves/HighFrequencyAudioCurve.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling HighFrequencyAudioCurve.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PercussiveAudioCurve_8f452e62.o: ../../Source/rubberband/audiocurves/PercussiveAudioCurve.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PercussiveAudioCurve.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SilentAudioCurve_9024cdae.o: ../../Source/rubberband/audiocurves/SilentAudioCurve.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SilentAudioCurve.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SpectralDifferenceAudioCurve_8e7d4d9a.o: ../../Source/rubberband/audiocurves/SpectralDifferenceAudioCurve.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SpectralDifferenceAudioCurve.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Profiler_828588c6.o: ../../Source/rubberband/base/Profiler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Profiler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioCurveCalculator_46475234.o: ../../Source/rubberband/dsp/AudioCurveCalculator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioCurveCalculator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FFT_45db0585.o: ../../Source/rubberband/dsp/FFT.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FFT.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Resampler_a92452c6.o: ../../Source/rubberband/dsp/Resampler.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Resampler.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/kiss_fft_6693df97.o: ../../Source/rubberband/kissfft/kiss_fft.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling kiss_fft.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/kiss_fftr_6be90c85.o: ../../Source/rubberband/kissfft/kiss_fftr.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling kiss_fftr.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/resample_5c93213c.o: ../../Source/rubberband/speex/resample.c
	-@mkdir -p $(OBJDIR)
	@echo "Compiling resample.c"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Allocators_5a1aa587.o: ../../Source/rubberband/system/Allocators.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Allocators.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/sysutils_f49d3023.o: ../../Source/rubberband/system/sysutils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling sysutils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Thread_8316fc9.o: ../../Source/rubberband/system/Thread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Thread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/VectorOpsComplex_d473f100.o: ../../Source/rubberband/system/VectorOpsComplex.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling VectorOpsComplex.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RubberBandStretcher_5a4c48c.o: ../../Source/rubberband/RubberBandStretcher.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RubberBandStretcher.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/StretchCalculator_3dd0fa06.o: ../../Source/rubberband/StretchCalculator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling StretchCalculator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/StretcherChannelData_436ad386.o: ../../Source/rubberband/StretcherChannelData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling StretcherChannelData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/StretcherImpl_eaa2c951.o: ../../Source/rubberband/StretcherImpl.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling StretcherImpl.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/StretcherProcess_eaf48ba8.o: ../../Source/rubberband/StretcherProcess.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling StretcherProcess.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioEngine_12afc819.o: ../../Source/AudioEngine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioEngine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CustomLookAndFeel_ab3d772e.o: ../../Source/CustomLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CustomLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LibraryView_f7f69861.o: ../../Source/LibraryView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LibraryView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LooperEngine_1aeddefc.o: ../../Source/LooperEngine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LooperEngine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MemoryView_f9aa85cf.o: ../../Source/MemoryView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MemoryView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PatchTreeView_100561ac.o: ../../Source/PatchTreeView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PatchTreeView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PatchView_448cb1ee.o: ../../Source/PatchView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PatchView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PropertySetDialog_1b847cd6.o: ../../Source/PropertySetDialog.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PropertySetDialog.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PropertyTreeView_5b1bfae1.o: ../../Source/PropertyTreeView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PropertyTreeView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PropertyView_f2e22da3.o: ../../Source/PropertyView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PropertyView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RC505_1d39b82a.o: ../../Source/RC505.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RC505.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SystemView_a2cb2cfd.o: ../../Source/SystemView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SystemView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TooltipPanel_d75f3bca.o: ../../Source/TooltipPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TooltipPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrackLabel_3212ed72.o: ../../Source/TrackLabel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TrackLabel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrackView_3138cff1.o: ../../Source/TrackView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TrackView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Utils_e4b11b92.o: ../../Source/Utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/WaveformView_80f7e30b.o: ../../Source/WaveformView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling WaveformView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_c7eb679f.o: ../../JuceLibraryCode/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
