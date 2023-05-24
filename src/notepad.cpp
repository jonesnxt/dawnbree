#include "plugin.hpp"


struct Notepad : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		MAIN_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		ROOT_OUTPUT,
		MINOR_SECOND_OUTPUT,
		MAJOR_SECOND_OUTPUT,
		MINOR_THIRD_OUTPUT,
		MAJOR_THIRD_OUTPUT,
		PERFECT_FOURTH_OUTPUT,
		TRITONE_OUTPUT,
		PERFECT_FIFTH_OUTPUT,
		MINOR_SIXTH_OUTPUT,
		MAJOR_SIXTH_OUTPUT,
		MINOR_SEVENTH_OUTPUT,
		MAJOR_SEVENTH_OUTPUT,
		OCTAVE_OUTPUT,
		MINOR_NINTH_OUTPUT,
		MAJOR_NINTH_OUTPUT,
		MINOR_TENTH_OUTPUT,
		MAJOR_TENTH_OUTPUT,
		PERFECT_ELEVENTH_OUTPUT,
		OCTAVE_TRITONE_OUTPUT,
		PERFECT_TWELFTH_OUTPUT,
		MINOR_THIRTEENTH_OUTPUT,
		MAJOR_THIRTEENTH_OUTPUT,
		MINOR_FOURTEENTH_OUTPUT,
		MAJOR_FOURTEENTH_OUTPUT,
		DOUBLE_OCTAVE_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	Notepad() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(MAIN_INPUT, "main");
		configOutput(ROOT_OUTPUT, "root");
		configOutput(MINOR_SECOND_OUTPUT, "minor second");
		configOutput(MAJOR_SECOND_OUTPUT, "major second");
		configOutput(MINOR_THIRD_OUTPUT, "minor third");
		configOutput(MAJOR_THIRD_OUTPUT, "major third");
		configOutput(PERFECT_FOURTH_OUTPUT, "perfect fourth");
		configOutput(TRITONE_OUTPUT, "tritone");
		configOutput(PERFECT_FIFTH_OUTPUT, "perfect fifth");
		configOutput(MINOR_SIXTH_OUTPUT, "minor sixth");
		configOutput(MAJOR_SIXTH_OUTPUT, "major sixth");
		configOutput(MINOR_SEVENTH_OUTPUT, "minor seventh");
		configOutput(MAJOR_SEVENTH_OUTPUT, "major seventh");
		configOutput(OCTAVE_OUTPUT, "octave");
		configOutput(MINOR_NINTH_OUTPUT, "minor ninth");
		configOutput(MAJOR_NINTH_OUTPUT, "major ninth");
		configOutput(MINOR_TENTH_OUTPUT, "minor tenth");
		configOutput(MAJOR_TENTH_OUTPUT, "major tenth");
		configOutput(PERFECT_ELEVENTH_OUTPUT, "perfect eleventh");
		configOutput(OCTAVE_TRITONE_OUTPUT, "octave tritone");
		configOutput(PERFECT_TWELFTH_OUTPUT, "perfect twelfth");
		configOutput(MINOR_THIRTEENTH_OUTPUT, "minor thirteenth");
		configOutput(MAJOR_THIRTEENTH_OUTPUT, "major thirteenth");
		configOutput(MINOR_FOURTEENTH_OUTPUT, "minor fourteenth");
		configOutput(MAJOR_FOURTEENTH_OUTPUT, "major fourteenth");
		configOutput(DOUBLE_OCTAVE_OUTPUT, "double octave");
	}

	float semitone = 1.f / 12;

	void process(const ProcessArgs& args) override {

		float input = inputs[MAIN_INPUT].getVoltage();

		for(int i = 0; i < OUTPUTS_LEN; i++) {
			outputs[i].setVoltage(input + (semitone * i));
		}
	}
};


struct NotepadWidget : ModuleWidget {
	NotepadWidget(Notepad* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/notepad.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.315, 20.71)), module, Notepad::MAIN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(68.697, 20.71)), module, Notepad::ROOT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.944, 44.538)), module, Notepad::MINOR_SECOND_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.178, 44.538)), module, Notepad::MAJOR_SECOND_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.412, 44.538)), module, Notepad::MINOR_THIRD_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.646, 44.538)), module, Notepad::MAJOR_THIRD_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(59.736, 44.538)), module, Notepad::PERFECT_FOURTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(73.115, 44.538)), module, Notepad::TRITONE_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.944, 59.318)), module, Notepad::PERFECT_FIFTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.178, 59.318)), module, Notepad::MINOR_SIXTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.412, 59.318)), module, Notepad::MAJOR_SIXTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.646, 59.318)), module, Notepad::MINOR_SEVENTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(59.88, 59.318)), module, Notepad::MAJOR_SEVENTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(73.115, 59.318)), module, Notepad::OCTAVE_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.944, 85.698)), module, Notepad::MINOR_NINTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.178, 85.698)), module, Notepad::MAJOR_NINTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.412, 85.698)), module, Notepad::MINOR_TENTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.646, 85.698)), module, Notepad::MAJOR_TENTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(59.736, 85.698)), module, Notepad::PERFECT_ELEVENTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(73.115, 85.698)), module, Notepad::OCTAVE_TRITONE_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.944, 100.478)), module, Notepad::PERFECT_TWELFTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20.178, 100.478)), module, Notepad::MINOR_THIRTEENTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.412, 100.478)), module, Notepad::MAJOR_THIRTEENTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.646, 100.478)), module, Notepad::MINOR_FOURTEENTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(59.88, 100.478)), module, Notepad::MAJOR_FOURTEENTH_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(73.115, 100.478)), module, Notepad::DOUBLE_OCTAVE_OUTPUT));
	}
};


Model* modelNotepad = createModel<Notepad, NotepadWidget>("notepad");