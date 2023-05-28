#include "plugin.hpp"


struct Prismo : Module {
	enum ParamId {
		CLOCK_PARAM,
		RESET_PARAM,
		STEPS_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		CLOCK_INPUT,
		RESET_INPUT,
		STEPS_INPUT,
		A1_INPUT,
		A2_INPUT,
		A3_INPUT,
		A4_INPUT,
		A5_INPUT,
		A6_INPUT,
		A7_INPUT,
		A8_INPUT,
		B1_INPUT,
		B2_INPUT,
		B3_INPUT,
		B4_INPUT,
		B5_INPUT,
		B6_INPUT,
		B7_INPUT,
		B8_INPUT,
		C1_INPUT,
		C2_INPUT,
		C3_INPUT,
		C4_INPUT,
		C5_INPUT,
		C6_INPUT,
		C7_INPUT,
		C8_INPUT,
		D1_INPUT,
		D2_INPUT,
		D3_INPUT,
		D4_INPUT,
		D5_INPUT,
		D6_INPUT,
		D7_INPUT,
		D8_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		A_OUTPUT,
		B_OUTPUT,
		C_OUTPUT,
		D_OUTPUT,
		ABCD_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		ONE_LIGHT,
		TWO_LIGHT,
		THREE_LIGHT,
		FOUR_LIGHT,
		FIVE_LIGHT,
		SIX_LIGHT,
		SEVEN_LIGHT,
		EIGHT_LIGHT,
		LIGHTS_LEN
	};

	dsp::SchmittTrigger clockTrigger;
	dsp::SchmittTrigger resetTrigger;
	int currentStep = 0;

	Prismo() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(CLOCK_PARAM, 0, 1, 0, "clock");
		configParam(RESET_PARAM, 0, 1, 0, "reset");
		configParam(STEPS_PARAM, 1, 8, 8, "steps");
		getParamQuantity(STEPS_PARAM)->snapEnabled = true;
		configInput(CLOCK_INPUT, "clock");
		configInput(RESET_INPUT, "reset");
		configInput(STEPS_INPUT, "steps");
		configInput(A1_INPUT, "a one");
		configInput(A2_INPUT, "a two");
		configInput(A3_INPUT, "a three");
		configInput(A4_INPUT, "a four");
		configInput(A5_INPUT, "a five");
		configInput(A6_INPUT, "a six");
		configInput(A7_INPUT, "a seven");
		configInput(A8_INPUT, "a eight");
		configInput(B1_INPUT, "b one");
		configInput(B2_INPUT, "b two");
		configInput(B3_INPUT, "b three");
		configInput(B4_INPUT, "b four");
		configInput(B5_INPUT, "b five");
		configInput(B6_INPUT, "b six");
		configInput(B7_INPUT, "b seven");
		configInput(B8_INPUT, "b eight");
		configInput(C1_INPUT, "c one");
		configInput(C2_INPUT, "c two");
		configInput(C3_INPUT, "c three");
		configInput(C4_INPUT, "c four");
		configInput(C5_INPUT, "c five");
		configInput(C6_INPUT, "c six");
		configInput(C7_INPUT, "c seven");
		configInput(C8_INPUT, "c eight");
		configInput(D1_INPUT, "d one");
		configInput(D2_INPUT, "d two");
		configInput(D3_INPUT, "d three");
		configInput(D4_INPUT, "d four");
		configInput(D5_INPUT, "d five");
		configInput(D6_INPUT, "d six");
		configInput(D7_INPUT, "d seven");
		configInput(D8_INPUT, "d eight");
		configOutput(A_OUTPUT, "a");
		configOutput(B_OUTPUT, "b");
		configOutput(C_OUTPUT, "c");
		configOutput(D_OUTPUT, "d");
		configOutput(ABCD_OUTPUT, "combined");
		
