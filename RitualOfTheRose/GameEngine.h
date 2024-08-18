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
		bool run(GameEngine& gameEngine) {
			if (type == "LOADIMAGE") {
				List<string> sources = split(data["sources"], " ");
				List<int> intSources;
				if (sources.size() == 0) {
					ErrorHelper::warning("Can't have an image with no sources.", false);
				}
				for (int x = 0; x < sources.size(); x++) {
					intSources.push_back(stoi(sources.at(x)));
				}
				pair<int, int> position = { stoi(data["x"]), stoi(data["y"]) };
				string anchor = data["anchor"];
				float opacity = stof(data["opacity"]);
				int layer = stoi(data["layer"]);
				string uniqueID = data["uniqueID"];
				Graphics::Image * image = graphics.addImage(new Graphics::Image(intSources, position, anchor, opacity, uniqueID), layer);
				bool animated = data.getKeys().contains("animated");
				int animationSpeed = 0;
				if (animated) {
					animationSpeed = stoi(data["animation_speed"]);
					image->animated = true;
					image->animationSpeed = animationSpeed;
					image->animationStyles = data["styles"];
				}
				if (data.getKeys().contains("scale")) {
					image->scale = stof(data["scale"]); // make maps x2 the size by default
				}
				return true;
			}
			if (type == "LOADMAP") {
				string mapName = data["uniqueID"];
				string explorerName = saveContainer.getCurrentMainCharacter();
				explorer.loadMap(mapName);
				Event("Load" + explorerName, "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", to_string(explorer.currentMap.source)),
					pair<string, string>("x", "50"),
					pair<string, string>("y", "50"),
					pair<string, string>("anchor", "CENTRE"),
					pair<string, string>("opacity", "1.0"),
					pair<string, string>("layer", "0"),
					pair<string, string>("scale", "2.0"),
					pair<string, string>("uniqueID", mapName), }))).run(*&gameEngine);
				Event("Load" + explorerName, "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", imageLookup.getSequenceAsString(explorerName, "STAND_FRONT")),
					pair<string, string>("x", "50"),
					pair<string, string>("y", "50"),
					pair<string, string>("anchor", "BOTTOMMIDDLE"),
					pair<string, string>("opacity", "1.0"),
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["PLAYER"])),
					pair<string, string>("animated", "1"),
					pair<string, string>("animation_speed", "500"),
					pair<string, string>("styles", "LOOP"),
					pair<string, string>("uniqueID", explorerName + "_Explore"), }))).run(*&gameEngine);
				Event("Load Shadow", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", imageLookup.getSequenceAsString("Shadow " + explorerName, "STAND_FRONT")),
					pair<string, string>("x", "50"),
					pair<string, string>("y", "50"),
					pair<string, string>("anchor", "BOTTOMMIDDLE"),
					pair<string, string>("opacity", "0.5"),
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["PLAYERSHADOW"])),
					pair<string, string>("animated", "1"),
					pair<string, string>("animation_speed", "500"),
					pair<string, string>("styles", "LOOP"),
					pair<string, string>("uniqueID", explorerName + "_Shadow")}))).run(*&gameEngine);
				for (auto const& x : explorer.currentMap.objects.internalList) {
					if (x.imageSources == "") { continue; }
					Event("Load" + x.name, "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", x.imageSources),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", x.opacity),
						pair<string, string>("layer", to_string(x.layer)),
						pair<string, string>("animated", x.animated),
						pair<string, string>("animation_speed", to_string(x.animationSpeed)),
						pair<string, string>("styles", "LOOP"),
						pair<string, string>("uniqueID", x.name), }))).run(*&gameEngine);
				}
				Event("", "MAPMOVE", {}).run(*&gameEngine);
				return true;
			}
			if (type == "MAPMOVE") {
				string explorerName = saveContainer.getCurrentMainCharacter() + "_Explore";
				string shadowName = saveContainer.getCurrentMainCharacter() + "_Shadow";
				map<string, pair<float, float>> imagePositions = explorer.getUpdatedMapImagePositions();
				Graphics::Image* mapImage = graphics.accessImageViaUniqueID(explorer.currentMap.name);
				Graphics::Image* image = graphics.accessImageViaUniqueID(explorerName);
				Graphics::Image* shadowImage = graphics.accessImageViaUniqueID(shadowName);
				mapImage->positionAsPercentage = imagePositions["map position"];
				image->positionAsPercentage = imagePositions["player image position"];
				shadowImage->positionAsPercentage = imagePositions["player image position"];
				for (auto const& x : explorer.currentMap.objects.internalList) {
					if (!x.visible) { continue; }
					Graphics::Image* objectImage = graphics.accessImageViaUniqueID(x.name);
					objectImage->positionAsPercentage = imagePositions[x.name];
				}
				if (graphics.does_this_text_already_exist(explorer.mapPopupTextID) and data.getKeys().contains("copy")) {
					int layer = imageLookup.layerDefaults["TEXTONMAP"];
					graphics.TextMap[layer][explorer.mapPopupTextID].positionAsPercentage = imagePositions[data["copy"]];
				}
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
				if (data["direct"] != "1") {
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
					graphics.addText(Graphics::Text(L"", "Centaur_25", { 50,50 }, "CENTRE", { 100,100 }, graphics.Colours["WHITE"], graphics.Colours["BLACK"], uniqueID), layer);
				}
				graphics.TextMap[layer].internalMap[uniqueID].message = StringToWString(controller.controllerDebug());
				if (controller.hasThisBeenPressed(27)) {
					return true;
				}
				return false;
			}
			if (type == "DEBUGCLICKANDDRAG") {
				string uniqueID = "DEBUGCLICKANDDRAG";
				int layer = 10;
				if (!graphics.does_this_text_already_exist(uniqueID)) {
					graphics.addText(Graphics::Text(L"", "Centaur_25", { 10,90 }, "TOPLEFT", { 100,100 }, graphics.Colours["BLACK"], graphics.Colours["BLACK"], uniqueID), layer);
				}
				string clickAndDragMessage = "Images Hovered Over: ";
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
							clickAndDragMessage += theImage->unique_ID + " ";
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

				graphics.TextMap[layer].internalMap[uniqueID].message = StringToWString(clickAndDragMessage);
				Event("", "DEBUGUSERINPUT", {}).run(*&gameEngine);
				return false;
			};
			if (type == "DEBUGWALKING") {
				string textUniqueID = "DEBUGWALKING";
				int layer = 10;
				if (!graphics.does_this_text_already_exist(textUniqueID)) {
					graphics.addText(Graphics::Text(L"", "Centaur_25", { 2,80 }, "TOPLEFT", { 100,100 }, graphics.Colours["WHITE"], graphics.Colours["BLACK"], textUniqueID), layer);
				}
				graphics.TextMap[layer].internalMap[textUniqueID].message = StringToWString(explorer.debug());
				return false;
			}
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
				string newDirection = "";
				string newAction = "STAND";
				map<string, int> exploreAnimationSpeeds = { {"WALK",200} , {"STAND" ,500 }, {"MOVE", explorer.mapSize.first / 50} };
				if (Args.get("mode") == "DEBUG") {
					if (controller.hasThisBeenPressed(VK_F1)) {
						Args.toggle("SPEEDCHEAT");
					}
					if (Args.get("SPEEDCHEAT") == "1") {
						exploreAnimationSpeeds["MOVE"] = 10;
					}
				}
				int animationSpeed = 500;

				string character = saveContainer.getCurrentMainCharacter();
				string uniqueID = character + "_Explore";
				string shadowID = character + "_Shadow";
				Graphics::Image* image = graphics.accessImageViaUniqueID(uniqueID);
				Graphics::Image* shadowImage = graphics.accessImageViaUniqueID(shadowID);
				string currentDirection = image->direction;
				string currentAction = image->action;

				for (auto const& x : controller.keysPressedInOrderAsInts.internalList) {
					if (controller.up.contains(x) or controller.down.contains(x) or controller.left.contains(x) or controller.right.contains(x)) {
						moving = true;
						newAction = "WALK";
					}
					if (controller.up.contains(x)) {
						newDirection = "BACK";
						break;
					}
					if (controller.down.contains(x)) {
						newDirection = "FRONT";
						break;
					}
					if (controller.left.contains(x)) {
						newDirection = "LEFT";
						break;
					}
					if (controller.right.contains(x)) {
						newDirection = "RIGHT";
						break;
					}
				}

				if (currentDirection == newDirection and currentAction == newAction) {
					need_to_reset_image_sources = false;
				}
				else {
					if (newDirection == "") {
						newDirection = currentDirection;
					}
					image->direction = newDirection;
					shadowImage->direction = newDirection;
					image->action = newAction;
					shadowImage->action = newAction;
				}
				List<int> sources;
				List<int> shadowSources;

				if (need_to_reset_image_sources) {
					if (newAction == "WALK") {
						animationSpeed = exploreAnimationSpeeds["WALK"];
					}
					if (newAction == "STAND") {
						animationSpeed = exploreAnimationSpeeds["STAND"];
					}
					sources = imageLookup.animationFrames[character][newAction + "_" + newDirection];
					shadowSources = imageLookup.animationFrames["Shadow " + character][newAction + "_" + newDirection];
					image->resetSources(*&graphics, sources);
					shadowImage->resetSources(*&graphics, shadowSources);
					for (auto const& x : { image, shadowImage }) {
						x->action = newAction;
						x->animationSpeed = animationSpeed;
					}
				}
				bool stopExploringStartCutscene = false;
				bool popUpTextNeedsToBeDrawn = false;
				bool mapPopUpTextExists = false;
				string whichCutscene = "";
				if (moving and CLOCK.hasEnoughTimePassed("EXPLORE",exploreAnimationSpeeds["MOVE"])) {
					explorer.tryToMovePlayer(newDirection);
					Event("Map Move", "MAPMOVE", {}).run(*&gameEngine);
					image->opacity = 1.0;
					shadowImage->opacity = 0.5;
					mapPopUpTextExists = graphics.does_this_text_already_exist(explorer.mapPopupTextID);
					popUpTextNeedsToBeDrawn = false;
					for (auto walkable : explorer.getCurrentlySteppedOn().internalList) {
						if (walkable.data["trans"] == "1") {
							image->opacity = 0.5;
							shadowImage->opacity = 0;
						}
					}
					for (auto walkable : explorer.getObjectsInRange().internalList) {
						if (walkable.data.getKeys().contains("message")) {
							popUpTextNeedsToBeDrawn = true;
							if (!mapPopUpTextExists) {
								Event("PopUpTextOnMap", "DRAWTEXT", walkable.data).run(*&gameEngine);
								mapPopUpTextExists = true;
							}
							Event("Map Move", "MAPMOVE", { pair<string, string>("copy",walkable.data["copy"]) }).run(*&gameEngine); // call again to move text to right position
						}
						// put trigger here
					}
				}
				// user has pressed space on an interactable object
				for (auto walkable : explorer.getObjectsInRange().internalList) {
					if (walkable.data.getKeys().contains("cutscene") and controller.haveOneOfTheseBeenPressed(VK_SPACE)) {
						whichCutscene = walkable.data["cutscene"];
						stopExploringStartCutscene = true;
						popUpTextNeedsToBeDrawn = false;
					}
				}

				if (!popUpTextNeedsToBeDrawn and mapPopUpTextExists) {
					Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", explorer.mapPopupTextID})).run(*&gameEngine);
				}
				
				Event("userInput", "DEBUGWALKING", { }).run(*&gameEngine);
				Event("userInput", "DEBUGUSERINPUT", { }).run(*&gameEngine);
				if (stopExploringStartCutscene) {
					gameEngine.activeProcedure = gameEngine.storedProcedures[whichCutscene];
					return false;
				}
				return false;
			}
			if (type == "DIALOGUESTART") {
				string speakerID = data["speaker"];
				string line = data["line"];
				string direct = data["direct"]; // 1 = use string here 0 = get string from Strings.h
				if (speakerID == "PLAYER") {
					string speakerID = saveContainer.getCurrentMainCharacter();
				}
				string speakerImageID = imageLookup.getSequenceAsString(speakerID, "SPEAKER");
				Event("Load TextBox", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("sources", to_string(TEXTBOX)),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "86"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "1"),
				pair<string, string>("uniqueID", "TEXTBOX"), }))).run(*&gameEngine);
				Event("Load Speaker", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("sources", speakerImageID),
				pair<string, string>("x", "0"),
				pair<string, string>("y", "65"),
				pair<string, string>("anchor", "TOPLEFT"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "2"),
				pair<string, string>("uniqueID", "SPEAKER"), }))).run(*&gameEngine);
				Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("message",speakerID),
				pair<string, string>("direct", "1"),
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
				}))).run(*&gameEngine);
				bool finishedWriting = Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("message",line),
				pair<string, string>("format", "Centaur_25"),
				pair<string, string>("anchorStyle", "TOPLEFT"),
				pair<string, string>("x", "20"),
				pair<string, string>("y", "79"),
				pair<string, string>("w", "78"),
				pair<string, string>("h", "33"),
				pair<string, string>("direct",direct),
				pair<string, string>("colour", "BLACK"),
				pair<string, string>("shadowColour", "DARKBROWN"),
				pair<string, string>("layer", "15"),
				pair<string, string>("uniqueID", "debugText1"),
				pair<string, string>("animated", "TRUE"),
				pair<string, string>("styles", "TYPEWRITER,PARCHMENT"),
					}))).run(*&gameEngine);
				return finishedWriting;
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
		bool run(GameEngine & gameEngine) {
			while (!eventList.empty() and eventList.front().run(*&gameEngine)) {
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
			Event("Load Map", "LOADMAP", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("sources", to_string(MAP_DEBUG)),
				pair<string, string>("uniqueID", "debugmap"),
			}))),
			Event("Debug Exploring", "DEBUGEXPLORE", Map<string,string>(List<pair<string,string>>({}))),
			})) }),
		pair<string, Procedure>({ "DeadHorse", Procedure("Cutscene", List<Event>({Event("Line1", "DIALOGUESTART", {Map<string, string>(List<pair<string,string>>({
				pair<string, string>({"speaker", "EXPLORER"}),
				pair<string, string>({"line", "example text."}),
				pair<string, string>({"direct", "1"}),
			}))})}))}),
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
		activeProcedure.run(*this);
	}

	Procedure activeProcedure;
	Map<string, string> stateFlags;
};
GameEngine game;