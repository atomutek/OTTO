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
      Property<float> feedback                = {this, "feedback",  0,   has_limits::init(0, 0.4),  steppable::init(0.01)};
      Property<float> mAtt                    = {this, "mAtt",      0.2, has_limits::init(0,    1),  steppable::init(0.01)};
      Property<float> mDecrel                 = {this, "mDecrel",   0.5, has_limits::init(0,    1),  steppable::init(0.01)};
      Property<float> mSuspos                 = {this, "mSuspos",   0.5, has_limits::init(0,    1),  steppable::init(0.01)};
      //Oscillator
      Property<float> detune                  = {this, "detune",    0,   has_limits::init(-1,   1),  steppable::init(0.01)};
      Property<float, no_serialize> ratio     = {this, "ratio",     1,   has_limits::init(0.25, 4),  steppable::init(0.01)};
      Property<int, no<faust_link>> ratio_idx = {this, "ratio_idx", 0,   has_limits::init(0,    18), steppable::init(1)};
      //Amp
      Property<float> outLev                  = {this, "outLev",    1,   has_limits::init(0, 1),    steppable::init(0.01)};
      // clang-format on
    };

    struct OperatorEnvelope : Properties<> {
      using Properties::Properties;
      Property<float> modulator = {this, "modulator",    0,   has_limits::init(0,   1), faust_link::init(FaustLink::Type::FromFaust)};

    };

    struct VoiceEnvelope : Properties<> {
      using Properties::Properties;
      Property<float> carrier   = {this, "carrier",      0,   has_limits::init(0,   1), faust_link::init(FaustLink::Type::FromFaust)};
      std::array<OperatorEnvelope,4> ops = {{{this,"op0"}, {this,"op1"}, {this,"op2"}, {this,"op3"}}};
    };

    struct Props : Properties<> {
      Property<int> algN = {this, "algN",    0,   has_limits::init(0, 10),    steppable::init(1)};
      Property<float> fmAmount = {this, "fmAmount",    1,   has_limits::init(0, 1),    steppable::init(0.01)};

      std::array<Operator,4> operators = {{{this,"op0"}, {this,"op1"}, {this,"op2"}, {this,"op3"}}};
      std::array<VoiceEnvelope,6> voice_envelopes = {{{this,"v0"}, {this,"v1"}, {this,"v2"}, {this,"v3"}, {this,"v4"}, {this,"v5"}}};

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
    friend struct OTTOFMSynthScreen;
    audio::VoiceManager<6> voice_mgr_;
    audio::FaustWrapper<0, 1> faust_;
  };
} // namespace otto::engines