		lights[ONE_LIGHT].setBrightness(1);
	}

	void process(const ProcessArgs& args) override {
		bool shouldReset = resetTrigger.process(inputs[RESET_INPUT].getVoltage() + params[RESET_PARAM].getValue());
		bool shouldClock = clockTrigger.process(inputs[CLOCK_INPUT].getVoltage() + params[CLOCK_PARAM].getValue());

		if(shouldReset) {
			currentStep = 0;
			for(int i = 1; i < 8; i++) {
				lights[ONE_LIGHT + i].setBrightness(0);
			}
			lights[ONE_LIGHT].setBrightness(1);
		}	else if(shouldClock) {
			int steps = inputs[STEPS_INPUT].isConnected() ? clamp((int) floor(inputs[STEPS_INPUT].getVoltage()), (int) 1, (int) 8) : params[STEPS_PARAM].getValue();

			currentStep = (currentStep + 1) % steps;

			for(int i = 0; i < 8; i++) {
				lights[ONE_LIGHT + i].setBrightness(0);
			}
			lights[ONE_LIGHT + currentStep].setBrightness(1);
		}

		// set all of the correct outputs
		outputs[A_OUTPUT].setVoltage(inputs[A1_INPUT + currentStep].getVoltage());
		outputs[B_OUTPUT].setVoltage(inputs[B1_INPUT + currentStep].getVoltage());
		outputs[C_OUTPUT].setVoltage(inputs[C1_INPUT + currentStep].getVoltage());
		outputs[D_OUTPUT].setVoltage(inputs[D1_INPUT + currentStep].getVoltage());

		outputs[ABCD_OUTPUT].setChannels(4);
		outputs[ABCD_OUTPUT].setVoltage(inputs[A1_INPUT + currentStep].getVoltage(), 0);
		outputs[ABCD_OUTPUT].setVoltage(inputs[B1_INPUT + currentStep].getVoltage(), 1);
		outputs[ABCD_OUTPUT].setVoltage(inputs[C1_INPUT + currentStep].getVoltage(), 2);
		outputs[ABCD_OUTPUT].setVoltage(inputs[D1_INPUT + currentStep].getVoltage(), 3);
	}
};


struct PrismoWidget : ModuleWidget {
	PrismoWidget(Prismo* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/prismo.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<LEDButton>(mm2px(Vec(10.299, 11.602)), module, Prismo::CLOCK_PARAM));
		addParam(createParamCentered<LEDButton>(mm2px(Vec(27.186, 11.602)), module, Prismo::RESET_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(104.1, 11.602)), module, Prismo::STEPS_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.299, 27.589)), module, Prismo::CLOCK_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.186, 27.589)), module, Prismo::RESET_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(104.1, 27.589)), module, Prismo::STEPS_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.759, 45.16)), module, Prismo::A1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(34.379, 45.16)), module, Prismo::A2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(47.0, 45.16)), module, Prismo::A3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(59.62, 45.16)), module, Prismo::A4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(72.227, 45.16)), module, Prismo::A5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(84.422, 45.16)), module, Prismo::A6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(97.482, 45.16)), module, Prismo::A7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(110.103, 45.16)), module, Prismo::A8_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.759, 56.011)), module, Prismo::B1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(34.379, 56.011)), module, Prismo::B2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(47.0, 56.011)), module, Prismo::B3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(59.62, 56.011)), module, Prismo::B4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(72.227, 56.011)), module, Prismo::B5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(84.422, 56.011)), module, Prismo::B6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(97.482, 56.011)), module, Prismo::B7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(110.103, 56.011)), module, Prismo::B8_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.759, 66.862)), module, Prismo::C1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(34.379, 66.862)), module, Prismo::C2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(47.0, 66.862)), module, Prismo::C3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(59.62, 66.862)), module, Prismo::C4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(72.227, 66.862)), module, Prismo::C5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(84.422, 66.862)), module, Prismo::C6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(97.482, 66.862)), module, Prismo::C7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(110.103, 66.862)), module, Prismo::C8_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.759, 77.714)), module, Prismo::D1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(34.379, 77.714)), module, Prismo::D2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(47.0, 77.714)), module, Prismo::D3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(59.62, 77.714)), module, Prismo::D4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(72.227, 77.714)), module, Prismo::D5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(84.422, 77.714)), module, Prismo::D6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(97.482, 77.714)), module, Prismo::D7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(110.103, 77.714)), module, Prismo::D8_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(11.22, 103.509)), module, Prismo::A_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(23.752, 103.509)), module, Prismo::B_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(36.283, 103.509)), module, Prismo::C_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(48.815, 103.509)), module, Prismo::D_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(91.594, 103.509)), module, Prismo::ABCD_OUTPUT));

		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(21.759, 84.628)), module, Prismo::ONE_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(34.379, 84.628)), module, Prismo::TWO_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(47.0, 84.628)), module, Prismo::THREE_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(59.62, 84.628)), module, Prismo::FOUR_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(72.227, 84.628)), module, Prismo::FIVE_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(84.862, 84.628)), module, Prismo::SIX_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(97.482, 84.628)), module, Prismo::SEVEN_LIGHT));
		addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(110.103, 84.628)), module, Prismo::EIGHT_LIGHT));
	}
};


Model* modelPrismo = createModel<Prismo, PrismoWidget>("prismo");