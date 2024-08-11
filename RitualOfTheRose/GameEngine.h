#pragma once

#include "Graphics.h"

class GameEngine {
public:
	class Event {
	public:
		Event(string _name, string _type, Map<string, string> _data) {
			name = _name;
			type = _type;
			data = _data;
		}
		bool run() {
			if (type == "LOADIMAGE") {
				List<string> sources = split(data["sources"], " ");
				List<int> intSources;
				if (sources.size() == 0) {
					ErrorHelper::warning("Can't have an image with no sources.", true);
				}
				for (int x = 0; x < sources.size(); x++) {
					intSources.push_back(stoi(sources.at(x)));
				}
				pair<int, int> position = { stoi(data["x"]), stoi(data["y"]) };
				string anchor = data["anchor"];
				float opacity = stof(data["opacity"]);
				int layer = stoi(data["layer"]);
				string uniqueID = data["uniqueID"];
				graphics.addImage(new Graphics::Image(intSources, position, anchor, opacity, uniqueID), layer);
				return true;
			}
			if (type == "WAIT") {
				string clock_id = data["clockID"];
				CLOCK.startClockIfItDoesNotExist(clock_id);
				return CLOCK.hasEnoughTimePassed(data["clockID"], stoi(data["waitDuration"]));
			}
			if (type == "ANIMATEIMAGE") {
				string imageHandle = data["uniqueID"];
				List<string> styles = split(data["styles"], ",");
				Graphics::Image* theImage = graphics.accessImageViaUniqueID(imageHandle);
				int animationSpeed = stoi(data["speed"]);
				if (!theImage->animated) {
					theImage->animated = true;
					theImage->animationStyles = styles;
					theImage->animationSpeed = animationSpeed;
				}
				bool wait = data["wait"] == "TRUE";
				if (wait and styles.contains("FADEOUT") and theImage->opacity != 0.0f) {
					return false;
				}
				return true;
			}
			if (type == "DRAWTEXT") {
				List<string> mp = split(data["message"], " ");
				wstring message = wstring(data["message"].begin(), data["message"].end());
				if (!data.getKeys().contains("direct")) {
					message = strings[mp.at(0)][mp.at(1)][mp.at(2)];
				}
				string format = data["format"];
				pair<int, int> position = { stoi(data["x"]), stoi(data["y"]) };
				string anchorStyle = data["anchorStyle"];
				pair<int, int> size = { stoi(data["w"]), stoi(data["h"]) };
				vector<float> colour = graphics.Colours[data["colour"]];
				vector<float> shadowColour = graphics.Colours[data["shadowColour"]];
				int layer = stoi(data["layer"]);
				string uniqueID = data["uniqueID"];
				List<string> styles = split(data["styles"], ",");
				bool animated = data["animated"] == "TRUE";
				if (!graphics.does_this_text_already_exist(uniqueID)) {
					graphics.addText(Graphics::Text(message, format, position, anchorStyle, size, colour, shadowColour, uniqueID), layer);
					if (animated) {
						if (styles.contains("TYPEWRITER")) {
							graphics.TextMap[layer].internalMap[uniqueID].startTypewriter();
						}
					}
				}
				else {
					if (animated and styles.contains("TYPEWRITER")) {
						int typewriterSpeed = 5;
						wstring full = graphics.TextMap[layer].internalMap[uniqueID].fullMessage;
						bool finished = graphics.TextMap[layer].internalMap[uniqueID].message == full;
						int howFarAlong = graphics.TextMap[layer].internalMap[uniqueID].howFarAlong();
						if (CLOCK.hasEnoughTimePassed("TYPEWRITER", typewriterSpeed)) {
							if (finished) {
								return true;
							}
							else {
								int size = graphics.TextMap[layer].internalMap[uniqueID].message.size();
								graphics.TextMap[layer].internalMap[uniqueID].message += full[size];
								if (styles.contains("PARCHMENT") and howFarAlong < 50) {
									audio.playRandomSFXFromThisCollection("PARCHMENT", 0.5f);
								}
							}
						}
					}
				}
				if (!animated) {
					return true;
				}
				
				return false;
			}
			if (type == "TEARDOWNIMAGE") {
				graphics.tearDownSpecifiedImage(data["uniqueID"]);
				return true;
			}
			if (type == "TEARDOWNTEXT") {
				graphics.tearDownSpecifiedText(data["uniqueID"]);
				return true;
			}
			if (type == "DEBUGUSERINPUT") {
				string uniqueID = "DEBUGTEXT";
				int layer = 10;
				if (!graphics.does_this_text_already_exist(uniqueID)) {
					graphics.addText(Graphics::Text(L"", "Centaur_25", { 50,50 }, "CENTER", { 100,100 }, graphics.Colours["WHITE"], graphics.Colours["BLACK"], uniqueID), layer);
				}
				graphics.TextMap[layer].internalMap[uniqueID].message = StringToWString(controller.controllerDebug());
				if (controller.hasThisBeenPressed(27)) {
					return true;
				}
				return false;
			}
			if (type == "DEBUGCLICKANDDRAG") {
				List<string> draggable = split(data["objects"], ",");
				if (controller.mouseInstructionsInOrder.contains("LButtonDown")) {
					graphics.beingDragged.clear();
					pair<int, int> click = controller.mouseClickPosition;
					for (auto const& ID : draggable.internalList) {
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(ID);
						if (theImage->hasThisBeenClickedOn(*&graphics, click)) {
							graphics.beingDragged.push_back(theImage);
							}
						}
					}
				if (controller.mouseInstructionsInOrder.contains("MouseMove")) {
					pair<int, int> move = controller.mouseMovePosition;
					bool hoveringOverSomethingDraggable = false;
					for (auto const& ID : draggable.internalList) {
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(ID);
						if (theImage->hasThisBeenClickedOn(*&graphics, move)) {
							hoveringOverSomethingDraggable = true;
							graphics.changeCursor("SELECTED");
						}
					}
					for (auto const& x : graphics.beingDragged.internalList) {
						pair<int, int> mouseMoveAsPercent = x->convertActualToPercent(graphics.hwndRenderTarget->GetSize(), move);
						x->positionAsPercentage = mouseMoveAsPercent;
					}
					if (!hoveringOverSomethingDraggable) {
						graphics.changeCursor("DEFAULT");
					}
				}
				if (controller.mouseInstructionsInOrder.contains("LButtonUp")) {
					// call upon drag and drop logic depending on the event
					graphics.beingDragged.clear();
				}


				
				return false;
			};
			if (type == "DEBUGLOAD") {
				filesystem::path defaultSavePath = filesystem::current_path() / "slot_0";
				filesystem::path debugSavePath = filesystem::current_path() / "slot_-1";
				SaveContainer::SaveFile save(defaultSavePath);
				save.saveToDisk(debugSavePath);
				saveContainer.load(debugSavePath);
				return true;
			}
			if (type == "DEBUGEXPLORE") {
				bool moving = false;
				bool need_to_reset_image_sources = true;
				string direction = "";
				string action = "STAND";
				map<string, int> exploreAnimationSpeeds = { {"WALK",200} , {"STAND" ,500 }};
				int animationSpeed = 500;
				if (controller.userPressedOneOfThese({ VK_UP, 0x57 })) {
					moving = true;
					direction = "BACK";
					action = "WALK";
				}
				if (controller.userPressedOneOfThese({ VK_LEFT, 0x41 })) {
					moving = true;
					direction = "LEFT";
					action = "WALK";
				}
				if (controller.userPressedOneOfThese({ VK_RIGHT, 0x44 })) {
					moving = true;
					direction = "RIGHT";
					action = "WALK";
				}
				if (controller.userPressedOneOfThese({ VK_DOWN, 0x53 })) {
					moving = true;
					direction = "FRONT";
					action = "WALK";
				}
				string character = saveContainer.getCurrentMainCharacter();
				string uniqueID = split(character, " ").front() + "_Explore";
				Graphics::Image* image = graphics.accessImageViaUniqueID(uniqueID);
				if (direction != "") {
					image->direction = direction;
				}
				else {
					direction = image->direction;
				}
				List<int> sources;
				if (!moving and image->action == "STAND") { // continue standing
					need_to_reset_image_sources = false;
				}
				if (moving and image->action == "WALK") { // continue walking
					need_to_reset_image_sources = false;
				}
				if (need_to_reset_image_sources) {
					if (action == "WALK") {
						animationSpeed = 200;
					}
					sources = imageLookup.animationFrames[character][action + "_" + direction];
					image->resetSources(*&graphics, sources);
					image->action = action;
				}
				if (moving and CLOCK.hasEnoughTimePassed("EXPLORE",exploreAnimationSpeeds["WALK"])) {
					
				}



				return false;
			}
}
		string name;
		string type;
		Map<string, string> data;
	};
	class Procedure {
	public:
		Procedure() {}
		Procedure(string _name, List<Event> _eventList) {
			name = _name;
			eventList = _eventList;
		}
		bool run() {
			while (!eventList.empty() and eventList.front().run()) {
				eventList.pop_front();
			}
			return false;
		}
		string name = "None";
		List<Event> eventList;
	};
	Map<string, Procedure> storedProcedures = List<pair<string, Procedure>>({
		pair<string, Procedure>({"DEBUG1" ,
		Procedure("Load Debug", List<Event>({
			Event("Load Background", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(MAIN_MENU_BACKGROUND)),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "50"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "0"),
				pair<string, string>("uniqueID", "debugmenubackground"),
			}))),
			Event("Load Logo", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(IDB_PNG1)),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "40"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "1"),
				pair<string, string>("uniqueID", "debugmenulogo"),}))),
			Event("Wait", "WAIT", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("clockID", "DEBUGCLOCK1"),
				pair<string, string>("waitDuration", "1000"),}))),
			Event("FadeoutLogo", "ANIMATEIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("uniqueID", "debugmenulogo"),
				pair<string, string>("styles", "FADEOUT"),
				pair<string, string>("speed", "500"),
				pair<string, string>("wait", "TRUE"),}))),
			Event("TeardownImage", "TEARDOWNIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("uniqueID", "debugmenulogo"),}))),
			Event("Load TextBox", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(TEXTBOX)),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "86"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "1"),
				pair<string, string>("uniqueID", "TEXTBOX"),}))),
			Event("Load Speaker", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(DEBUGSPEAKER)),
				pair<string, string>("x", "0"),
				pair<string, string>("y", "65"),
				pair<string, string>("anchor", "TOPLEFT"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "2"),
				pair<string, string>("uniqueID", "SPEAKER"),}))),
			Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("message","ENG NAMES Angela"),
				pair<string, string>("format", "LightText_40"),
				pair<string, string>("anchorStyle", "TOPLEFT"),
				pair<string, string>("x", "16"),
				pair<string, string>("y", "72"),
				pair<string, string>("w", "50"),
				pair<string, string>("h", "50"),
				pair<string, string>("colour", "BLACK"),
				pair<string, string>("shadowColour", "DARKBROWN"),
				pair<string, string>("layer", "15"),
				pair<string, string>("uniqueID", "debugText2"),
				}))),
			Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("message","ENG DEBUG TestString"),
				pair<string, string>("format", "Centaur_25"),
				pair<string, string>("anchorStyle", "TOPLEFT"),
				pair<string, string>("x", "20"),
				pair<string, string>("y", "79"),
				pair<string, string>("w", "78"),
				pair<string, string>("h", "33"),
				pair<string, string>("colour", "BLACK"),
				pair<string, string>("shadowColour", "DARKBROWN"),
				pair<string, string>("layer", "15"),
				pair<string, string>("uniqueID", "debugText1"),
				pair<string, string>("animated", "TRUE"),
				pair<string, string>("styles", "TYPEWRITER,PARCHMENT"),
				}))),
			Event("Wait", "WAIT", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("clockID", "DEBUGCLOCK2"),
				pair<string, string>("waitDuration", "5000"),}))),
			}))}),
		pair<string, Procedure>({"DEBUG2",
		Procedure("UserInput", List<Event>({
			Event("Debug User Input", "DEBUGUSERINPUT", Map<string, string>(List<pair<string,string>>({}))),
			Event("TeardownUserInput", "TEARDOWNTEXT", Map<string, string>(List<pair<string,string>>(
				{pair<string, string>({"uniqueID", "DEBUGTEXT"})}))),
			Event("Load Skill Icon", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(SKILLICON_HEALWOUNDS)),
				pair<string, string>("x", "25"),
				pair<string, string>("y", "25"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "0"),
				pair<string, string>("uniqueID", "healwoundsicon"),
			}))),
			Event("Load Skill Border", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(NORMALSKILLBORDER)),
				pair<string, string>("x", "25"),
				pair<string, string>("y", "25"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "1"),
				pair<string, string>("uniqueID", "normalskillborder"),
			}))),
			Event("Load Skill Border", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(SKILLBAR)),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "75"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "1"),
				pair<string, string>("uniqueID", "skillbar"),
			}))),
			Event("DebugClickAndDrag", "DEBUGCLICKANDDRAG", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("objects", "healwoundsicon,normalskillborder"),
			}))),
			}))
			}),
			pair<string, Procedure>({ "DEBUG3",
			Procedure("SaveAndLoad", List<Event>({
			Event("Debug Loading", "DEBUGLOAD", Map<string,string>(List<pair<string,string>>({}))),
			Event("Load Map", "LOADIMAGE", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(MAP_DEBUG)),
				pair<string, string>("x", "0"),
				pair<string, string>("y", "0"),
				pair<string, string>("anchor", "TOPLEFT"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "0"),
				pair<string, string>("uniqueID", "debugmap"),
			}))),
			Event("Load Angela", "LOADIMAGE", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("sources", imageLookup.getSequenceAsString("Angela Fleuret", "STAND_FRONT")),
				pair<string, string>("x", "0"),
				pair<string, string>("y", "0"),
				pair<string, string>("anchor", "TOPLEFT"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "2"),
				pair<string, string>("uniqueID", "Angela_Explore"),
			}))),
			Event("Load Angela", "LOADIMAGE", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(ANGELA_SHADOW)),
				pair<string, string>("x", "0"),
				pair<string, string>("y", "0"),
				pair<string, string>("anchor", "TOPLEFT"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "1"),
				pair<string, string>("uniqueID", "Angela_Shadow"),
			}))),
			Event("Animate Angela", "ANIMATEIMAGE", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("uniqueID", "Angela_Explore"),
				pair<string, string>("speed", "200"),
				pair<string, string>("styles", "LOOP"),
			}))),
			Event("Debug Exploring", "DEBUGEXPLORE", Map<string,string>(List<pair<string,string>>({}))),
			})) }),

		});

	GameEngine() {}
	void setup() {
		string mode = Args.get("mode");
		if (mode == "DEBUG") {
			activeProcedure = storedProcedures["DEBUG3"];
		}
		else {
			ErrorHelper::warning("Not Implemented yet" , true);
		}
		CLOCK.startClock("FPS");
		stateFlags["QUIT"] = "0";
	}
	bool quit() {
		return stateFlags["QUIT"] == "1";
	}
	void run() {
		activeProcedure.run();
	}

		Procedure activeProcedure;
		Map<string, string> stateFlags;
};
GameEngine game;