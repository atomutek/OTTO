#pragma once

#include "core/engine/engine.hpp"

#include "core/audio/faust.hpp"
#include "core/audio/voice_manager.hpp"

namespace otto::engines {

  using namespace core;
  using namespace core::engine;
  using namespace props;

  struct OTTOFMSynth : SynthEngine, EngineWithEnvelope {
    struct Operator : Properties<> {
      using Properties::Properties;
      // clang-format off
      //Envelopes
      Property<float> cAtt                    = {this, "cAtt",      0,   has_limits::init(-1,   1),  steppable::init(0.01)};
      Property<float> cSus                    = {this, "cSus",      0,   has_limits::init(-1,   1),  steppable::init(0.01)};
      Property<float> cRel                    = {this, "cRel",      0,   has_limits::init(-1,   1),  steppable::init(0.01)};
      Property<float> mAtt                    = {this, "mAtt",      0.2, has_limits::init(0,    1),  steppable::init(0.01)};
      Property<float> mDecrel                 = {this, "mDecrel",   0.5, has_limits::init(0,    1),  steppable::init(0.01)};
      Property<float> mSuspos                 = {this, "mSuspos",   0.5, has_limits::init(0,    1),  steppable::init(0.01)};
      //Oscillator
      Property<float> detune                  = {this, "detune",    0,   has_limits::init(-1,   1),  steppable::init(0.01)};
      Property<float, no_serialize> ratio     = {this, "ratio",     0,   has_limits::init(0.25, 4),  steppable::init(0.01)};
      Property<int, no<faust_link>> ratio_idx = {this, "ratio_idx", 0,   has_limits::init(0,    18), steppable::init(1)};
      //Amp
      Property<float> outLev                  = {this, "outLev",    1,   has_limits::init(0, 1),    steppable::init(0.01)};
      // clang-format on
    };

    struct Props : Properties<> {
      Property<int> algN = {this, "algN", 1, has_limits::init(0, 11), steppable::init(1)};

      std::array<Operator, 4> operators = {
        {{this, "op0"}, {this, "op1"}, {this, "op2"}, {this, "op3"}}};
      Property<float> fmAmount = {this, "fmAmount",    1,   has_limits::init(0, 1),    steppable::init(0.01)};
      Property<int> cur_op =     {this, "Current_operator", 0, has_limits::init(0, 3)};
      Property<bool,no<faust_link>,no_serialize> shift = {this, "shift"};

    } props;



    OTTOFMSynth();

    audio::ProcessData<1> process(audio::ProcessData<1>) override;

    ui::Screen& envelope_screen() override
    {
      return voice_mgr_.envelope_screen();
    }

    ui::Screen& voices_screen() override
    {
      return voice_mgr_.settings_screen();
    }

  private:
    audio::VoiceManager<6> voice_mgr_;
    audio::FaustWrapper<0, 1> faust_;
  };
} // namespace otto::engines
