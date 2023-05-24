#include "plugin.hpp"


struct Nudge : Module {
	enum ParamId {
		SCALE_PARAM,
		OFFSET_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		MAIN_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		MAIN_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	Nudge() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(SCALE_PARAM, 0.f, 2.f, 1.f, "scale");
		configParam(OFFSET_PARAM, -10.f, 10.f, 0.f, "offset");
		configInput(MAIN_INPUT, "");
		configOutput(MAIN_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
		outputs[MAIN_OUTPUT].setVoltage((inputs[MAIN_INPUT].getVoltage() * params[SCALE_PARAM].getValue()) + params[OFFSET_PARAM].getValue());
	}
};

struct NudgeWidget : ModuleWidget {
	NudgeWidget(Nudge* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/nudge.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10.189, 43.901)), module, Nudge::SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10.189, 70.006)), module, Nudge::OFFSET_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.189, 24.282)), module, Nudge::MAIN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(10.189, 108.061)), module, Nudge::MAIN_OUTPUT));
	}
};


Model* modelNudge = createModel<Nudge, NudgeWidget>("nudge");
