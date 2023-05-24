#include "plugin.hpp"


struct Seesaw : Module {
	enum ParamId {
		CLOCK_PARAM,
		RESET_PARAM,
		STEPS_A_PARAM,
		STEPS_B_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		CLOCK_INPUT,
		RESET_INPUT,
		A_INPUT,
		B_INPUT,
		STEPS_A_INPUT,
		STEPS_B_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		SWAP_OUTPUT,
		COMBINE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		STEPS_A_LIGHT,
		STEPS_B_LIGHT,
		LIGHTS_LEN
	};

	dsp::SchmittTrigger clockTrigger;
	dsp::SchmittTrigger resetTrigger;
	int currentStep = 0;

	Seesaw() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);

		configParam(STEPS_A_PARAM, 0, 9, 1, "steps for a");
		getParamQuantity(STEPS_A_PARAM)->snapEnabled = true;

		configParam(STEPS_B_PARAM, 0, 9, 1, "steps for b");
		getParamQuantity(STEPS_B_PARAM)->snapEnabled = true;

		configInput(CLOCK_INPUT, "clock");
		configParam(CLOCK_PARAM, 0, 1, 0, "clock");

		configInput(RESET_INPUT, "reset");
		configParam(RESET_PARAM, 0, 1, 0, "reset");

		configInput(STEPS_A_INPUT, "steps for a");
		getInputInfo(STEPS_A_INPUT)->description = "0-10V, 1V/step";

		configInput(STEPS_B_INPUT, "steps for b");
		getInputInfo(STEPS_B_INPUT)->description = "0-10V, 1V/step";

		configInput(A_INPUT, "a");
		configInput(B_INPUT, "b");

		configOutput(SWAP_OUTPUT, "a or b");
		getOutputInfo(SWAP_OUTPUT)->description = "swaps between a and b";

		configOutput(COMBINE_OUTPUT, "a or a + b");
		getOutputInfo(COMBINE_OUTPUT)->description = "swaps between just a and both inputs combined";

		lights[STEPS_A_LIGHT].setBrightness(1);
		lights[STEPS_B_LIGHT].setBrightness(0);
	}

	void process(const ProcessArgs& args) override {
		bool shouldReset = resetTrigger.process(inputs[RESET_INPUT].getVoltage() + params[RESET_PARAM].getValue());
		bool shouldClock = clockTrigger.process(inputs[CLOCK_INPUT].getVoltage() + params[CLOCK_PARAM].getValue());

		int stepsForA = inputs[STEPS_A_INPUT].isConnected() ? clamp(floor(inputs[STEPS_A_INPUT].getVoltage()), 0, 9) : params[STEPS_A_PARAM].getValue();
		int stepsForB = inputs[STEPS_B_INPUT].isConnected() ? clamp(floor(inputs[STEPS_B_INPUT].getVoltage()), 0, 9) : params[STEPS_B_PARAM].getValue();

		int channelsForA = inputs[A_INPUT].getChannels();
		int channelsForB = inputs[B_INPUT].getChannels();

		// if both are zero just give up and set A to one
		if(stepsForA == 0 && stepsForB == 0) stepsForA = 1;

		if (shouldReset || shouldClock) {
			if(shouldReset) currentStep = 0;
			else currentStep = (currentStep + 1) % (stepsForA + stepsForB);
			
			if(currentStep < stepsForA) {
				lights[STEPS_A_LIGHT].setBrightness(1);
				lights[STEPS_B_LIGHT].setBrightness(0);
			} else {
				lights[STEPS_A_LIGHT].setBrightness(0);
				lights[STEPS_B_LIGHT].setBrightness(1);
			}
		}

		if(currentStep < stepsForA) {
			outputs[SWAP_OUTPUT].setChannels(channelsForA);
			outputs[COMBINE_OUTPUT].setChannels(channelsForA);
			for(int i = 0; i < channelsForA; i++) {
				outputs[SWAP_OUTPUT].setVoltage(inputs[A_INPUT].getVoltage(i), i);
				outputs[COMBINE_OUTPUT].setVoltage(inputs[A_INPUT].getVoltage(i), i);
			}
		} else {
			int maxChannels = std::max(channelsForA, channelsForB);
			outputs[SWAP_OUTPUT].setChannels(channelsForB);
			outputs[COMBINE_OUTPUT].setChannels(maxChannels);
			for(int i = 0; i < channelsForB; i++) {
				outputs[SWAP_OUTPUT].setVoltage(inputs[B_INPUT].getVoltage(i), i);
			}

			for(int i = 0; i < maxChannels; i++) {
				outputs[COMBINE_OUTPUT].setVoltage(inputs[A_INPUT].getVoltage(i) + inputs[B_INPUT].getVoltage(i), i);
			}
		}
	}
};

struct SeesawWidget : ModuleWidget {
	SeesawWidget(Seesaw* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/seesaw.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<LEDButton>(mm2px(Vec(7.764, 18.679)), module, Seesaw::CLOCK_PARAM));
		addParam(createParamCentered<LEDButton>(mm2px(Vec(22.544, 18.679)), module, Seesaw::RESET_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.606, 66.628)), module, Seesaw::STEPS_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(22.386, 66.628)), module, Seesaw::STEPS_B_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.606, 31.777)), module, Seesaw::CLOCK_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.386, 31.777)), module, Seesaw::RESET_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.606, 50.229)), module, Seesaw::A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.386, 50.229)), module, Seesaw::B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.606, 89.666)), module, Seesaw::STEPS_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.386, 89.666)), module, Seesaw::STEPS_B_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(7.606, 108.061)), module, Seesaw::SWAP_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.386, 108.061)), module, Seesaw::COMBINE_OUTPUT));

		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(7.606, 74.611)), module, Seesaw::STEPS_A_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(22.386, 74.611)), module, Seesaw::STEPS_B_LIGHT));
	}
};

Model* modelSeesaw = createModel<Seesaw, SeesawWidget>("seesaw");