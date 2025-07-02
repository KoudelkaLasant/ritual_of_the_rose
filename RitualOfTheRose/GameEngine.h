#pragma once

#include "Combat.h"


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
					throw runtime_error("Can't have an image with no sources.");
				}
				for (int x = 0; x < sources.size(); x++) {
					intSources.push_back(stoi(sources.at(x)));
				}
				if (intSources.empty()) {
					throw runtime_error("Can't have an image with no sources.");
				}
				pair<float, float> position = { stof(data["x"]), stof(data["y"]) };
				string anchor = data["anchor"];
				float opacity = stof(data["opacity"]);
				int layer = stoi(data["layer"]);
				string uniqueID = data["uniqueID"];
				if (graphics.doesThisImageAlreadyExist(uniqueID)) { return true; }
				Graphics::Image * image = graphics.addImage(new Graphics::Image(intSources, position, anchor, opacity, uniqueID, data["sources"]), layer);
				if (data.hasKey("yStretch")) {
					image->yStretch = stof(data["yStretch"]);
				}
				if (data.hasKey("xStretch")) {
					image->xStretch = stof(data["xStretch"]);
				}
				bool animated = data.getKeys().contains("animated");
				int animationSpeed = 0;
				if (animated) {
					animationSpeed = stoi(data["animation_speed"]);
					image->animated = true;
					image->animationSpeed = animationSpeed;
					image->animationStyles = split(data["styles"], "$");
				}
				if (data.getKeys().contains("scale")) {
					image->scale = stof(data["scale"]); // make maps x2 the size by default
				}
				image->scale *= controller.initialGUIScale;
				return true;
			}
			if (type == "STOPALLSONGS") {
				List<string> existingSongs = explorer.currentMap.getSongNames();
				for (auto song : existingSongs.internalList) {
					string songName = split(song, " ").at(0);
					audio.fadeOutAndStopThis(stoi(songName), 3);
				}
				return true;
			}
			if (type == "STOPTHISSONG") {
				audio.fadeOutAndStopThis(stoi(data["uniqueID"]), 3);
				return true;
			}
			if (type == "PLAYALLSONGSFORTHISMAP") {
				// for when combat ends and we go back to explore mode
				List<string> existingSongs = explorer.currentMap.getSongNames();
				for (auto song : existingSongs.internalList) {
					string songName = split(song, " ").at(0);
					string volumeName = split(song, " ").at(1);
					float volume = audio.getVolumeBasedOnName(volumeName);
					if (!audio.isThisAudioLoaded(stoi(songName))) {
						audio.loadAudio(stoi(songName));
					}
					audio.playSound(stoi(songName), volume, true, true, 0);
				}
				return true;
			}
			if (type == "PLAYTHISSONG") {
				string songName = data["uniqueID"];
				float volume = audio.getVolumeBasedOnName("MusicVolume");
				if (!audio.isThisAudioLoaded(stoi(songName))) {
					audio.loadAudio(stoi(songName));
				}
				audio.playSound(stoi(songName), volume, true, true, 0);
				return true;
			}
			if (type == "MANAGEAUDIOSWAP") {
				string previousMap = explorer.currentMap.name;
				string targetMap = data["targetMap"];
				bool unloadAudio = data.hasKey("unloadAudio");
				
				List<string> existingSongs = explorer.currentMap.getSongNames();
				List<string> upcomingSongs;
				upcomingSongs = explorer.maps[targetMap].getSongNames();

				for (auto song : existingSongs.internalList) {
					string songName = split(song, " ").at(0);
					if (!upcomingSongs.contains(song)) {
						audio.fadeOutAndStopThis(stoi(songName), 3);
						if (unloadAudio) {
							audio.unloadThisAudio(stoi(songName));
						}
					}
				}

				for (auto song : upcomingSongs.internalList) {
					string songName = split(song, " ").at(0);
					string volumeName = split(song, " ").at(1);
					float volume = audio.getVolumeBasedOnName(volumeName);
					if (!existingSongs.contains(song)) {
						if (!audio.isThisAudioLoaded(stoi(songName))) {
							audio.loadAudio(stoi(songName));
						}
						audio.playSound(stoi(songName), volume, true, true, 0);
					}
				}
				return true;
			}
			if (type == "SHOWMAPLOADINGSCREEN") {
				string mapName = data["targetMap"];
				string uniqueID = "LoadingScreen";
				string loadingScreenSource = "";
				int speed = 10;
				if (data.hasKey("speed")) {
					speed = stoi(data["speed"]);
				}
				if (data.hasKey("direct")) {
					loadingScreenSource = data["direct"];
					}
				else {
					loadingScreenSource = explorer.maps[mapName].data["LoadingScreenImage"];
				}
				if (!graphics.doesThisImageAlreadyExist(uniqueID)) {
					Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", loadingScreenSource),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "0.0"),
						pair<string, string>("layer",to_string(imageLookup.layerDefaults["LOADINGSCREEN"])),
						pair<string, string>("scale", "1.1"),
						pair<string, string>("uniqueID", uniqueID), }))).run(*&gameEngine);
					return false;
				}
				if (!CLOCK.hasEnoughTimePassed("loadingscreenmapfade", speed)) {
					return false;
				}
				Graphics::Image* theImage = graphics.accessImageViaUniqueID(uniqueID);
				theImage->opacity = TChange(theImage->opacity,0.1f,0.0f,1.0f);
				if (theImage->opacity == 1.0f ) {
					CLOCK.startClockIfItDoesNotExist("makesureLoadingScreenIsOpaque");
					if (CLOCK.hasEnoughTimePassed("makesureLoadingScreenIsOpaque", 100)) {
						CLOCK.eraseClock("makesureLoadingScreenIsOpaque");
						return true;
					}
				}
				
				return false;
			}
			if (type == "UNLOADIMAGESFORMAPCHANGE") {
				for (auto layer : graphics.ImageMap.getKeys().internalList) {
					if (layer == imageLookup.layerDefaults["LOADINGSCREEN"]) { continue; }
					graphics.teardownAllImagesOnThisLayer(layer);
				}
				graphics.tearDownAllText();
				return true;
			}
			if (type == "PLAYSFX") {
				int delay = 0;
				if (data.hasKey("delay")) {
					delay = stoi(data["delay"]);
				}
				if (data["direct"] == "1") {
					audio.loadAudio(stoi(data["audio"]));
					audio.playSound(stoi(data["audio"]), audio.volumes["SFXVolume"], false, false, 0);
				}
				else {
					audio.playRandomSFXFromThisCollection(data["audio"], audio.volumes["SFXVolume"]);
				}
				
				return true;
			}
			if (type == "RELOADMAPOBJECTS") {
				for (auto& x : explorer.currentMap.objects.internalList) {
					Map<string, string> data = x.data;
					if (data.hasOneOfTheseKeys({ "don'tLoadIfNot", "don'tLoadIfPlayer", "don'tLoadIf"})) {
						bool needToLoad = true;
						if (data.hasKey("don'tLoadIfNot")) {
							string flagName = data["don'tLoadIfNot"];
							bool status = saveContainer.current.flags[flagName];
							needToLoad = status;
						}
						if (data.hasKey("don'tLoadIfPlayer")) {
							bool status = saveContainer.getCurrentMainCharacter() != data["don'tLoadIfPlayer"];
							needToLoad = status;
						}
						if (data.hasKey("don'tLoadIf")) {
							string flagName = data["don'tLoadIf"];
							bool status = saveContainer.current.flags[flagName];
							if (data.hasKey("don'tLoadIfNot") and needToLoad == false) {}
							else {
								needToLoad = !status;
							}
						}
						if (!needToLoad) {
							x.visible = false;
							x.canInteract = false;
							x.obstruction = false;
						}
						else {
							if (x.imageSources != "") {
								x.visible = true;
							}
							x.canInteract = true;
						}
					}

					if (x.imageSources == "") { continue; }
					string opacity = x.opacity;
					if (!x.visible) {
						opacity = "0.0";
					}
					string scale = x.scale;
					if (graphics.doesThisImageAlreadyExist(x.name)) {
						Event("Teardown", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
							pair<string, string>("uniqueID", x.name),
							}))).run(*&gameEngine);
					}
					string imageSources = x.imageSources;
					if (x.name.find("$PLAYER1$") != -1) {
						imageSources = imageLookup.getSequenceAsString(x.name, "STAND_FRONT");
					}
					Event("Load" + x.name, "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageSources),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", opacity),
						pair<string, string>("scale", scale),
						pair<string, string>("layer", to_string(x.layer)),
						pair<string, string>("animated", x.animated),
						pair<string, string>("animation_speed", to_string(x.animationSpeed)),
						pair<string, string>("styles", "LOOP"),
						pair<string, string>("uniqueID", x.name), }))).run(*&gameEngine);
				}
				Event("", "MAPMOVE", {}).run(*&gameEngine);
				Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", explorer.mapPopupTextID})).run(*&gameEngine);
				return true;
			}
			if (type == "LOADMAP") {
				string mapName = data["targetMap"];
				string explorerName = saveContainer.getCurrentMainCharacter();
				explorer.loadMap(mapName);
				if (data.hasKey("putPlayerHere")) {
					explorer.playerOnMap.position = { stof(data["x"]), stof(data["y"]) };
				}
				string direction = "FRONT";
				string action = "STAND";
				string directionPlusAction = "STAND_FRONT";
				if (data.hasKey("direction")) {
					directionPlusAction = data["direction"];
					direction = split(data["direction"], "_").at(1);
				}
				Event("Load Map", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", to_string(explorer.currentMap.source)),
					pair<string, string>("x", "50"),
					pair<string, string>("y", "50"),
					pair<string, string>("anchor", "CENTRE"),
					pair<string, string>("opacity", "1.0"),
					pair<string, string>("layer", "0"),
					pair<string, string>("scale", "2.0"),
					pair<string, string>("uniqueID", mapName), }))).run(*&gameEngine);
				Event("Load" + explorerName, "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", imageLookup.getSequenceAsString(explorerName, directionPlusAction)),
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
					pair<string, string>("sources", imageLookup.getSequenceAsString("Shadow " + explorerName, directionPlusAction)),
					pair<string, string>("x", "50"),
					pair<string, string>("y", "50"),
					pair<string, string>("anchor", "BOTTOMMIDDLE"),
					pair<string, string>("opacity", "0.5"),
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["PLAYER"] - 1)),
					pair<string, string>("animated", "1"),
					pair<string, string>("animation_speed", "500"),
					pair<string, string>("styles", "LOOP"),
					pair<string, string>("uniqueID", explorerName + "_Shadow")}))).run(*&gameEngine);
				graphics.accessImageViaUniqueID(explorerName + "_Explore")->direction = direction;
				graphics.accessImageViaUniqueID(explorerName + "_Shadow")->direction = direction;
				Event("LoadMapObjects", "RELOADMAPOBJECTS", {}).run(*&gameEngine);
				CLOCK.startClock("DialogueEnded"); // wait short time once a cutscene ends before moving on
				return true;
			}
			if (type == "SWAPEXPLORERS") {
				string previous = data["previous"];
				string next = data["next"];
				string previousExplorerImageID = previous + "_Explore";
				string previousExplorerShadowImageID = previous + "_Shadow";
				string newExplorerImageID = next + "_Explore";
				string newExplorerShadowImageID = next + "_Shadow";
				Graphics::Image* explorer = graphics.accessImageViaUniqueID(previousExplorerImageID);
				Graphics::Image* shadow = graphics.accessImageViaUniqueID(previousExplorerShadowImageID);
				string direction = explorer->direction;
				if (direction == "") {
					direction = "FRONT";
				}
				List<int> newSources = imageLookup.getSequence(next, "STAND_" + direction);
				List<int> newShadowSources = imageLookup.getSequence("Shadow " + next, "STAND_" + direction);
				explorer->resetSources(*&graphics, newSources);
				shadow->resetSources(*&graphics, newShadowSources);
				explorer->unique_ID = newExplorerImageID;
				shadow->unique_ID = newExplorerShadowImageID;
				controller.resetMouseClickPosition();
				controller.resetMouseMovePosition();
				controller.resetMouseUnclickPosition();
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
				shadowImage->positionAsPercentage.second -= 0.0001f;
				for (auto const& x : explorer.currentMap.objects.internalList) {
					if (!x.visible) { continue; }
					Graphics::Image* objectImage = graphics.accessImageViaUniqueID(x.name);
					objectImage->positionAsPercentage = imagePositions[x.name];
				}
				if (graphics.doesThisTextAlreadyExist(explorer.mapPopupTextID) and data.getKeys().contains("copy")) {
					int layer = imageLookup.layerDefaults["TEXTONMAP"];
					Graphics::Text * theText = graphics.accessTextViaUniqueID(explorer.mapPopupTextID);
					theText->positionAsPercentage = imagePositions[data["copy"] + "_text"];
					theText->resetText(*&graphics);
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
				string origMessage = data["message"];
				wstring message = wstring(data["message"].begin(), data["message"].end());
				if (message == L"$LATESTCOMBATSTRING$") {
					message = combat.currentBattle->combatMessages.front().second;
				}
				if (message == L"$INFINITYSYMBOL$") {
					message = L"∞";
				}

				if (data["animateExisting"] == "1") {
					data["x"] = "0";
					data["y"] = "0";
					data["w"] = "0";
					data["h"] = "0";
					data["direct"] = "1";
					data["layer"] = "0";
					data["styles"] = "TYPEWRITER,PARCHMENT";
					data["animated"] = "TRUE";
				}
				if (data["direct"] != "1" and message.find(L"$LANGUAGE$") != -1) {
					origMessage = SReplace(origMessage, "$LANGUAGE$", gameEngine.language);
				}

				List<string> mp = split(origMessage, "_");
				
				if (data["direct"] != "1") {
					message = strings[mp.at(0)][mp.at(1)][mp.at(2)];
				}
				if (data["getStringFromCombatant"] == "printout") {
					message = combat.loadPartyMemberAsCombatant(data["message"]).getPrintout(gameEngine.language,*&combat, gameEngine.stateFlags["includeEquipmentInStatView"] == "1");
				}
				if (data["getStringFromCombatant"] == "pointsLeft") {
					message = combat.loadPartyMemberAsCombatant(data["message"]).getPointsRemainingPrintout(gameEngine.language, *&combat);
				}
				if (data["getStringFromCombatant"] == "skillprintout") {
					message = combat.loadPartyMemberAsCombatant(data["message"]).getSkillPrintOut(gameEngine.language, data["skill"], * &combat, data["influences"]=="1");
					if (combat.currentBattle != NULL) {
						Combat::CombatantInstance* actor = combat.currentBattle->getThisCombatant(combat.currentBattle->currentRound.whoseTurnIsIt());
						wstring canThisBeUsedOrNotMessage = actor->getCanIUseThisSkillOrNotMessage(*&combat, data["skill"]);
						canThisBeUsedOrNotMessage = WSReplace(canThisBeUsedOrNotMessage, L" ", L"⑥");
						message += L"\n⑥\n" + canThisBeUsedOrNotMessage + L"⑥\n⑥\n";
					}
				}
				if (data["getStringFromCombatant"] == "learningTomes") {
					message = combat.definedCombatants["Tomes"].getSkillPrintOut(gameEngine.language, data["skill"], *&combat, data["influences"] == "1");
				}
				if (data["getStringFromCombatant"] == "skillName") {
					message = strings[gameEngine.language]["Skill Names"][data["skill"]];
				}
				
				if (data["getColourFromEquipment"] == "1") {
					string colourTag = combat.equipmentDefinitions[data["itemName"]].textColour;
					wchar_t wColourTag = graphics.colourTagLookupTable.getKeyAssociatedWithThisValue(colourTag);
					wstring colourTagAsSymbol = wstring(1, wColourTag);
					message = colourTagAsSymbol + message;
					message = WSReplace(message, L" ", colourTagAsSymbol) + colourTagAsSymbol;
				}
				if (data["sayHowManyPlayerHas"] == "1") {
					int howMany = saveContainer.current.inventory[data["itemName"]];
					message += L" x" + to_wstring(howMany);
				}
				if (data["addPriceToEquipment"] == "1") {
					message += L" " + to_wstring(combat.equipmentDefinitions[data["itemName"]].price);
				}
				if (data["getMessageFromMoney"] == "1") {
					message = to_wstring(saveContainer.current.money);
				}
				if (data["uniqueID"] == "explainEquipmentHover") {
					if (data["hoverTextOneLine"] == "1") {
						message = combat.equipmentDefinitions[data["message"]].printoutOnOneLine(gameEngine.language, *&combat);
					}
					else {
						message = combat.equipmentDefinitions[data["message"]].printout(gameEngine.language, *&combat);
					}
				}
				message = Graphics::Text::commonTextReplacements(gameEngine.language, message);

				string format = data["format"];
				pair<float, float> position = { stof(data["x"]), stof(data["y"]) };
				string anchorStyle = data["anchorStyle"];
				pair<float, float> size = { stof(data["w"]), stof(data["h"]) };
				vector<float> colour = graphics.Colours[data["colour"]];
				vector<float> shadowColour = graphics.Colours[data["shadowColour"]];
				int layer = stoi(data["layer"]);
				string uniqueID = data["uniqueID"];
				List<string> styles = split(data["styles"], ",");
				bool animated = data["animated"] == "TRUE";
				int typewriterSpeed = 5;
				if (data.hasKey("typewriterSpeed")) {
					typewriterSpeed = stoi(data["typewriterSpeed"]);
				}

				if (!graphics.doesThisTextAlreadyExist(uniqueID)) {
					data["animateExisting"] = "0";
					Graphics::Text * theText = graphics.addText(new Graphics::Text(*&graphics, message, format, position, anchorStyle, size, colour, shadowColour, uniqueID,animated,styles,typewriterSpeed), layer);
					if (animated) {
						if (styles.contains("TYPEWRITER")) {
							if (message == L"") {
								return true; // there is no dialogue to animate
							}
							theText->resetMessage(*&graphics, graphics.insertNewlines(message));
							theText->startTypewriter(*&graphics);
						}
					}
				}
				else {
					Graphics::Text* theText = graphics.accessTextViaUniqueID(uniqueID);
					if (!animated) {
						if (theText->getMessage() != message) {
							theText->resetMessage(*&graphics, message);
						}
					}
					if (animated and styles.contains("TYPEWRITER")) {
						if (data["animateExisting"] == "0") {
							if (theText->fullMessage == L"") {
								theText->resetMessage(*&graphics, graphics.insertNewlines(message));
								theText->startTypewriter(*&graphics);
							}
						}
						bool finished = theText->getMessage().find(L"⑤") == -1;
						int howFarAlong = theText->howFarAlong();
						if (finished and CLOCK.hasEnoughTimePassed("TypewriterFinished", 100)) {
							return true;
						}
						if (data["nowait"] == "1") {
							return true;
						}
					}
				}
				if (!animated) {
					return true;
				}
				
				return false;
			}
			if (type == "TEARDOWNIMAGE") {
				if (graphics.doesThisImageAlreadyExist(data["uniqueID"])) {
					graphics.tearDownSpecifiedImage(data["uniqueID"]);
				}
				return true;
			}
			if (type == "TEARDOWNTEXT") {
				string uniqueID = data["uniqueID"];
				bool textExists = graphics.doesThisTextAlreadyExist(uniqueID);
				if (textExists) {
					graphics.tearDownSpecifiedText(data["uniqueID"]);
				}
				return true;
			}
			if (type == "DEBUGUSERINPUT") {
				string uniqueID = "DEBUGTEXT";
				int layer = imageLookup.layerDefaults["DEBUGUSERINPUT"];
				if (!graphics.doesThisTextAlreadyExist(uniqueID)) {
					graphics.addText(new Graphics::Text(*&graphics, L"", "Centaur_25", { 0,0 }, "TOPLEFT", { 100,100 }, graphics.Colours["OBVIOUSPINK"], graphics.Colours["BLACK"], uniqueID, false, {},0), layer);
				}
				Graphics::Text* theText = graphics.accessTextViaUniqueID(uniqueID);
				string message = controller.controllerDebug() + "\nBeing Clicked and Dragged: ";
				for (Graphics::Image* dragged : graphics.beingDragged.internalList) {
					message += dragged->unique_ID;
				}
				message += "\n";

				string hoverMessage = "Being Hovered: ";
				string clickMessage = "Being Clicked On: ";

				for (int layer : {20}) {
					for (int x = 0; x < graphics.ImageMap[layer].size(); x++) {
						if (graphics.ImageMap[layer].at(x)->hasThisBeenClickedOn(*&graphics, controller.mouseMovePosition)) {
							hoverMessage += graphics.ImageMap[layer].at(x)->unique_ID + ",";
						}
						if (graphics.ImageMap[layer].at(x)->hasThisBeenClickedOn(*&graphics, controller.mouseClickPosition)) {
							clickMessage += graphics.ImageMap[layer].at(x)->unique_ID + ",";
						}
					}
				}

				message += hoverMessage + "\n" + clickMessage + "\n";
				message += "Scroll Wheel Delta: ";
				message += to_string(controller.currentScrollWheelDelta);
				message += "\n";

				theText->resetMessage(*&graphics, StringToWString(message));
				if (controller.hasThisBeenPressed(27)) {
					return true;
				}

				return false;
			}
			if (type == "DEBUGBEZIER") {
				Event("LoadImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("uniqueID", "victim"),
							pair<string, string>("sources",imageLookup.getSequenceAsString("OldBookMan", "STAND_FRONT")),
							pair<string, string>("x","0"),
							pair<string, string>("y","0"),
							pair<string, string>("layer","10"),
							pair<string, string>("scale","1.0"),
							pair<string, string>("anchor","CENTRE"),
							pair<string, string>("animated","1"),
							pair<string, string>("opacity","1.0"),
							pair<string, string>("animation_speed", "200"),
					})).run(*&gameEngine);
				return gameEngine.skillAnimationContainer.handleAnimation(*&gameEngine, graphics.accessPlayerImage(), graphics.accessImageViaUniqueID("victim"), "Life Drain", {});
			}
			if (type == "DEBUGSTRAIGHT") {
				Event("LoadImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("uniqueID", "victim"),
							pair<string, string>("sources",imageLookup.getSequenceAsString("OldBookMan", "STAND_FRONT")),
							pair<string, string>("x","0"),
							pair<string, string>("y","0"),
							pair<string, string>("layer","10"),
							pair<string, string>("scale","1.0"),
							pair<string, string>("anchor","CENTRE"),
							pair<string, string>("animated","1"),
							pair<string, string>("opacity","1.0"),
							pair<string, string>("animation_speed", "200"),
					})).run(*&gameEngine);
				return gameEngine.skillAnimationContainer.handleAnimation(*&gameEngine, graphics.accessPlayerImage(), graphics.accessImageViaUniqueID("victim"), "Ice Beam", {});
			}
			if (type == "DEBUGCOMBAT") {
				Map<string, string>combatData({
					pair<string, string>("team1","PARTY"),
					pair<string, string>("team1allies",""),
					pair<string, string>("team2","DEBUG"),
					pair<string, string>("team2allies",""),
					pair<string, string>("background", to_string(BATTLEBACKGROUND_ROADTOTOWN)),
					pair<string, string>("song",""),
					pair<string, string>("postBattle","RETURNTOEXPLORE"),
					pair<string, string>("LOOT$GOLD", "1"),
					pair<string, string>("LOOT$Tome of Chaos Storm", "1"),
					pair<string, string>("direction", "STAND_FRONT"),
					pair<string, string>("x", "50"),
					pair<string, string>("y", "50"),
					});
				Graphics::Image* explorerImage = graphics.accessImageViaUniqueID(saveContainer.getCurrentMainCharacter() + "_Explore");
				if (explorerImage != NULL and explorerImage->direction != "") {
					combatData["direction"] = "STAND_" + explorerImage->direction;
					combatData["x"] = to_string(explorer.playerOnMap.position.first);
					combatData["y"] = to_string(explorer.playerOnMap.position.second);
				}
				gameEngine.activeProcedure = gameEngine.makeCombatProcedure(combatData);
				return false;
			}
			if (type == "STARTCOMBAT") {
				Map<string, string>combatData = data;
				Graphics::Image* explorerImage = graphics.accessImageViaUniqueID(saveContainer.getCurrentMainCharacter() + "_Explore");
				if (explorerImage != NULL and explorerImage->direction != "") {
					combatData["direction"] = "STAND_" + explorerImage->direction;
					combatData["x"] = to_string(explorer.playerOnMap.position.first);
					combatData["y"] = to_string(explorer.playerOnMap.position.second);
				}
				gameEngine.activeProcedure = gameEngine.makeCombatProcedure(combatData);
				return false;
			}
			if (type == "CLICKANDDRAG") {				
				//Event("Debug", "DEBUGUSERINPUT", {}).run(*&gameEngine);
				List<string> draggable = split(data["objects"], ",");
				bool YLocked = data["YLock"] == "1";
				float XMin = 0;
				float XMax = 100;
				bool onlyOne = data["onlyOne"] == "1";
				if (data.hasKey("XMin")) {
					XMin = stof(data["XMin"]);
				}
				if (data.hasKey("XMax")) {
					XMax = stof(data["XMax"]);
				}
				if (controller.mouseInstructionsInOrder.contains("LButtonDown")) {
					for (Graphics::Image* image : graphics.beingDragged.internalList) {
						graphics.bumpLayer(image, -2);
					}
					pair<float, float> click = controller.mouseClickPosition;
					for (auto const& ID : draggable.internalList) {
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(ID);
						if (theImage == NULL) { continue; }
						if (onlyOne and graphics.beingDragged.size() > 0) { continue; }
						if (theImage->hasThisBeenClickedOn(*&graphics, click)) {
							graphics.beingDragged.push_back(theImage);
							}
						}
					for (Graphics::Image * image : graphics.beingDragged.internalList) {
						graphics.bumpLayer(image, 2);
					}
				}
				if (controller.mouseInstructionsInOrder.contains("MouseMove")) {
					pair<int, int> move = controller.mouseMovePosition;
					bool hoveringOverSomethingDraggable = false;
					for (auto const& ID : draggable.internalList) {
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(ID);
						if (theImage == NULL) { continue; }
						if (theImage->hasThisBeenClickedOn(*&graphics, move)) {
							hoveringOverSomethingDraggable = true;
							graphics.changeCursor("SELECTED");
						}
					}
					for (auto const& x : graphics.beingDragged.internalList) {
						pair<float, float> mouseMoveAsPercent = x->convertActualToPercent(graphics.hwndRenderTarget->GetSize(), move);
						if (YLocked) {
							mouseMoveAsPercent.second = x->positionAsPercentage.second;
						}
						mouseMoveAsPercent.first = TChange(mouseMoveAsPercent.first, 0.0f, XMin, XMax);
						x->positionAsPercentage = mouseMoveAsPercent;
					}
					if (!hoveringOverSomethingDraggable) {
						graphics.changeCursor("DEFAULT");
					}
				}
				if (controller.mouseInstructionsInOrder.contains("LButtonUp") or !controller.hasThisBeenPressed(1)) {
					// call upon drag and drop logic depending on the event
					for (Graphics::Image* image : graphics.beingDragged.internalList) {
						graphics.bumpLayer(image, -2);
					}
					for (Graphics::Image * image : graphics.beingDragged.internalList) {
						graphics.recentlyFinishedBeingDragged.addToBackIfNotAlreadyInList(image->unique_ID);
					}
					graphics.beingDragged.clear();
				}
				return false;
			};
			if (type == "HANDLEAUDIOCLICKANDDRAG") {
				List<string> draggable = split(data["objects"], ",");
				pair<float, float> click = controller.mouseClickPosition;
				if (!graphics.beingDragged.empty() and CLOCK.hasEnoughTimePassed("CHANGEVOLUME",150)) {
					Graphics::Image* theImage = graphics.beingDragged.front();
					float xPos = theImage->positionAsPercentage.first;
					string whichVolume = split(theImage->unique_ID, "_").at(0);
					audio.volumes[whichVolume] = audio.convertAudioSliderOptionToVolume(xPos);
					List<string> existingSongs = explorer.currentMap.getSongNames();
					for (auto song : existingSongs.internalList) {
						string songName = split(song, " ").at(0);
						string volumeName = split(song, " ").at(1);
						float volume = audio.volumes[volumeName];
						audio.changeTheVolumeOfThis(stoi(songName), volume);
					}
					if (whichVolume == "SFXVolume" and CLOCK.hasEnoughTimePassed("PLAYTESTSOUND", 250)) {
						Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
									pair<string,string>("audio",to_string(BUTTON_HOVER_WAV)),
									pair<string,string>("direct","1"),
							})).run(*&gameEngine);
					}
				}
				return true;
			}
			if (type == "HANDLEPARTYREARRANGECLICKANDDRAG") {
				bool forceRedraw = data["force"] == "1";
				string theObjects;
				List<string> party = saveContainer.current.party;
				if (data["mode"] == "reform") {
					party += saveContainer.getCharactersInReserve();
				}
				for (int x = 0; x < party.size(); x++) {
					theObjects += party.at(x) + "_CARD,";
				}
				Event("ClickAndDrag", "CLICKANDDRAG", Map<string, string>({
					pair<string, string>("objects", theObjects),
					pair<string, string>("onlyOne", "1"),
					})).run(*&gameEngine);
				if (!graphics.recentlyFinishedBeingDragged.empty()) {
					List<Graphics::Image*> images;
					for (int x = 0; x < min(party.size(), saveContainer.partyLimit); x++) {
						if (!graphics.recentlyFinishedBeingDragged.contains(party.at(x) + "_CARD")) {
							images.push_back(graphics.accessImageViaUniqueID(party.at(x) + "_CARD"));
						}
					}
					bool didASwap = false;
					for (Graphics::Image * image : images.internalList) {
						if (image->hasThisBeenClickedOn(*&graphics, controller.mouseUnclickPosition)) {
							string lhs = split(graphics.recentlyFinishedBeingDragged.front(), "_").at(0);
							string rhs = split(image->unique_ID, "_").at(0);
							if (lhs == rhs) {
								return true;
							}
							didASwap = true;
							forceRedraw = true;
							if (data["mode"] == "shuffle") {
								string previousLeader = saveContainer.getCurrentMainCharacter();
								saveContainer.rearrangeTheseCharactersInParty(lhs, rhs);
								string nextLeader = saveContainer.getCurrentMainCharacter();
								Event("PrintoutPlayer", "SWAPEXPLORERS", List<pair<string, string>>({
										pair<string, string>("previous", previousLeader),
										pair<string, string>("next", nextLeader),
									})).run(*&gameEngine);
							}
							if (data["mode"] == "reform") {
								string previousLeader = saveContainer.getCurrentMainCharacter();
								string lhs_source = "party";
								string rhs_source = "party";
								if (saveContainer.getCharactersInReserve().contains(lhs)) {
									lhs_source = "reserve";
								}
								if (saveContainer.getCharactersInReserve().contains(rhs)) {
									rhs_source = "reserve";
								}
								if (lhs_source == "party" and rhs_source == "party") {
									saveContainer.swapPartyMembers(lhs, rhs);
								}
								if (lhs_source == "reserve" and rhs_source == "party") {
									saveContainer.movePlayerFromReserveIntoParty(lhs, rhs);
								}
								if (lhs_source == "party" and rhs_source == "reserve") {
									saveContainer.movePlayerFromReserveIntoParty(rhs, lhs);
								}
								string nextLeader = saveContainer.getCurrentMainCharacter();
								if (previousLeader != nextLeader) {
									Event("PrintoutPlayer", "SWAPEXPLORERS", List<pair<string, string>>({ pair<string, string>("previous", previousLeader),pair<string, string>("next", nextLeader), })).run(*&gameEngine);
								}
							}
						}
					}
					if (!didASwap and data["mode"] == "reform") {
						RECT partyRegion;
						partyRegion.top = 120 * controller.initialGUIScale;
						partyRegion.left = 350 * controller.initialGUIScale;
						partyRegion.bottom = 250 * controller.initialGUIScale;
						partyRegion.right = 630 * controller.initialGUIScale;

						RECT reserveRegion;
						reserveRegion.top = 120 * controller.initialGUIScale;
						reserveRegion.left = 780 * controller.initialGUIScale;
						reserveRegion.bottom = 350 * controller.initialGUIScale;
						reserveRegion.right = 1083 * controller.initialGUIScale;

						pair<float, float> unclickPos = controller.mouseUnclickPosition;

						bool goingToParty = graphics.isThisInsideRect(unclickPos, partyRegion);
						bool goingToReserve = graphics.isThisInsideRect(unclickPos, reserveRegion);
						string lhs = split(graphics.recentlyFinishedBeingDragged.front(), "_").at(0);
						List<string> theCurrentParty = saveContainer.current.party;

						if (goingToReserve and theCurrentParty.contains(lhs) and theCurrentParty.size() > 1) {
						// move out of party
							string currentLeader = saveContainer.getCurrentMainCharacter();
							theCurrentParty.internalList.remove(lhs);
							saveContainer.current.party = theCurrentParty.internalList;
							forceRedraw = true;

							string nextLeader = saveContainer.getCurrentMainCharacter();
							if (currentLeader != nextLeader) {
								Event("PrintoutPlayer", "SWAPEXPLORERS", List<pair<string, string>>({ pair<string, string>("previous", currentLeader),pair<string, string>("next", nextLeader), })).run(*&gameEngine);
							}
						}
						if (goingToParty and !theCurrentParty.contains(lhs) and theCurrentParty.size() < saveContainer.partyLimit) {
							// move into party
							theCurrentParty.push_back(lhs);
							saveContainer.current.party = theCurrentParty.internalList;
							forceRedraw = true;
						}
					}
					forceRedraw = true;
					}
				if (forceRedraw) {
					Event("LoadMapObjects", "RELOADMAPOBJECTS", {}).run(*&gameEngine);
				}
				if (forceRedraw and data["mode"] == "shuffle") {
						List<pair<float, float>> positions = Menu::getPlayerCardPositions();
						List<string> party = saveContainer.current.party;
						for (int x = 0; x < min(party.size(), saveContainer.partyLimit); x++) {
							string who = party.at(x);
							if (!graphics.doesThisImageAlreadyExist(who + "_CARD")) { return true; }
							graphics.accessImageViaUniqueID(who + "_CARD")->positionAsPercentage = positions.at(x);
						}
						for (int x = 0; x < party.size(); x++) {
							Event("TearDown", "TEARDOWNPLAYERREADOUT", pair<string, string>("who", party.at(x))).run(*&gameEngine);
							Event("TeardownSkillBar", "TEARDOWNTHISSKILLBAR", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("who", party.at(x)) }))).run(*&gameEngine);
						}
						for (int x = 0; x < party.size(); x++) {
							string who = party.at(x);
							pair<float, float> Where = positions.at(x); Where.first -= 8; Where.second += 25;
							Event("SetUpPlayerReadoutText", "DRAWTEXT", List<pair<string, string>>({
								pair<string, string>("message", who),
								pair<string, string>("getStringFromCombatant", "printout"),
								pair<string, string>("direct", "1"),
								pair<string, string>("format", "GoudyMedieval_17"),
								pair<string, string>("anchorStyle", "TOPLEFT"),
								pair<string, string>("x", to_string(Where.first)),
								pair<string, string>("y", to_string(Where.second)),
								pair<string, string>("w", "20"),
								pair<string, string>("h",  "50"),
								pair<string, string>("colour", "WHITE"),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer",  to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("uniqueID", who + "_PRINTOUTTEXT"),
								})).run(*&gameEngine);
							Event("SetUpPlayerPointsLeftText", "DRAWTEXT", List<pair<string, string>>({
								pair<string, string>("message", who),
								pair<string, string>("getStringFromCombatant", "pointsLeft"),
								pair<string, string>("direct", "1"),
								pair<string, string>("format", "GoudyMedieval_12"),
								pair<string, string>("anchorStyle", "CENTRE"),
								pair<string, string>("x", to_string(Where.first + 7)),
								pair<string, string>("y", to_string(Where.second + 47)),
								pair<string, string>("w", "20"),
								pair<string, string>("h",  "50"),
								pair<string, string>("colour", "WHITE"),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer",  to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("uniqueID", who + "_POINTSLEFTTEXT"),
								})).run(*&gameEngine);
							Event("LoadSkillBar", "LOADSKILLBARHERE", List<pair<string, string>>({
								pair<string, string>("who", party.at(x)),
								pair<string, string>("x", to_string(positions.at(x).first)),
								pair<string, string>("y", to_string(positions.at(x).second + 30)),
								pair<string, string>("scale", "0.25"),
								})).run(*&gameEngine);
							float buttonYGap = 2.55;
							float currentYGap = 11.5;
							for (auto attribute : combat.AttributesInOrder.internalList) {
								Event("LoadPlusButton", "LOADIMAGE", Map<string, string>({
										pair<string, string>("sources", to_string(PLUSBUTTON_DEFAULT) + " " + to_string(PLUSBUTTON_HOVERED) + " " + to_string(PLUSBUTTON_PRESSED)),
										pair<string, string>("x", to_string(Where.first + 15.5)),
										pair<string, string>("y", to_string(Where.second + currentYGap)),
										pair<string, string>("anchor", "CENTRE"),
										pair<string, string>("opacity", "1.0"),
										pair<string, string>("layer",to_string(imageLookup.layerDefaults["BUTTONS"])),
										pair<string, string>("scale", "1.0"),
										pair<string, string>("uniqueID", who + "_" + attribute + "_PLUSBUTTONIMAGE"),
									})).run(*&gameEngine);
								Event("LoadMinusButton", "LOADIMAGE", Map<string, string>({
										pair<string, string>("sources", to_string(MINUSBUTTON_DEFAULT) + " " + to_string(MINUSBUTTON_HOVERED) + " " + to_string(MINUSBUTTON_PRESSED)),
										pair<string, string>("x", to_string(Where.first + 14)),
										pair<string, string>("y", to_string(Where.second + currentYGap)),
										pair<string, string>("anchor", "CENTRE"),
										pair<string, string>("opacity", "1.0"),
										pair<string, string>("layer",to_string(imageLookup.layerDefaults["BUTTONS"])),
										pair<string, string>("scale", "1.0"),
										pair<string, string>("uniqueID", who + "_" + attribute + "_MINUSBUTTONIMAGE"),
									})).run(*&gameEngine);
								currentYGap += buttonYGap;
							}
						}
						graphics.recentlyFinishedBeingDragged.clear();
						return true;
				}
				if (forceRedraw and data["mode"] == "reform") {
						Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
						pair<string, string>("offsetX", "32"),
						pair<string, string>("offsetY", "25"),
						pair<string, string>("scale", data["scale"]),
						pair<string, string>("what", "PARTY"),
						pair<string, string>("moveExisting", "1"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							})).run(*&gameEngine);
						Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
							pair<string, string>("offsetX", "67"),
							pair<string, string>("offsetY", "25"),
							pair<string, string>("scale", data["scale"]),
							pair<string, string>("what", "RESERVES"),
							pair<string, string>("moveExisting", "1"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							})).run(*&gameEngine);
						graphics.recentlyFinishedBeingDragged.clear();
						return true;
				}
				
				return false;
			}
			if (type == "DEBUGWALKING") {
				string textUniqueID = "DEBUGWALKING";
				int layer = 100;
				if (!graphics.doesThisTextAlreadyExist(textUniqueID)) {
					graphics.addText(new Graphics::Text(*&graphics, L"", "Centaur_25", { 2,80 }, "TOPLEFT", { 100,100 }, graphics.Colours["WHITE"], graphics.Colours["BLACK"], textUniqueID, false, {}, 0), layer);
				}
				Graphics::Text* theText = graphics.accessTextViaUniqueID(textUniqueID);
				theText->resetMessage(*&graphics, StringToWString(explorer.debug()));
				return false;
			}
			if (type == "DEBUGLOAD") {
				filesystem::path defaultSavePath = filesystem::current_path() / "slot_0";
				filesystem::path debugSavePath = filesystem::current_path() / "slot_-1";
				saveContainer.activeSaveSlot = -1;
				SaveContainer::SaveFile save(defaultSavePath);
				save.saveToDisk(debugSavePath);
				saveContainer.load(debugSavePath);
				for (auto equipmentDef : combat.equipmentDefinitions.getValues().internalList) {
					saveContainer.current.inventory[equipmentDef.uniqueID] = 1;
				}
				saveContainer.current.equippedSkills = combat.defaultSkillChoices.internalMap;
				saveContainer.current.attributeInvestments = combat.defaultAttInvestments.internalMap;
				saveContainer.current.equippedSkillTrees = combat.defaultSkillTreeChoices.internalMap;
				saveContainer.current.equippedItems = combat.defaultEquipment.internalMap;
				for (auto s : saveContainer.current.allCharacters) {
					saveContainer.current.knownSkills[s] = combat.getAllLegalSkills().internalList;
				}
				codex.unlockEveryCodexPage();
				return true;
			}
			if (type == "EXPLORE") {
				//Event("", "DEBUGUSERINPUT", {}).run(*&gameEngine);

				graphics.changeCursor("NONE");

				bool force = data["force"] == "1";
				bool moving = false;
				if (force) { 
					moving = true; 
					data["force"] = "0";
				}
				bool need_to_reset_image_sources = true;
				string newDirection = "";
				string newAction = "STAND";
				Map<string, int> exploreAnimationSpeeds = explorer.getAnimationSpeeds();
				if (Args.get("mode") == "DEBUG") {
					if (controller.hasThisBeenPressed(VK_F1)) {
						gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("DEBUGMENU");
					}
					if (controller.hasThisBeenPressed(VK_F2)) {
						gameEngine.activeProcedure.eventList.push_front(Event("Debug Combat", "DEBUGCOMBAT", {}));
						return false;
					}
					if (controller.hasThisBeenPressed(VK_F3)) {}
					if (Args.get("SPEEDCHEAT") == "1") {
						exploreAnimationSpeeds["MOVE"] = 10;
						Event("userInput", "DEBUGWALKING", { }).run(*&gameEngine);
					}
					if (Args.get("DEBUGUSERINPUT") == "1") {
						Event("userInput", "DEBUGUSERINPUT", { }).run(*&gameEngine);
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
				bool stopExploringChangeArea = false;
				bool stopExploringLoadMerchant = false;
				bool popUpTextNeedsToBeDrawn = false;
				bool mapPopUpTextExists = false;
				bool stopExploringOpenChest = false;
				Map<string, string> chestData;
				string whichCutscene = "";
				string currentMap = explorer.currentMap.name;
				string targetMap = "";
				string targetMerchant = "";
				pair<float, float> futurePlayerPosition = {0,0};
				string futurePlayerDirection = "FRONT";
				Map<string, string> walkableDataToMoveOn; // send this to function that deals with next step
				if (force) { moving = false; force = false; }
				if (force or (moving and CLOCK.hasEnoughTimePassed("EXPLORE",exploreAnimationSpeeds["MOVE"]))) {
					// allow for 8-way movement without new animation
					string eightWayMove = "";
					for (auto const& x : controller.keysPressedInOrderAsInts.internalList) {
						if (newDirection == "FRONT") {
							if (controller.left.contains(x)) {
								eightWayMove = "LEFT";
								break;
							}
							if (controller.right.contains(x)) {
								eightWayMove = "RIGHT";
								break;
							}
						}
						if (newDirection == "LEFT") {
							if (controller.up.contains(x)) {
								eightWayMove = "BACK";
								break;
							}
							if (controller.down.contains(x)) {
								eightWayMove = "FRONT";
								break;
							}
						}
						if (newDirection == "BACK") {
							if (controller.left.contains(x)) {
								eightWayMove = "LEFT";
								break;
							}
							if (controller.right.contains(x)) {
								eightWayMove = "RIGHT";
								break;
							}
						}
						if (newDirection == "RIGHT") {
							if (controller.up.contains(x)) {
								eightWayMove = "BACK";
								break;
							}
							if (controller.down.contains(x)) {
								eightWayMove = "FRONT";
								break;
							}
						}
					}
					if (eightWayMove == "") {
						explorer.tryToMovePlayer(newDirection, explorer.unitOfMovement);
					}
					else {
						explorer.tryToMovePlayer(eightWayMove, explorer.unitOfMovement / 1.41421);
						explorer.tryToMovePlayer(newDirection, explorer.unitOfMovement / 1.41421);
					}
					

					if (data["audio"] != "0") {
						if (data["noaudioyet"] == "1") {
							data["noaudioyet"] = "0";
						}
						else {
							explorer.playWalkingAudio();
						}
					}
					Event("Map Move", "MAPMOVE", {}).run(*&gameEngine);
					image->opacity = 1.0;
					shadowImage->opacity = 0.5;
					mapPopUpTextExists = graphics.doesThisTextAlreadyExist(explorer.mapPopupTextID);
					popUpTextNeedsToBeDrawn = false;
					for (auto walkable : explorer.getCurrentlySteppedOn().internalList) {
						if (walkable.data["trans"] == "1") {
							image->opacity = 0.5;
							shadowImage->opacity = 0;
						}
					}
					for (auto walkable : explorer.getObjectsThatAreClose().internalList) {
						if (walkable.data.getKeys().contains("message") and walkable.canInteract) {
							popUpTextNeedsToBeDrawn = true;
							Event("PopUpTextOnMap", "DRAWTEXT", walkable.data).run(*&gameEngine);
								mapPopUpTextExists = true;
							Event("Map Move", "MAPMOVE", { pair<string, string>("copy",walkable.data["copy"]) }).run(*&gameEngine); // call again to move text to right position
						}
					}
					for (auto walkable : explorer.getObjectsInRange().internalList) {
						// objects that are stepped on but don't require interaction
						if (walkable.data.hasKey("GoingUp")) {
							bool needToChangeAudio = false;
							bool up = false;

							if (walkable.data["GoingUp"] == "1" and gameEngine.stateFlags["GoingUp"] == "") { // first step to going up
								gameEngine.stateFlags["GoingUp"] = "1";
							}
							if (walkable.data["GoingUp"] == "2" and gameEngine.stateFlags["GoingUp"] == "1") { // going up
								gameEngine.stateFlags["GoingUp"] = "2";
								string imageName = walkable.data["image"];
								string obstructionName = walkable.data["obstruction"];
								
								graphics.bumpLayer(graphics.accessImageViaUniqueID(imageName), -2);
								explorer.getThisMapObject(obstructionName).obstruction = true;
								needToChangeAudio = true;
								up = true;
							}
							if ((walkable.data["GoingUp"] == "3" and gameEngine.stateFlags["GoingUp"] != "") or (walkable.data["GoingUp"] == "1" and gameEngine.stateFlags["GoingUp"] == "2")) { // going down
								gameEngine.stateFlags["GoingUp"] = "";
								string imageName = walkable.data["image"];
								string obstructionName = walkable.data["obstruction"];
								graphics.bumpLayer(graphics.accessImageViaUniqueID(imageName), 2);
								explorer.getThisMapObject(obstructionName).obstruction = false;
								needToChangeAudio = true;
							}
							if (walkable.data.hasKey("audioSwap") and needToChangeAudio) {
								List<string> audioSwapData = split(walkable.data["audioSwap"], "=");
								string floorName = audioSwapData.at(0);
								string audioSourceName = audioSwapData.at(1);
								if (!up) {
									audioSourceName = audioSwapData.at(2);
								}
								for (int x = 0; x < explorer.currentMap.walkables.size(); x++) {
									if (explorer.currentMap.walkables.at(x).uniqueID == floorName) {
										explorer.currentMap.walkables.at(x).data["audio source"] = audioSourceName;
									}
								}
							}
						}
					}
				}
				// user has walked in range of an interactible object
				bool userInput = controller.haveOneOfTheseBeenPressed(VK_SPACE) and CLOCK.hasEnoughTimePassed("DialogueEnded", 1000);
				for (auto walkable : explorer.getObjectsThatAreClose().internalList) {
					if (walkable.data.getKeys().contains("message") and walkable.canInteract) {
						popUpTextNeedsToBeDrawn = true;
					}
					if (walkable.data.hasKey("trigger") and walkable.canInteract) {
						explorer.disableThisObject(walkable.name);
						saveContainer.current.flags[walkable.name + "_TRIGGERED"] = true;
						whichCutscene = walkable.data["cutscene"];
						whichCutscene = FlagDependentCutsceneNameFinder::getNameOfCutsceneDependingOnFlags(whichCutscene);
						stopExploringStartCutscene = true;
					}
					if (walkable.data.getKeys().contains("cutscene") and userInput and walkable.canInteract) {
						whichCutscene = walkable.data["cutscene"];
						whichCutscene = FlagDependentCutsceneNameFinder::getNameOfCutsceneDependingOnFlags(whichCutscene);
						stopExploringStartCutscene = true;
						popUpTextNeedsToBeDrawn = false;
					}
					if (walkable.data.getKeys().contains("areaTransition") and userInput and walkable.canInteract) {
						currentMap = explorer.currentMap.name;
						targetMap = walkable.data["areaTransition"];
						futurePlayerPosition = { stof(walkable.data["playerPosX"]), stof(walkable.data["playerPosY"]) };
						futurePlayerDirection = "BACK";
						walkableDataToMoveOn = walkable.data;
						stopExploringChangeArea = true;
					}
					if (walkable.data.getKeys().contains("Merchant") and userInput) {
						targetMerchant = walkable.name;
						stopExploringLoadMerchant = true;
					}
					if (walkable.data.getKeys().contains("isAChest") and walkable.canInteract and userInput) {
						stopExploringOpenChest = true;
						chestData = walkable.data;
						popUpTextNeedsToBeDrawn = false;
					}
				}
				if (stopExploringLoadMerchant) {
					gameEngine.storedMenus["MERCHANT"].data["MERCHANT"] = "Buy";
					gameEngine.activeProcedure = gameEngine.makeMerchantLoadProcedure(targetMerchant);
					controller.menuItemCooldown = true;
				}
				if (!popUpTextNeedsToBeDrawn and mapPopUpTextExists) {
					Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", explorer.mapPopupTextID})).run(*&gameEngine);
				}
				if (stopExploringStartCutscene) {
					if (currentDirection != "") {
						Event("MakePlayerStopMoving", "ANIMATEIMAGEONMAP", Map<string, string>({
					pair<string, string>("whichImage", uniqueID),
					pair <string, string>("character", character),
					pair <string, string>("direction", currentDirection),
					pair <string, string>("action", "STAND"),
							})).run(*&gameEngine);
						Event("MakePlayerStopMoving", "ANIMATEIMAGEONMAP", Map<string, string>({
						pair<string, string>("whichImage", shadowID),
						pair <string, string>("character", "Shadow " + character),
						pair <string, string>("direction", currentDirection),
						pair <string, string>("action", "STAND"),
							})).run(*&gameEngine);
						
					}
					if (!gameEngine.storedProcedures.getKeys().contains(whichCutscene)) {
						gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, whichCutscene, saveContainer.getCurrentMainCharacter(), "EXPLORE");
					}
					else {
						gameEngine.activeProcedure = gameEngine.storedProcedures[whichCutscene];
					}
				}
				if (stopExploringChangeArea) {
					gameEngine.activeProcedure = gameEngine.makeAreaTransitionProcedure(currentMap, targetMap, futurePlayerPosition, futurePlayerDirection, walkableDataToMoveOn);
				}
				if (stopExploringOpenChest) {
					Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", explorer.mapPopupTextID})).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeOpenChestProcedure(chestData);
				}
				if (controller.haveOneOfTheseBeenPressed(VK_ESCAPE) and CLOCK.hasEnoughTimePassed("MENUINPUTDELAY", 100) and not controller.menuItemCooldown) {
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EXPLOREPAUSE");
					controller.menuItemCooldown = true;
				}
				return false;
			}
			if (type == "DEBUGEXPLORE") {
				Event("userInput", "DEBUGWALKING", { }).run(*&gameEngine);
				Event("userInput", "DEBUGUSERINPUT", { }).run(*&gameEngine);
				return Event("Explore", "EXPLORE", {}).run(*&gameEngine);
			}
			if (type == "SHOWFPS") {
				long total = 0;
				long average = 0;
				if (graphics.RenderSpeedHistory.size() > 0) {
					for (auto & time : graphics.RenderSpeedHistory.internalList) {
						long current = time.count();
						total += current;
					}
					average = total / graphics.RenderSpeedHistory.size();
				}
				string textUniqueID = "FPSCOUNTER";
				int layer = 10;
				if (!graphics.doesThisTextAlreadyExist(textUniqueID)) {
					graphics.addText(new Graphics::Text(*&graphics, L"", "Centaur_25", { 2,95 }, "TOPLEFT", { 100,100 }, graphics.Colours["WHITE"], graphics.Colours["BLACK"], textUniqueID, false, {},0), layer);
				}
				Graphics::Text* theText = graphics.accessTextViaUniqueID(textUniqueID);
				string message = "Average Frame Render Time: ";
				message += to_string(average);
				message += " FPS: ";
				if (average == 0) {
					average = 1;
				}
				message += to_string(1000 / average);
				theText->resetMessage(*&graphics, StringToWString(message));
				return false;
			}
			if (type == "DIALOGUE") {
				string cutscene = data["cutscene"];
				string speakerID = data["speaker"];
				if (speakerID.find("$PLAYER2==") != -1) {
					speakerID = SReplace(speakerID, "$PLAYER2==", "");
				}
				if (speakerID.find("$ASYNC$") != -1) {
					speakerID = SReplace(speakerID, "$ASYNC$", "");
					if (gameEngine.activeProcedure.eventList.at(1).name != "PostCutscene") {
						bool finishedNextEvent = gameEngine.activeProcedure.eventList.at(1).run(*&gameEngine);
						if (finishedNextEvent) {
							if (gameEngine.activeProcedure.eventList.at(1).type != "DIALOGUE") {
								gameEngine.activeProcedure.eventList.remove_at(1);
								return false;
							}
							graphics.tearDownSpecifiedText("speakerDialogueText");
								return true;
					}
					}
				}
				string displayName = speakerID;
				string line = name;
				string direct = data["direct"]; // 1 = use string here 0 = get string from Strings.h
				if (direct != "1") {
					line = gameEngine.language + "_" + cutscene + "_" + line + " " + data["speaker"];
				}
				bool hideSpeakerName = speakerID.find("?") != -1;
				if (hideSpeakerName) {
					speakerID = SReplace(speakerID, "?", "");
					speakerID = SReplace(speakerID, "$DIRECT$", "");
					displayName = "???";
				}

				if (speakerID == "PLAYER" or speakerID == "PLAYER2") {
					if (speakerID == "PLAYER") {
						speakerID = saveContainer.getCurrentMainCharacter();
						displayName = gameEngine.language + "_NPCNames_" + speakerID;
					}
					if (speakerID == "PLAYER2") {
						speakerID = saveContainer.getNamesOfTwoPeopleForScene().second;
						displayName = gameEngine.language + "_NPCNames_" + speakerID;
					}
				}
				else {
					speakerID = SReplace(speakerID, "$DIRECT$", "");
					displayName = SReplace(displayName, "$DIRECT$", "");
					displayName = gameEngine.language + "_NPCNames_" + displayName;
				}
				string speakerImageID = imageLookup.getSequenceAsString(speakerID, "SPEAKER");
				if (!graphics.doesThisImageAlreadyExist("TEXTBOX")) {
					Event("Load TextBox", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("sources", to_string(TEXTBOX)),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "90"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", to_string(imageLookup.layerDefaults["UI"] + 1)),
				pair<string, string>("uniqueID", "TEXTBOX"), }))).run(*&gameEngine);
					Event("Load Speaker", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", speakerImageID),
					pair<string, string>("x", "0"),
					pair<string, string>("y", "65"),
					pair<string, string>("anchor", "TOPLEFT"),
					pair<string, string>("opacity", "1.0"),
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["UI"] + 2)),
					pair<string, string>("uniqueID", "SPEAKER"), }))).run(*&gameEngine);
				}
				Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("message",displayName),
					pair<string, string>("direct", "0"),
					pair<string, string>("format", "HighTowerText_40"),
					pair<string, string>("anchorStyle", "TOPLEFT"),
					pair<string, string>("x", "18"),
					pair<string, string>("y", "76"),
					pair<string, string>("w", "50"),
					pair<string, string>("h", "50"),
					pair<string, string>("colour", "BLACK"),
					pair<string, string>("shadowColour", "DARKBROWN"),
					pair<string, string>("layer",  to_string(imageLookup.layerDefaults["UI"] + 3)),
					pair<string, string>("uniqueID", "speakerNameText"),
					}))).run(*&gameEngine);
				Graphics::Image * speaker = graphics.accessImageViaUniqueID("SPEAKER");
				if (!speaker->sources.contains(stoi(speakerImageID))) {
					speaker->resetSources(*&graphics, { stoi(speakerImageID) });
				}
				string width = "78";
				string x = "20";
				if (speakerID == "EMPTY") {
					x = "12";
				}
				bool finishedWriting = Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("message",line),
				pair<string, string>("animateExisting","0"),
				pair<string, string>("format", "Centaur_25"),
				pair<string, string>("anchorStyle", "TOPLEFT"),
				pair<string, string>("x", x),
				pair<string, string>("y", "83"),
				pair<string, string>("w", width),
				pair<string, string>("h", "33"),
				pair<string, string>("direct",direct),
				pair<string, string>("colour", "BLACK"),
				pair<string, string>("shadowColour", "DARKBROWN"),
				pair<string, string>("layer", to_string(imageLookup.layerDefaults["UI"] + 3)),
				pair<string, string>("uniqueID", "speakerDialogueText"),
				pair<string, string>("animated", "TRUE"),
				pair<string, string>("styles", "TYPEWRITER,PARCHMENT"),
					}))).run(*&gameEngine);
				bool userInput = controller.haveOneOfTheseBeenPressed({ VK_SPACE });
				if (Args.get("mode") == "DEBUG" and controller.haveOneOfTheseBeenPressed({ VK_F2 }) and CLOCK.hasEnoughTimePassed("DebugFlag", 100)) {
					if (gameEngine.stateFlags["FASTDIALOGUE"] == "1") {
						gameEngine.stateFlags["FASTDIALOGUE"] = "0";
					}
					else {
						gameEngine.stateFlags["FASTDIALOGUE"] = "1";
					}
				}
				if (Args.get("mode") == "DEBUG" and userInput and gameEngine.stateFlags["FASTDIALOGUE"] == "1") {
					return true;
				}
				if (finishedWriting and userInput and CLOCK.hasEnoughTimePassed("DialogueWait", 100)) {
					graphics.tearDownSpecifiedText("speakerDialogueText");
					return true;
				}
				return false;
			}
			if (type == "TEARDOWNDIALOGUE") {
				Event("TeardownImage", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", "TEXTBOX"), }))).run(*&gameEngine);
				Event("TeardownImage", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", "SPEAKER"), }))).run(*&gameEngine);
				Event("TeardownText", "TEARDOWNTEXT", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", "speakerNameText"), }))).run(*&gameEngine);
				Event("TeardownText", "TEARDOWNTEXT", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", "speakerDialogueText"), }))).run(*&gameEngine);
				CLOCK.startClock("DialogueEnded");
				return true;
			}
			if (type == "DYNAMICBUTTONSFORSKILLTREE") {
				string who = gameEngine.stateFlags["PARTYEDITSELECTED"];
				string baseMenuName = data["menuName"];
				string whichMenu = "SKILLTREEEDIT" + gameEngine.storedMenus[baseMenuName].data["SKILLTREEEDIT"];
				Combat::Combatant theCombatant = combat.loadPartyMemberAsCombatant(who);
				List<string> toDraw = theCombatant.getNamesOfAllUnusedSkillTrees(*&combat);
				pair<float, float> startPosition = { 45, 8 };
				if (gameEngine.storedMenus[baseMenuName].data["SKILLTREEEDIT"] == "2") {
					startPosition.first += 30;
				}
				float gap = 4;
				List<Menu::Button> dynamicButtons;
				dynamicButtons.push_back(Menu::smallButton("CANCELSKILLTREECHOICE", "GUI_CANCELSKILLTREECHOICE", startPosition));
				for (int x = 0; x < toDraw.size(); x++) {
					dynamicButtons.push_back(Menu::smallButton(toDraw.at(x), "Skill Tree Names_" + toDraw.at(x), { startPosition.first, startPosition.second + gap * (x + 1) }));
				}
				gameEngine.storedMenus[whichMenu].buttonReplace(dynamicButtons);
			}
			if (type == "LOADMENU") {
				controller.resetMouseClickPosition();
				CLOCK.startClock("MENUINPUTDELAY");
				string menuName = data["uniqueID"];
				string layer = to_string(imageLookup.layerDefaults["BUTTONS"]);
				if (data.getKeys().contains("layer")) {
					layer = data["layer"];
				}
				if (gameEngine.storedMenus[menuName].data.hasKey("SKILLTREEEDIT")) {
					Event("Load", "DYNAMICBUTTONSFORSKILLTREE", Map<string, string>({
						pair<string, string>("menuName", menuName),
						})).run(*&gameEngine);
				}
				if (menuName == "CODEX1") {
					gameEngine.storedMenus[menuName].buttonReplace(Menu::getCodexButtons(gameEngine.stateFlags["CODEXBOOK"], gameEngine.stateFlags["CODEXPAGE"]));
				}
				Menu menu = gameEngine.storedMenus[menuName];
				List<Menu::Button> toLoad = menu.getButtonsToLoad();
				for (auto button : toLoad.internalList) {
					Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
						pair<string, string>("uniqueID", menuName),
						pair<string, string>("which", button.uniqueID),
						pair<string, string>("layer", layer),
						pair<string, string>("format", button.extras["format"]),
						})).run(*&gameEngine);
				}
				if (menu.data.hasKey("AUDIOMANAGEMENT")) {
					pair<float,float> audioManagementStartPos = { 50,10 };
					float audioManagementHeightDiff = 10;
					for (auto node : audio.volumes.getKeys().internalList) {
						float sliderPosition = audio.convertVolumeToAudioSliderPosition(audio.volumes[node]);
						Event("LoadThisAudioImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(VOLUMESLIDERBASE)),
							pair<string, string>("x", to_string(audioManagementStartPos.first)),
							pair<string, string>("y", to_string(audioManagementStartPos.second)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("layer",to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("scale", "1.0"),
							pair<string, string>("uniqueID", node + "_IMAGE"),
							})).run(*&gameEngine);
						Event("LoadThisAudioImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(AUDIOKNOB)),
							pair<string, string>("x", to_string(sliderPosition)),
							pair<string, string>("y", to_string(audioManagementStartPos.second-2)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("layer",to_string(imageLookup.layerDefaults["BUTTONS"]+1)),
							pair<string, string>("scale", "1.0"),
							pair<string, string>("uniqueID", node + "_KNOB_IMAGE"),
							})).run(*&gameEngine);
						Event("LoadThisAudioText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",gameEngine.language + "_GUI_" + node),
						pair<string, string>("format", "Centaur_25"),
						pair<string, string>("anchorStyle", "CENTRE"),
						pair<string, string>("x", to_string(audioManagementStartPos.first)),
						pair<string, string>("y", to_string(audioManagementStartPos.second + 1.4f)),
						pair<string, string>("w", "50"),
						pair<string, string>("h",  "0"),
						pair<string, string>("colour", "WHITE"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer",  to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", node + "_TEXT"),
							})).run(*&gameEngine);
						audioManagementStartPos.second += audioManagementHeightDiff;
					}
				}
				if (menu.data.hasKey("PARTYSHUFFLE")) {
					List<string> party = saveContainer.current.party;
					List<pair<float, float>> positions = Menu::getPlayerCardPositions();
					for (int x = 0; x < min(party.size(), saveContainer.partyLimit); x++) {
						Event("LoadThisCharacter'sSkillbar", "LOADSKILLBARHERE", List <pair<string, string>>({
							pair<string,string>("who", party.at(x)),
							pair<string,string>("x", to_string(positions.at(x).first)),
							pair<string,string>("y", to_string(positions.at(x).second + 30)),
							pair<string,string>("scale", "0.25"),
							})).run(*&gameEngine);
						Event("LoadThisCharacter'sCard", "LOADIMAGE", List <pair<string, string>>({
							pair<string, string>("sources", to_string(imageLookup.animationFrames[party.at(x)]["CARD"].front())),
							pair<string, string>("x", to_string(positions.at(x).first)),
							pair<string, string>("y", to_string(positions.at(x).second)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", party.at(x) + "_CARD"),
							})).run(*&gameEngine);
						Event("PrintoutPlayer", "SETUPPLAYERREADOUT", List<pair<string,string>>({
							pair<string, string>("who", party.at(x)),
							pair<string, string>("x", to_string(positions.at(x).first-8)),
							pair<string, string>("y", to_string(positions.at(x).second + 25)),
						})).run(*&gameEngine);
					}
				}
				if (menu.data.hasKey("PARTYREFORM")) {
					Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
						pair<string, string>("offsetX", "32"),
						pair<string, string>("offsetY", "25"),
						pair<string, string>("what", "PARTY"),
						pair<string, string>("scale", "0.25"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						})).run(*&gameEngine);
					Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
						pair<string, string>("offsetX", "67"),
						pair<string, string>("offsetY", "25"),
						pair<string, string>("what", "RESERVES"),
						pair<string, string>("scale", "0.25"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						})).run(*&gameEngine);
				}
				if (menu.data.hasKey("SELECTCHARACTERTOEDIT")) {
					gameEngine.stateFlags["PARTYEDITSELECTED"] = saveContainer.getCurrentMainCharacter();
					Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
						pair<string, string>("offsetX", menu.data["CHARACTERTOEDITOFFSETX"]),
						pair<string, string>("offsetY", menu.data["CHARACTERTOEDITOFFSETY"]),
						pair<string, string>("what", "EVERYONE"),
						pair<string, string>("scale", menu.data["CHARACTERTOEDITSCALE"]),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("menu", menuName),
						})).run(*&gameEngine);
					Event("LoadPartyGrid", "SELECTCHARACTERTOEDIT", Map<string, string>({
						pair<string, string>("offsetX", menu.data["CHARACTERTOEDITOFFSETX"]),
						pair<string, string>("offsetY", menu.data["CHARACTERTOEDITOFFSETY"]),
						pair<string, string>("what", "EVERYONE"),
						pair<string, string>("byForce", "1"),
						pair<string, string>("menu", menuName),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						})).run(*&gameEngine);
					if (menuName == "EQUIPMENTMANAGEMENT") {
						Event("Autoselect", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "Weapon"),
							})).run(*&gameEngine);
						return false;
					}
				}	
				if (menu.data.hasKey("MERCHANT")) {
					string merchantID = data["whichMerchant"];
					Event("AddMerchantButtons", "ADDMERCHANTITEMBUTTONS", Map<string, string>({
						pair<string,string>({"whichMerchant", merchantID})
						})).run(*&gameEngine);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->resetMessage(*&graphics,Graphics::Text::commonTextReplacements(gameEngine.language, merchants.merchantDefinitions[merchantID].getCorrectDialogue(gameEngine.language, menu.data["MERCHANT"])));
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter(*&graphics);
				}
				if (menu.uniqueID == "MERCHANTCONFIRM") {
					string mode = gameEngine.storedMenus["MERCHANT"].data["MERCHANT"];
					string item = gameEngine.stateFlags["itemOfInterest"];
					wstring dialogue = strings[gameEngine.language]["GUI"]["TradeConfirm"];
					dialogue = WSReplace(dialogue, L"$ITEM$", strings[gameEngine.language]["Item Names"][item]);
					dialogue = WSReplace(dialogue, L"$MODE$", strings[gameEngine.language]["GUI"][mode]);
					int price = combat.equipmentDefinitions[item].price;
					dialogue = WSReplace(dialogue, L"$PRICE$", to_wstring(price));
					int currentlyInInventory = saveContainer.howManyOfThisItemInInventory(item);

					if (currentlyInInventory > 0) {
						wstring extra = L"\n\n" + strings[gameEngine.language]["GUI"]["TradeConfirmPlus_" + mode];
						extra = WSReplace(extra, L"$AMOUNT$", to_wstring(currentlyInInventory));
						dialogue += extra;
					}

					graphics.accessTextViaUniqueID("TradeConfirm_TEXT")->resetMessage(*&graphics, dialogue);
				}
				if (menu.uniqueID == "YOUFOUNDANITEM") {
					string foundItem = data["contents"];
					wstring toPrint = strings[gameEngine.language]["GUI"]["You found other"];
					wstring itemName = strings[gameEngine.language]["Item Names"][foundItem];
					Combat::Equipment def = combat.equipmentDefinitions[foundItem];
					string type = def.category;
					if (type == "Weapon") {
						toPrint = strings[gameEngine.language]["GUI"]["You found weapon"];
					}
					if (type == "Armour") {
						toPrint = strings[gameEngine.language]["GUI"]["You found armour"];
					}
					if (type == "Accessory") {
						toPrint = strings[gameEngine.language]["GUI"]["You found accessory"];
					}
					if (type == "Tome") {
						toPrint = strings[gameEngine.language]["GUI"]["You found tome"];
					}
					toPrint += L"\n\n";
					toPrint += def.getColourForPrint();
					toPrint += WSReplace(itemName, L" ", def.getColourForPrint());
					toPrint += def.getColourForPrint();
					Graphics::Text * toUpdate = graphics.accessTextViaUniqueID("YouFoundTextBox_TEXT");
					toUpdate->resetMessage(*&graphics, toPrint);
				}
				return true;
			}
			if (type == "LOADABUTTON") {
				string menuName = data["uniqueID"];
				Menu menu = gameEngine.storedMenus[menuName];
				List<Menu::Button> toLoad = menu.getButtonsToLoad();
				bool justLoadText = data["justLoadText"] == "1";
				bool justLoadImage = data["justLoadImage"] == "1";
				Menu::Button button = menu.buttons[data["which"]];
				string layer = data["layer"];
				if (button.extras.hasKey("layer")) {
					layer = button.extras["layer"];
				}
				if (!button.visible) { return true; }
				if (button.sources.size() > 0 and !justLoadText) { // no sources = no images needed
					Event("LoadThisButtonImage", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", button.getSources()),
						pair<string, string>("x", to_string(button.position.first)),
						pair<string, string>("y", to_string(button.position.second)),
						pair<string, string>("anchor", button.anchorStyle),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", layer),
						pair<string, string>("scale", "1.0"),
						pair<string, string>("uniqueID", button.imageID),
						})).run(*&gameEngine);
				}
				if (justLoadImage) { return true; }
				string format = "Centaur_25";
				if (data.getKeys().contains("format") and data["format"] != "") {
					format = data["format"];
				}
				string colour = "WHITE";
				if (button.extras.getKeys().contains("colour")) {
					colour = button.extras["colour"];
				}
				int textOffsetX = 0;
				int textOffsetY = 0;
				int textOffsetX2 = 0;
				if (button.extras.getKeys().contains("textOffsetX")) {
					textOffsetX = stoi(button.extras["textOffsetX"]);
					textOffsetY = stoi(button.extras["textOffsetY"]);
					textOffsetX2 = stoi(button.extras["textOffsetX2"]);
				}
				string prefix = gameEngine.language + "_";
				if (button.extras.hasKey("don'tAddLanguage")) {
					prefix = "";
				}
				Event("LoadThisButtonText", "DRAWTEXT", Map<string, string>({
					pair<string, string>("message",prefix + button.buttonContent),
					pair<string, string>("format", format),
					pair<string, string>("anchorStyle", button.anchorStyle),
					pair<string, string>("x", to_string(button.position.first + textOffsetX)),
					pair<string, string>("y", to_string(button.position.second + textOffsetY)),
					pair<string, string>("w", to_string(button.width - textOffsetX2)),
					pair<string, string>("h",  to_string(button.height)),
					pair<string, string>("colour", colour),
					pair<string, string>("shadowColour", "DARKBROWN"),
					pair<string, string>("direct",  button.extras["direct"]),
					pair<string, string>("layer",  layer),
					pair<string, string>("uniqueID", button.textID),
					pair<string, string>("animated", button.extras["animated"]),
					pair<string, string>("getMessageFromMoney", button.extras["getMessageFromMoney"]),
					}) + button.extras).run(*&gameEngine);
				return true;
			}
			if (type == "LOADXINAGRID") {
				float offsetX = stof(data["offsetX"]);
				float offsetY = stof(data["offsetY"]);
				int yspace = 10;
				string what = data["what"];
				string imageIDSuffix = "";
				Map<string, int> toDraw;
				List<string> toDrawOrder;
				float scale = stof(data["scale"]);
				float xSpaceFactor = 20;
				string layer = data["layer"];

				if (what == "PARTY") {
					for (auto x : saveContainer.current.party) {
						toDraw[x] = imageLookup.animationFrames[x]["CARD"].front();
						toDrawOrder.push_back(x);
					}
					imageIDSuffix = "_CARD";
				}
				if (what == "RESERVES") {
					for (auto x : saveContainer.getCharactersInReserve().internalList) {
						toDraw[x] = imageLookup.animationFrames[x]["CARD"].front();
						toDrawOrder.push_back(x);
					}
					imageIDSuffix = "_CARD";
					yspace = 12;
				}
				if (what == "EVERYONE") {
					yspace = 12;
					string selected = saveContainer.getCurrentMainCharacter();
					if (gameEngine.stateFlags["PARTYEDITSELECTED"] != "") {
						selected = gameEngine.stateFlags["PARTYEDITSELECTED"];
					}
					List<string> characters = saveContainer.current.allCharacters;
					if (data["menu"] == "NEWGAME") {
						characters = saveContainer.getAllStartingCharacters();
					}
					for (auto x : characters.internalList) {
						toDraw[x] = imageLookup.animationFrames[x]["CARD"].front();
						if (selected == x) {
							toDraw[x] = imageLookup.animationFrames[x]["CARD_SELECTED"].front();
						}
						toDrawOrder.push_back(x);
					}
					imageIDSuffix = "_CARD";
				}
				if (what == "SKILLS") {
					string skillTreeName = data["skillTreeName"];
					string who = data["who"];
					Combat::Combatant theCharacter = combat.loadPartyMemberAsCombatant(who);

					List<string> knownSkillNames = saveContainer.current.knownSkills[who];
					knownSkillNames.internalList.sort();
					for (auto knownSkill : knownSkillNames.internalList) {
						Combat::Skill theSkill = combat.skillDefinitions[knownSkill];
						if (theSkill.skillTree == skillTreeName) {
							toDraw[knownSkill] = theSkill.imageSource;
							toDrawOrder.push_back(knownSkill);
						}
					}
					imageIDSuffix = "_" + who + "_SKILLSELECTIONGRID";
					xSpaceFactor *= 0.5;
				}
				if (what == "SKILLBORDERS") {
					string skillTreeName = data["skillTreeName"];
					string who = data["who"];
					Combat::Combatant theCharacter = combat.loadPartyMemberAsCombatant(who);

					List<string> knownSkillNames = saveContainer.current.knownSkills[who];
					knownSkillNames.internalList.sort();
					for (auto knownSkill : knownSkillNames.internalList) {
						Combat::Skill theSkill = combat.skillDefinitions[knownSkill];
						if (theSkill.skillTree == skillTreeName) {
							toDraw[knownSkill] = theSkill.getBorderSource();
							toDrawOrder.push_back(knownSkill);
						}
					}
					imageIDSuffix = "_" + who + "_SKILLSELECTIONBORDER";
					xSpaceFactor *= 0.5;
				}
				Map<int, List<pair<float, float>>> positions = Menu::getPlayerCardReformGridPositions(offsetX, offsetY, scale*xSpaceFactor, yspace);
				int rowSize = positions[0].size();
				int currentRow = 0;
				for (int x = 0; x < toDrawOrder.size(); x++) {
					if (x != 0 and x % rowSize == 0) {
						currentRow += 1;
						if (!positions.getKeys().contains(currentRow)) {
							throw exception("There aren't enough rows to draw all the items.");
						}
					}
					string current = toDrawOrder.at(x);
					int currentValue = toDraw[current];
					if (data["moveExisting"] == "1") {
						graphics.accessImageViaUniqueID(current + imageIDSuffix)->positionAsPercentage = positions[currentRow].at(x - (rowSize * currentRow));
					}
					else {
					Event("LoadThisCharacter'sCard", "LOADIMAGE", List <pair<string, string>>({
						pair<string, string>("sources", to_string(currentValue)),
						pair<string, string>("x", to_string(positions[currentRow].at(x - (rowSize * currentRow)).first)),
						pair<string, string>("y", to_string(positions[currentRow].at(x - (rowSize * currentRow)).second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", to_string(scale)),
						pair<string, string>("layer", layer),
						pair<string, string>("uniqueID", current + imageIDSuffix),
						})).run(*&gameEngine);
					}
				}
				return true;
			}
			if (type == "SETUPPLAYERREADOUT") {
				string who = data["who"];
				pair<float, float> Where = { stof(data["x"]), stof(data["y"]) };
				Event("SetUpPlayerReadoutText", "DRAWTEXT", List<pair<string, string>>({
						pair<string, string>("message", who),
						pair<string, string>("getStringFromCombatant", "printout"),
						pair<string, string>("direct", "1"),
						pair<string, string>("format", "GoudyMedieval_17"),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", to_string(Where.first)),
						pair<string, string>("y", to_string(Where.second)),
						pair<string, string>("w", "20"),
						pair<string, string>("h",  "50"),
						pair<string, string>("colour", "WHITE"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer",  to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", who + "_PRINTOUTTEXT"),
					})).run(*&gameEngine);
				Event("SetUpPlayerPointsLeftText", "DRAWTEXT", List<pair<string, string>>({
						pair<string, string>("message", who),
						pair<string, string>("getStringFromCombatant", "pointsLeft"),
						pair<string, string>("direct", "1"),
						pair<string, string>("format", "GoudyMedieval_12"),
						pair<string, string>("anchorStyle", "CENTRE"),
						pair<string, string>("x", to_string(Where.first + 7)),
						pair<string, string>("y", to_string(Where.second + 47)),
						pair<string, string>("w", "20"),
						pair<string, string>("h",  "50"),
						pair<string, string>("colour", "WHITE"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer",  to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", who + "_POINTSLEFTTEXT"),
					})).run(*&gameEngine);
				float buttonYGap = 2.55;
				float currentYGap = 11.5;
				for (auto attribute : combat.AttributesInOrder.internalList) {
					Event("LoadPlusButton", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(PLUSBUTTON_DEFAULT) + " " +to_string(PLUSBUTTON_HOVERED) + " " + to_string(PLUSBUTTON_PRESSED)),
							pair<string, string>("x", to_string(Where.first + 15.5)),
							pair<string, string>("y", to_string(Where.second + currentYGap)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("layer",to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("scale", "1.0"),
							pair<string, string>("uniqueID", who + "_" + attribute + "_PLUSBUTTONIMAGE"),
						})).run(*&gameEngine);
					Event("LoadMinusButton", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(MINUSBUTTON_DEFAULT) + " " + to_string(MINUSBUTTON_HOVERED) + " " + to_string(MINUSBUTTON_PRESSED)),
							pair<string, string>("x", to_string(Where.first + 14)),
							pair<string, string>("y", to_string(Where.second + currentYGap)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("layer",to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("scale", "1.0"),
							pair<string, string>("uniqueID", who + "_" + attribute + "_MINUSBUTTONIMAGE"),
						})).run(*&gameEngine);
					currentYGap += buttonYGap;
				}
				return true;
			}
			if (type == "TEARDOWNPLAYERREADOUT") {
				string who = data["who"];
				graphics.tearDownSpecifiedText(who + "_PRINTOUTTEXT");
				graphics.tearDownSpecifiedText(who + "_POINTSLEFTTEXT");
				graphics.tearDownSpecifiedImage(who + "_VITALITY_PLUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_PIETY_PLUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_STRENGTH_PLUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_INTELLIGENCE_PLUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_AGILITY_PLUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_LUCK_PLUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_VITALITY_MINUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_PIETY_MINUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_STRENGTH_MINUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_INTELLIGENCE_MINUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_AGILITY_MINUSBUTTONIMAGE");
				graphics.tearDownSpecifiedImage(who + "_LUCK_MINUSBUTTONIMAGE");
				return true;
			}
			if (type == "SELECTCHARACTERTOEDIT") {
				string menuName = data["menu"];
				bool byForce = data["byForce"] == "1";
				bool justRemoveExisting = data["justRemoveExisting"] == "1";
				List<Graphics::Image*> characterCards;
				List<string> characters; characters.internalList = saveContainer.current.allCharacters;
				if (menuName == "NEWGAME") {
					characters = saveContainer.getAllStartingCharacters();
				}
				for (auto who : characters.internalList) {
					if (graphics.doesThisImageAlreadyExist(who + "_CARD")) {
						characterCards.push_back(graphics.accessImageViaUniqueID(who + "_CARD"));
					}
				}
				string who = "";
				string previousSelected = "";
				Map<string, string> locs = Menu::getLocAndScaleOfSkillBarEdit();
				string scale = locs["scale"];
				string x = locs["x"];
				string y = locs["y"];
				bool needToDraw = false;
				bool needToRemoveExisting = false;
				if (justRemoveExisting) {
					needToRemoveExisting = true;
					previousSelected = gameEngine.stateFlags["PARTYEDITSELECTED"];
				}
				if (byForce) {
					needToRemoveExisting = true;
					needToDraw = true;
					who = gameEngine.stateFlags["PARTYEDITSELECTED"];
					previousSelected = who;
				}
				for (Graphics::Image* image : characterCards.internalList) {
					if (image->hasThisBeenClickedOn(graphics, controller.mouseClickPosition)) {
						who = split(image->unique_ID, "_").at(0);
						previousSelected = gameEngine.stateFlags["PARTYEDITSELECTED"];
						if (previousSelected != who) {
							needToDraw = true;
							graphics.accessImageViaUniqueID(previousSelected + "_CARD")->resetSources(*&graphics, imageLookup.getSequence(previousSelected, "CARD"));
							graphics.accessImageViaUniqueID(who + "_CARD")->resetSources(*&graphics, imageLookup.getSequence(who, "CARD_SELECTED"));
							if (CLOCK.hasEnoughTimePassed("characterSelectionClick", 100)) {
								Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
									pair<string,string>("audio",to_string(CHARACTERSELECT_WAV)),
									pair<string,string>("direct","1"),
									})).run(*&gameEngine);
							}
							if (previousSelected != "") {
								needToRemoveExisting = true;
								break;}}}}

				if (needToRemoveExisting) {
					Event("RemoveExistingIfNecessary", "TEARDOWNTHISSKILLBAR", Map<string, string>({ pair<string, string>("who", previousSelected) })).run(*&gameEngine);
					Map<string, string> equippedSkillTrees; equippedSkillTrees.internalMap = saveContainer.current.equippedSkillTrees[previousSelected];
					for (auto knownSkill : saveContainer.current.knownSkills[previousSelected]) {
						string toTeardown = knownSkill + "_" + previousSelected + "_SKILLSELECTIONGRID";
						string toTearDown2 = knownSkill + "_" + previousSelected + "_SKILLSELECTIONBORDER";
						Event("Teardown", "TEARDOWNIMAGE", Map<string, string>({ pair<string, string>("uniqueID", toTeardown) })).run(*&gameEngine);
						Event("Teardown", "TEARDOWNIMAGE", Map<string, string>({ pair<string, string>("uniqueID", toTearDown2) })).run(*&gameEngine);
					}
					controller.resetMouseClickPosition();
				}
				
				if (needToDraw and menuName == "NEWGAME") {
					gameEngine.stateFlags["PARTYEDITSELECTED"] = who;
					graphics.accessTextViaUniqueID("startMenu2")->resetMessage(*&graphics, strings[gameEngine.language]["NPCNames"][who]);
					graphics.accessTextViaUniqueID("newGameCharacterDescription")->resetMessage(*&graphics, strings[gameEngine.language]["NewGameDescriptions"][who]);
					graphics.accessTextViaUniqueID("startMenu2")->startTypewriter(*&graphics);
					graphics.accessTextViaUniqueID("newGameCharacterDescription")->startTypewriter(*&graphics);
				}

				if (needToDraw and menuName == "EQUIPMENTMANAGEMENT") {
					Event("LoadText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("x","2"),
						pair<string, string>("y","50"),
						pair<string, string>("uniqueID", "EQUIPMENTEXPLAIN"),
						pair<string, string>("message", ""),
						pair<string, string>("direct", "1"),
						pair<string, string>("w", "30"),
						pair<string, string>("h", "50"),
						pair<string, string>("colour", "WHITE"),
						pair<string, string>("format", "GoudyMedieval_14"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer",  to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						})).run(*&gameEngine);
					Graphics::Text* theText = graphics.accessTextViaUniqueID("EQUIPMENTEXPLAIN");
					Combat::Combatant theCombatant = combat.loadPartyMemberAsCombatant(who);
					theText->resetMessage(*&graphics, theCombatant.getEquipmentPrintout(gameEngine.language, *&combat));
					if (gameEngine.stateFlags["PARTYEDITSELECTED"] != "") {
						// someone else was selected so do teardown
						List<Menu::Button> toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons.getValues();
						for (auto button : toHandle.internalList) {
							Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
								pair<string, string>("imageID", button.imageID),
								pair<string, string>("textID", button.textID),
								})).run(*&gameEngine);
						}
						if (gameEngine.stateFlags["WEPTYPESELECTED"] == "") {
							gameEngine.stateFlags["WEPTYPESELECTED"] = "Weapon";
						}
						Event("AddButtons", "ADDEQUIPBUTTONSTOMENU", Map<string, string>({
						pair<string, string>("category", gameEngine.stateFlags["WEPTYPESELECTED"]),
						pair<string, string>("who", who),
							})).run(*&gameEngine);

						toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons.getValues();
						for (auto button : toHandle.internalList) {
							Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
								pair<string, string>("uniqueID", menuName),
								pair<string, string>("which", button.uniqueID),
								pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("format", button.extras["format"]),
								})).run(*&gameEngine);
						}
					}
					gameEngine.stateFlags["PARTYEDITSELECTED"] = who;
				}

				if (needToDraw and menuName == "SKILLMANAGEMENT") {
					Map<string, string> equippedSkillTrees; equippedSkillTrees.internalMap = saveContainer.current.equippedSkillTrees[who];
					Map<string, string> xPositions = Menu::getSkillGridPositions();
					for (auto whichTree : equippedSkillTrees.getKeys().internalList) {
						Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
							pair<string, string>("offsetX", xPositions[whichTree]),
							pair<string, string>("offsetY", "20"),
							pair<string, string>("scale", "0.5"),
							pair<string, string>("what", "SKILLS"),
							pair<string, string>("skillTreeName",equippedSkillTrees[whichTree]),
							pair<string, string>("who", who),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							})).run(*&gameEngine);
						Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
							pair<string, string>("offsetX", xPositions[whichTree]),
							pair<string, string>("offsetY", "20"),
							pair<string, string>("what", "SKILLBORDERS"),
							pair<string, string>("skillTreeName",equippedSkillTrees[whichTree]),
							pair<string, string>("who", who),
							pair<string, string>("scale", "0.5"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"] + 1)),
							})).run(*&gameEngine);
					}
					Event("LoadSkillBar", "LOADSKILLBARHERE", Map<string, string>({
						pair<string, string>("who", who),
						pair<string, string>("scale", scale),
						pair<string, string>("x", x),
						pair<string, string>("y", y),
						pair<string, string>("full", "1"),
						})).run(*&gameEngine);
					gameEngine.stateFlags["PARTYEDITSELECTED"] = who;
					Event("ChangeDropDownText", "RENAMEDROPDOWNMENUS", Map<string, string>({
						pair<string, string>("menu", data["menu"]),
						})).run(*&gameEngine);
					graphics.accessTextViaUniqueID("SKILLTREE1_TEXT")->resetMessage(*&graphics, combat.loadPartyMemberAsCombatant(who).getSkillTreesNamesForEditPrintout(gameEngine.language)["1"]);
					graphics.accessTextViaUniqueID("SKILLTREE2_TEXT")->resetMessage(*&graphics, combat.loadPartyMemberAsCombatant(who).getSkillTreesNamesForEditPrintout(gameEngine.language)["2"]);
					
return true;
				}

				return false;
			}
			if (type == "HANDLEMENU") {
				//Event("Debug", "DEBUGUSERINPUT", {}).run(*&gameEngine);
				if (graphics.CurrentCursor == "NONE") {
					graphics.changeCursor("DEFAULT");
				}
				string whichMenu = data["uniqueID"];
				List<Menu::Button> clickables = gameEngine.storedMenus[whichMenu].getButtonsToLoad();

				if (gameEngine.storedMenus[whichMenu].data.getKeys().contains("PARTYSHUFFLE")) {
					Event("HandleRearrange", "HANDLEPARTYREARRANGECLICKANDDRAG", { Map<string, string>({
						pair<string, string>("mode","shuffle"),
						pair<string, string>("scale", gameEngine.storedMenus[whichMenu].data["CHARACTERTOEDITSCALE"]),
						}),}).run(*&gameEngine);
				}
				if (gameEngine.storedMenus[whichMenu].data.getKeys().contains("PARTYREFORM")) {
					Event("HandleRearrange", "HANDLEPARTYREARRANGECLICKANDDRAG", { Map<string, string>({
						pair<string, string>("mode","reform"),
						pair<string, string>("scale", gameEngine.storedMenus[whichMenu].data["CHARACTERTOEDITSCALE"]),
						}), }).run(*&gameEngine);
				}
				if (gameEngine.storedMenus[whichMenu].data.hasKey("SKILLEXPLAIN") and gameEngine.storedMenus[whichMenu].data["SKILLEXPLAIN"] == "1") {
					Event("HandleSkillBarExplain", "HANDLESKILLEXPLAIN", Map<string, string>({
						pair<string, string>("x","5"),
						pair<string, string>("y","10"),
						pair<string, string>("scale","1.0"),
						})).run(*&gameEngine);
				}
				if (gameEngine.storedMenus[whichMenu].data.hasKey("SKILLEXPLAIN") and gameEngine.storedMenus[whichMenu].data["SKILLEXPLAIN"] == "2") {
					Event("HandleSkillBarExplain", "HANDLESKILLEXPLAIN", Map<string, string>({
						pair<string, string>("x","6"),
						pair<string, string>("y","50"),
						pair<string, string>("scale","1.0"),
						pair<string, string>("full","1"),
						})).run(*&gameEngine);
				}
				if (gameEngine.storedMenus[whichMenu].data.getKeys().contains("ATTPOINTMANAGE")) {
					List<string> party = saveContainer.current.party;
					for (int x = 0; x < min(party.size(), saveContainer.partyLimit); x++) {
						string who = party.at(x);
						for (auto attribute : combat.AttributesInOrder.internalList) {
							string plus_image_name = who + "_" + attribute + "_" + "PLUSBUTTONIMAGE";
							string minus_image_name = who + "_" + attribute + "_" + "MINUSBUTTONIMAGE";
							clickables.push_back(Menu::plusButton(plus_image_name));
							clickables.push_back(Menu::minusButton(minus_image_name));
						}
					}
				}
				if (gameEngine.storedMenus[whichMenu].data.getKeys().contains("SELECTCHARACTERTOEDIT")) {
					Event("CharacterSelection", "SELECTCHARACTERTOEDIT", Map<string, string>({
						pair<string, string>("menu",whichMenu),
						})).run(*&gameEngine);
				}
				if (gameEngine.storedMenus[whichMenu].data.getKeys().contains("MANAGESKILLS")) {
					Event("CharacterSelection", "HANDLESKILLBAREDIT", Map<string, string>({
							pair<string, string>("scale", "0.25"),
						})).run(*&gameEngine);
				}
				if (List<string>({"MERCHANTCONFIRM", "TRADEDENY","TRADEDENYFULL"}).contains(whichMenu)){
					// keep animating the dialogue even behind the confirmation buttons
					clickables.push_back(Menu::MerchantTalkingBox("MERCHANTDIALOGUE"));
				}
				bool anythingHovered = false;
				List<Menu::Button> hoveredOverItems;
				List<Menu::Button> clickedOnItems;
				List<string> popUpTexts;
				for (auto clickable : clickables.internalList) {
					if (clickable.extras.getKeys().contains("TYPEWRITER")) {
						Event("Animate", "DRAWTEXT", Map<string, string>({
							pair<string, string>("uniqueID", clickable.textID),
							pair<string, string>("animateExisting", "1"),
							})).run(*&gameEngine);
					}
					if (clickable.extras.getKeys().contains("hasHoverText")) {
						popUpTexts.push_back(clickable.extras["hoverTextName"]);
					}
					if (graphics.doesThisImageAlreadyExist(clickable.imageID)) {
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(clickable.imageID);
						bool hoveredOver = theImage->hasThisBeenClickedOn(*&graphics, controller.mouseMovePosition);
						bool clickedOn = theImage->hasThisBeenClickedOn(*&graphics, controller.mouseClickPosition);
						theImage->frame = 0;
						if (clickedOn) {
							clickedOnItems.push_back(clickable);
						}
						if (hoveredOver) {
							if (clickable.extras.getKeys().contains("hasHoverText")) {
								string textID = clickable.extras["hoverTextName"];
								if (graphics.doesThisTextAlreadyExist(textID)) {
									Event("RemoveHoverText", "TEARDOWNTEXT", Map<string, string>({ "uniqueID", textID })).run(*&gameEngine);
								}
							}
							anythingHovered = true;
							hoveredOverItems.push_back(clickable);
						}
					}
				}
				for (auto clickable : clickedOnItems.internalList) {
					Graphics::Image* theImage = graphics.accessImageViaUniqueID(clickable.imageID);
					theImage->frame = 2;
					Event("PlayHoverSound", "PLAYSFX", Map<string, string>({ pair<string,string>("audio",to_string(clickable.audioClick)),pair<string,string>("direct","1"), })).run(*&gameEngine);
					Event("ButtonLogic", "HANDLEBUTTON", Map<string, string>({ 
						pair<string, string>("uniqueID", clickable.uniqueID),
						pair<string, string>("menuName", data["uniqueID"]), 
						pair<string, string>("whichMerchant", data["whichMerchant"]),
						pair<string, string>("objectID", data["objectID"]),
						pair<string, string>("contents", data["contents"]),
						})).run(*&gameEngine);
					return false;
				}
				for (auto clickable : hoveredOverItems.internalList) {
					if (!clickable.clickable) { continue; }
					Graphics::Image* theImage = graphics.accessImageViaUniqueID(clickable.imageID);
					theImage->frame = 1;
					string hoveredOverItem = clickable.imageID;
					if (hoveredOverItem != controller.latestMenuItemHovered) {
						if (CLOCK.hasEnoughTimePassed("PlayASoundInTheMenu", 100)) {
							Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
							pair<string,string>("audio",to_string(clickable.audioHover)),
							pair<string,string>("direct","1"),
								})).run(*&gameEngine);
							CLOCK.startClock("PlayASoundInTheMenu");
						}
						controller.latestMenuItemHovered = clickable.imageID;
					}
					if (clickable.extras.getKeys().contains("hasHoverText")) {
						string textID = clickable.extras["hoverTextName"];
						string message = "";
						string direct = "1";
						if (clickable.extras["hoverTextContent"] == "equipmentToggleText") {
							message = gameEngine.language + "_GUI_" + textID;
							direct = "0";
						}
						if (clickable.extras["hoverTextContent"] == "explainEquipmentHover") {
							message = clickable.extras["itemName"];
							direct = "1";
						}
						Event("MakeHoverText", "DRAWTEXT", Map<string, string>({
							pair<string, string>("x",clickable.extras["hoverTextContentX"]),
							pair<string, string>("y",clickable.extras["hoverTextContentY"]),
							pair<string, string>("direct", direct),
							pair<string, string>("uniqueID", textID),
							pair<string, string>("message", message),
							pair<string, string>("hoverTextOneLine", clickable.extras["hoverTextOneLine"]),
							pair<string, string>("w", "100"),
							pair<string, string>("h", "50"),
							pair<string, string>("colour", "WHITE"),
							pair<string, string>("format", clickable.extras["hoverTextFormat"]),
							pair<string, string>("shadowColour", "DARKBROWN"),
							pair<string, string>("layer",  to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("anchorStyle", clickable.extras["hoverAnchorStyle"]),
							})).run(*&gameEngine);
					}
					if (clickable.scrollable and graphics.CurrentCursor == "DEFAULT") {
						graphics.changeCursor("SCROLLABLE");
					}
					if (clickable.scrollable and CLOCK.hasEnoughTimePassed("SCROLLCLOCK", 5) and controller.currentScrollWheelDelta != 0) {
						string whichScroll = clickable.extras["whichScroll"];
						int limit = stoi(gameEngine.storedMenus[whichMenu].data["scrollingLimit_" + whichScroll]);
						int current = gameEngine.storedMenus[whichMenu].currentScrolls[whichScroll];
						int amount = -1;
						if (controller.currentScrollWheelDelta > 0) {
							amount = 1;
						}
						if (limit < 0) { limit = 0; }
						int result = TChange(current, amount, 0, limit);
						controller.currentScrollWheelDelta = 0;
						gameEngine.storedMenus[whichMenu].currentScrolls[whichScroll] = result;
						Event("Change Merch Buttons", "ADDMERCHANTITEMBUTTONS", Map<string, string>({
						pair<string, string>("whichMerchant", data["whichMerchant"]),
						pair<string, string>("reset", "1"),
							})).run(*&gameEngine);
					}
					if (!clickable.scrollable and graphics.CurrentCursor == "SCROLLABLE") {
						graphics.changeCursor("DEFAULT");
					}
				}
				if (!anythingHovered) {
					if (graphics.CurrentCursor == "SCROLLABLE") {
						graphics.changeCursor("DEFAULT");
					}
					controller.latestMenuItemHovered = "";
					for (auto text : popUpTexts.internalList) {
						Event("RemoveHoverText", "TEARDOWNTEXT", Map<string, string>({ "uniqueID", text })).run(*&gameEngine);
					}
					if (whichMenu == "CODEX1") {
						bool anythingClose = false;
						for (auto button : gameEngine.storedMenus[whichMenu].buttons.getValues().internalList) {
							if (button.uniqueID.find("CODEXPAGE") != -1) {
								Graphics::Image* theImage = graphics.accessImageViaUniqueID(button.imageID);
								if (theImage != NULL and theImage->isTheCursorCloseToThis(*&graphics, controller.mouseMovePosition)) {
									anythingClose = true;
								}
							}
						}
						if (!anythingClose) {
							gameEngine.stateFlags["CODEXBOOK"] = "";
							Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "CODEX1")).run(*&gameEngine);
							Event("LoadAMenu", "LOADMENU", Map<string, string>({
								pair<string, string>("uniqueID", "CODEX1") })).run(*&gameEngine);
						}
					}
				}
				if ((whichMenu == "CODEX1" or CLOCK.hasEnoughTimePassed("MENUINPUTDELAY", 25)) and not controller.menuItemCooldown) {
					for (auto const& [key, value] : gameEngine.storedMenus[whichMenu].getKeyboardShortcutsForThisMenu().internalMap) {
						bool hasBeenPressed = false;
						if (key < 0) {
							if (key == -1) {	
								if (graphics.isThisInsideRect(controller.mouseClickPosition, graphics.getRectOfWholeScreen())) {
									hasBeenPressed = true;
								}
							}
						}
						if (key > 0) {
							hasBeenPressed = controller.hasThisBeenPressed(key);
						}

						if (hasBeenPressed) {
							controller.menuItemCooldown = true;
							Event("ButtonLogic", "HANDLEBUTTON", Map<string, string>({
								pair<string, string>("uniqueID", value),
								pair<string, string>("menuName", data["uniqueID"]),
								pair<string, string>("whichMerchant", data["whichMerchant"]),
								pair<string, string>("objectID", data["objectID"]),
								pair<string, string>("contents", data["contents"]),
								})).run(*&gameEngine);
						}
					}
				}
				if (gameEngine.storedMenus[whichMenu].data.hasKey("AUDIOMANAGEMENT")) {
					Event("clickAndDragAudio", "CLICKANDDRAG", Map<string, string>({ 
						pair<string, string>("objects", "SFXVolume_KNOB_IMAGE,AmbienceVolume_KNOB_IMAGE,MusicVolume_KNOB_IMAGE"),
						pair<string, string>("YLock", "1"),
						pair<string, string>("XMin", "40"),
						pair<string, string>("XMax", "60"),
						})).run(*&gameEngine);
					Event("clickAndDragAudio2", "HANDLEAUDIOCLICKANDDRAG", Map<string, string>({
						pair<string, string>("objects", "SFXVolume_KNOB_IMAGE,AmbienceVolume_KNOB_IMAGE,MusicVolume_KNOB_IMAGE"),
						})).run(*&gameEngine);
				}
				return false;
			}
			if (type == "TEARDOWNABUTTON") {
				Event("TearDown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", data["imageID"])).run(*&gameEngine);
				Event("TearDownText", "TEARDOWNTEXT", pair<string, string>("uniqueID", data["textID"])).run(*&gameEngine);
				return true;
			}
			if (type == "TEARDOWNMENU") {
				graphics.beingDragged.clear();
				graphics.recentlyFinishedBeingDragged.clear();
				controller.resetMouseClickPosition();
				graphics.changeCursor("DEFAULT");
				string menuName = data["uniqueID"];
				Menu menu = gameEngine.storedMenus[menuName];

				List<Menu::Button> toUnload = menu.getButtonsToLoad();
				for (auto button : toUnload.internalList) {
					Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
						pair<string, string>("imageID", button.imageID),
						pair<string, string>("textID", button.textID),
						})).run(*&gameEngine);
					if (button.extras.hasKey("hasHoverText")) {
						Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
							pair<string, string>("uniqueID", button.extras["hoverTextName"]),
							})).run(*&gameEngine);
					}
				}
				if (menu.data.hasKey("AUDIOMANAGEMENT")) {
					Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
							pair<string, string>("uniqueID", "AmbienceVolume_TEXT"),
						})).run(*&gameEngine);
					Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
						pair<string, string>("uniqueID", "MusicVolume_TEXT"),
						})).run(*&gameEngine);
					Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
							pair<string, string>("uniqueID", "SFXVolume_TEXT"),
						})).run(*&gameEngine);
					for (auto node : audio.volumes.getKeys().internalList) {
						Event("TearDown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", node + "_IMAGE")).run(*&gameEngine);
						Event("TearDown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", node + "_KNOB_IMAGE")).run(*&gameEngine);
					}
				}
				if (menu.data.hasKey("PARTYSHUFFLE")) {
					for (auto const& who : saveContainer.current.party) {
						Event("TearDown", "TEARDOWNPLAYERREADOUT", pair<string, string>("who", who)).run(*&gameEngine);
					}
				}
				if (menu.data.hasKey("PARTYSHUFFLE") or menu.data.hasKey("MANAGESKILLS")) {
					List<string> party = saveContainer.current.allCharacters;
					List<Graphics::Image*> toRemove;
					for (auto const & who : party.internalList) {
						if (graphics.doesThisImageAlreadyExist(who + "_CARD")) {
							toRemove.push_back(graphics.accessImageViaUniqueID(who + "_CARD"));
						}
						Map<string, string> skills; skills.internalMap = saveContainer.current.equippedSkills[who];
						Map<string, Combat::Skill> skillsDefined = combat.getSkillsOnASkillBar(skills, true);
						for (auto skillSlot : skillsDefined.getKeys().internalList) {
							string imageID = who + "_SKILLSLOT_" + skillSlot;
							string borderImageID = who + "_SKILLSLOTBORDER_" + skillSlot;
							if (graphics.doesThisImageAlreadyExist(imageID)) {
								toRemove.push_back(graphics.accessImageViaUniqueID(imageID));
								toRemove.push_back(graphics.accessImageViaUniqueID(borderImageID));
							}
						}
					}
					if (graphics.doesThisImageAlreadyExist("SKILLBARBACKGROUND")) {
						toRemove.push_back(graphics.accessImageViaUniqueID("SKILLBARBACKGROUND"));
					}
					for (Graphics::Image* image : toRemove.internalList) {
						Event("TearDown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", image->unique_ID)).run(*&gameEngine);
					}
				}
				if (menu.data.hasKey("PARTYREFORM") or menu.data.hasKey("SELECTCHARACTERTOEDIT")) {
					for (auto const& member : saveContainer.current.allCharacters) {
						Event("TearDown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", member + "_CARD")).run(*&gameEngine);
					}
				}
				if (menu.data.hasKey("SKILLEXPLAIN")) {
					List<string> tearTheseDown = list<string>({ "skill_explain","skill_explainBorder", "skill_explainACTIVATION", "skill_explainCOST", "skill_explainRECHARGE" });
					for (auto image : tearTheseDown.internalList) {
						Event("TearDownImage", "TEARDOWNIMAGE", Map<string, string>({
						pair<string, string>("uniqueID", image) })).run(*&gameEngine);
					}
					List<string> tearTheseTextsDown = List<string>({"skillExplainText", "skillExplainText_skillTreeName", "skillExplainText_Name","skillExplainText_Cost","skillExplainText_Activation","skillExplainText_Recharge"});
					for (auto text : tearTheseTextsDown.internalList) {
						Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({ "uniqueID", text })).run(*&gameEngine);
					}
				}
				if (menu.data.hasKey("MANAGEEQUIPMENT")) {
					Event("Teardown", "TEARDOWNTEXT", Map<string, string>({ pair<string, string>("uniqueID", "EQUIPMENTEXPLAIN") })).run(*&gameEngine);
				}
				return true;
			}
			if (type == "HANDLEBUTTON") {
				string buttonLogic = data["uniqueID"];
				controller.resetMouseClickPosition();
				if (buttonLogic.find("PLUSBUTTON") != -1) {
					string who = split(buttonLogic, "_").at(0);
					string attribute = split(buttonLogic, "_").at(1);
					saveContainer.tryToChangeInvestmentByOnePoint(who, attribute, true);
				}
				if (buttonLogic.find("MINUSBUTTON") != -1) {
					string who = split(buttonLogic, "_").at(0);
					string attribute = split(buttonLogic, "_").at(1);
					saveContainer.tryToChangeInvestmentByOnePoint(who, attribute, false);
				}
				if (buttonLogic.find("MINUSBUTTON") != -1 or buttonLogic.find("PLUSBUTTON") != -1) {
					string who = split(buttonLogic, "_").at(0);
					wstring message1 = combat.loadPartyMemberAsCombatant(who).getPrintout(gameEngine.language, *&combat, gameEngine.stateFlags["includeEquipmentInStatView"] == "1");
					wstring message2 = combat.loadPartyMemberAsCombatant(who).getPointsRemainingPrintout(gameEngine.language, *&combat);
					graphics.accessTextViaUniqueID(who + "_PRINTOUTTEXT")->resetMessage(*&graphics, message1);
					graphics.accessTextViaUniqueID(who + "_POINTSLEFTTEXT")->resetMessage(*&graphics, message2);
					return true;
				}
				if (buttonLogic == "DEBUGBUTTON1") {
					Event("Return", "TEARDOWNMENU", pair<string,string>("uniqueID", "DEBUGMENU")).run(*&gameEngine);
					gameEngine.activeProcedure = Procedure("Explore", { Event("Explore","EXPLORE",{}) });
					return true;
				}
				if (buttonLogic == "RETURNTOEXPLORE") {

					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EXPLOREPAUSE")).run(*&gameEngine);
					gameEngine.activeProcedure = Procedure("Explore", { Event("Explore","EXPLORE",{}) });
					return true;
				}
				if (buttonLogic == "MERCHANTTOEXPLORE") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "MERCHANT")).run(*&gameEngine);
					List<Menu::Button> toHandle = gameEngine.storedMenus["MERCHANT"].buttons.getValues();
					for (auto button : toHandle.internalList) {
						Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
							pair<string,string>("imageID", button.imageID),
							pair<string,string>("textID", button.textID)})).run(*&gameEngine);
					}
					gameEngine.activeProcedure = Procedure("Explore", { Event("Explore","EXPLORE",{}) });
					return true;
				}
				if (buttonLogic == "AUDIOSETTINGS") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EXPLOREPAUSE")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("AUDIOSETTINGS");
					return true;
				}
				if (buttonLogic == "QUITTODESKTOP") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EXPLOREPAUSE")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("QUITCONFIRM");
					return true;
				}
				if (buttonLogic == "QUITTOMAIN") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EXPLOREPAUSE")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("QUITTOMAINCONFIRM");
					return true;
				}
				if (buttonLogic == "QUITYES") {
					saveContainer.save();
					gameEngine.stateFlags["QUIT"] = "1";
					return true;
				}
				if (buttonLogic == "QUITNO") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "QUITCONFIRM")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EXPLOREPAUSE");
					return true;
				}
				if (buttonLogic == "QUITMAINNO") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "QUITTOMAINCONFIRM")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EXPLOREPAUSE");
					return true;
				}
				if (buttonLogic == "QUITMAINYES") {
					saveContainer.save();
					Event("Return", "STOPALLSONGS", {}).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "QUITTOMAINCONFIRM")).run(*&gameEngine);
					Event("Return", "UNLOADIMAGESFORMAPCHANGE", pair<string, string>("uniqueID", "")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.storedProcedures["BOOTMENU"];
					return false;
				}
				if (buttonLogic == "FROMAUDIOTOPAUSE") {
					audio.tryToSaveAudioSettings();
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "AUDIOSETTINGS")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EXPLOREPAUSE");
					return true;
				}
				if (buttonLogic == "PARTYMANAGEMENT") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EXPLOREPAUSE")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("PARTYMANAGEMENT");
					return true;
				}
				if (buttonLogic == "FROMPARTYTOEXPLOREPAUSE") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "PARTYMANAGEMENT")).run(*&gameEngine);
					saveContainer.save();
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EXPLOREPAUSE");
					return true;
				}
				if (buttonLogic == "equipmentToggle") {
					int source = 0;
					if (gameEngine.stateFlags["includeEquipmentInStatView"] == "1") {
						gameEngine.stateFlags["includeEquipmentInStatView"] = "0";
						source = HIDEEQUIPMENT;
					}
					else {
						gameEngine.stateFlags["includeEquipmentInStatView"] = "1";
						source = SHOWEQUIPMENT;
					}
					Graphics::Image* theImage = graphics.accessImageViaUniqueID("equipmentToggle");
					theImage->resetSources(*&graphics, { source });
					List<string> party = saveContainer.current.party;
					List<pair<float, float>> positions = Menu::getPlayerCardPositions();
					for (int x = 0; x < min(party.size(), saveContainer.partyLimit); x++) {
						string who = party.at(x);
						Event("TearDown", "TEARDOWNPLAYERREADOUT", pair<string, string>("who", who)).run(*&gameEngine);
						Event("PrintoutPlayer", "SETUPPLAYERREADOUT", List<pair<string, string>>({
							pair<string, string>("who", who),
							pair<string, string>("x", to_string(positions.at(x).first - 8)),
							pair<string, string>("y", to_string(positions.at(x).second + 25)),
							})).run(*&gameEngine);
					}
					return true;
				}				
				if (buttonLogic == "FROMPARTYREFORMTOPARTYMANAGEMENT") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "PARTYREFORM")).run(*&gameEngine);
					saveContainer.save();
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("PARTYMANAGEMENT");
					return true;
				}
				if (buttonLogic == "FROMPARTYMANAGEMENTTOPARTYREFORM") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "PARTYMANAGEMENT")).run(*&gameEngine);
					saveContainer.save();
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("PARTYREFORM");
					return true;
				}
				if (buttonLogic == "FROMPARTYMANAGEMENTTOSKILLMANAGE") {
					gameEngine.stateFlags["PARTYEDITSELECTED"] = "";
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "PARTYMANAGEMENT")).run(*&gameEngine);
					saveContainer.save();
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("SKILLMANAGEMENT");
					return true;
				}
				if (buttonLogic == "FROMSKILLMANAGETOPARTYMANAGE") {
					string whoWasLastPlayer = gameEngine.stateFlags["PARTYEDITSELECTED"];
					gameEngine.stateFlags["PARTYEDITSELECTED"] = "";
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "SKILLMANAGEMENT")).run(*&gameEngine);
					if (whoWasLastPlayer != "") {
						for (auto knownSkill : saveContainer.current.knownSkills[whoWasLastPlayer]) {
							string toTeardown = knownSkill + "_" + whoWasLastPlayer + "_SKILLSELECTIONGRID";
							string toTearDown2 = knownSkill + "_" + whoWasLastPlayer + "_SKILLSELECTIONBORDER";
							if (graphics.doesThisImageAlreadyExist(toTeardown));
							Event("Teardown", "TEARDOWNIMAGE", Map<string, string>({ pair<string, string>("uniqueID", toTeardown) })).run(*&gameEngine);
							Event("Teardown", "TEARDOWNIMAGE", Map<string, string>({ pair<string, string>("uniqueID", toTearDown2) })).run(*&gameEngine);
						}
					}
					saveContainer.save();
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("PARTYMANAGEMENT");
					return true;
				}
				if (buttonLogic == "PARTYMANAGEHELP") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EXPLOREPAUSE")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("HELPMENU");
					Event("LoadCodexImage", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", "130"),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", "1.1"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"] - 1)),
						pair<string, string>("uniqueID", "HELPIMAGE"),
						})).run(*&gameEngine);
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message","$LANGUAGE$_GUI_HELP1"),
						pair<string, string>("direct", "0"),
						pair<string, string>("format", "HighTowerText_15"),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", "35"),
						pair<string, string>("y", "8"),
						pair<string, string>("w", "55"),
						pair<string, string>("h", "90"),
						pair<string, string>("colour", "WHITE"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "HELPTEXT"),
						})).run(*&gameEngine);
					return true;
				}
				if (buttonLogic.find("HELPBUTTON") != -1) {
					graphics.accessTextViaUniqueID("HELPTEXT")->resetMessage(*&graphics, strings[gameEngine.language]["GUI"][buttonLogic + "ANSWER"]);
					return true;
				}
				if (buttonLogic == "SKILLTREE1") {
					if (gameEngine.stateFlags["PARTYEDITSELECTED"] == "") { return true; }
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("SKILLTREEEDIT1");
					return true;
				}
				if (buttonLogic == "EQUIPMENTTYPE") {
					if (gameEngine.stateFlags["PARTYEDITSELECTED"] == "") { return true; }
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EQUIPMENTEDIT");
					return true;
				}
				if (buttonLogic == "SKILLTREE2") {
					if (gameEngine.stateFlags["PARTYEDITSELECTED"] == "") { return true; }
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("SKILLTREEEDIT2");
					return true;
				}
				if (buttonLogic == "CANCELSKILLTREECHOICE") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "SKILLTREEEDIT1")).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "SKILLTREEEDIT2")).run(*&gameEngine);
					gameEngine.activeProcedure = Procedure("MenuProcedure", List<Event>({ 
						Event("HandleMenu", "HANDLEMENU", Map<string, string>({pair<string, string>("uniqueID", "SKILLMANAGEMENT"),})) }));
				}
				if (buttonLogic == "CANCELEQUIPCHOICE") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EQUIPMENTEDIT")).run(*&gameEngine);
					gameEngine.activeProcedure = Procedure("MenuProcedure", List<Event>({
						Event("HandleMenu", "HANDLEMENU", Map<string, string>({pair<string, string>("uniqueID", "EQUIPMENTMANAGEMENT"),})) }));
				}
				if (combat.getNamesOfAllItemTypeNames(gameEngine.language).contains(buttonLogic)) {
					string category = buttonLogic;
					gameEngine.stateFlags["WEPTYPESELECTED"] = category;
					List<Menu::Button> toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons.getValues();
					for (auto button : toHandle.internalList) {
						Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
							pair<string, string>("imageID", button.imageID),
							pair<string, string>("textID",button.textID),
							})).run(*&gameEngine);
					}
					Event("AddButtons", "ADDEQUIPBUTTONSTOMENU", Map<string, string>({ 
						pair<string, string>("category", category),
						pair<string, string>("who", gameEngine.stateFlags["PARTYEDITSELECTED"]),
						})).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EQUIPMENTEDIT")).run(*&gameEngine);
					toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons.getValues();
					for (auto button : toHandle.internalList) {
						Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "EQUIPMENTMANAGEMENT"),
							pair<string, string>("which", button.uniqueID),
							pair<string, string>("layer", gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].data["layer"]),
							pair<string, string>("format", button.extras["format"]),
							})).run(*&gameEngine);
					}
					gameEngine.activeProcedure = Procedure("MenuProcedure", List<Event>({
						Event("HandleMenu", "HANDLEMENU", Map<string, string>({pair<string, string>("uniqueID", "EQUIPMENTMANAGEMENT"),})) }));
					return false;
				}
				if (combat.getNamesOfAllSkillTreeNames(gameEngine.language).contains(buttonLogic)) {
					string whichMenu = data["menuName"];
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", whichMenu)).run(*&gameEngine);
					Event("Return", "SELECTCHARACTERTOEDIT", Map<string, string>({
						pair<string, string>("justRemoveExisting", "1"),
						pair<string, string>("menu", whichMenu)
						})).run(*&gameEngine);
					
					string whichSlot = SReplace(whichMenu, "SKILLTREEEDIT", "");
					string who = gameEngine.stateFlags["PARTYEDITSELECTED"];
					saveContainer.current.equippedSkillTrees[who][whichSlot] = buttonLogic;
					Map<string, string> skillTrees; skillTrees.internalMap = saveContainer.current.equippedSkillTrees[who];
					Map<string, string> knownSkills; knownSkills.internalMap = saveContainer.current.equippedSkills[who];
					Map<string, string> knownSkillsAfter;
					for (auto slot : knownSkills.getKeys().internalList) {
						Combat::Skill def = combat.skillDefinitions[knownSkills[slot]];
						if (skillTrees.getValues().contains(def.skillTree)) {
							knownSkillsAfter[slot] = knownSkills[slot];
						}
					}
					saveContainer.current.equippedSkills[who] = knownSkillsAfter.internalMap;
					saveContainer.save();
					gameEngine.activeProcedure = Procedure("MenuProcedure", List<Event>({
						Event("HandleMenu", "HANDLEMENU", Map<string,string>({pair<string, string>("uniqueID", "SKILLMANAGEMENT"),}))
						}));
					Event("Return", "SELECTCHARACTERTOEDIT", Map<string, string>({
						pair<string, string>("menu", "SKILLMANAGEMENT"),
						pair<string, string>("byForce", "1") })).run(*&gameEngine);
				}
				if (buttonLogic == "FROMPARTYMANAGEMENTTOEQUIPMENT") {
					gameEngine.stateFlags["PARTYEDITSELECTED"] = "";
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "PARTYMANAGEMENT")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EQUIPMENTMANAGEMENT");
					return true;
				}
				if (buttonLogic == "FROMEQUIPMANAGETOPARTYMANAGE") {
					List<Menu::Button> toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons.getValues();
					for (auto button : toHandle.internalList) {
						Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
							pair<string, string>("imageID", button.imageID),
							pair<string, string>("textID",button.textID),
							})).run(*&gameEngine);
					}
					graphics.tearDownSpecifiedText("explainEquipmentHover");
					Event("AddButtons", "ADDEQUIPBUTTONSTOMENU", Map<string, string>({ 
						pair<string, string>("category", "None"),
						pair<string, string>("who", gameEngine.stateFlags["PARTYEDITSELECTED"]),
						})).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EQUIPMENTMANAGEMENT")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("PARTYMANAGEMENT");
					return true;
				}
				if (buttonLogic.find("EQUIP_") != -1) {
					string who = gameEngine.stateFlags["PARTYEDITSELECTED"];
					string what = SReplace(buttonLogic, "EQUIP_", "");
					Combat::Equipment def = combat.equipmentDefinitions[what];
					string category = def.category;
					saveContainer.unequipThis(who, category);
					saveContainer.equipThis(who, category, what);
					List<Menu::Button> toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons.getValues();
					for (auto button : toHandle.internalList) {
						Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
							pair<string, string>("imageID",button.imageID),
							pair<string, string>("textID",button.textID),
							})).run(*&gameEngine);
					}
					Event("AddButtons", "ADDEQUIPBUTTONSTOMENU", Map<string, string>({
						pair<string, string>("category", category),
						pair<string, string>("who", who),
						})).run(*&gameEngine);
					toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons.getValues();
					for (auto button : toHandle.internalList) {
						Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "EQUIPMENTMANAGEMENT"),
							pair<string, string>("which", button.uniqueID),
							pair<string, string>("layer", gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].data["layer"]),
							pair<string, string>("format", button.extras["format"]),
							})).run(*&gameEngine);
					}
					graphics.accessTextViaUniqueID("EQUIPMENTEXPLAIN")->resetMessage(*&graphics, combat.loadPartyMemberAsCombatant(who).getEquipmentPrintout(gameEngine.language, *&combat));
					return true;
				}
				if (buttonLogic.find("Buy_") != -1) {
					string what = SReplace(buttonLogic, "Buy_", "");
					gameEngine.stateFlags["itemOfInterest"] = what;
					int price = combat.equipmentDefinitions[what].price;
					int currentMoney = saveContainer.current.money;
					int currentlyHave = saveContainer.howManyOfThisItemInInventory(what);
					if (price > currentMoney) {
						gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("TRADEDENY");
					}
					if (currentlyHave >= saveContainer.inventoryLimitPerItem) {
						gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("TRADEDENYFULL");
					}
					if (price <= currentMoney and currentlyHave < saveContainer.inventoryLimitPerItem) {
						gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("MERCHANTCONFIRM");
					}
					gameEngine.activeProcedure.eventList.at(1).data["whichMerchant"] = data["whichMerchant"];
					return true;
				}
				if (buttonLogic.find("Buyback_") != -1) {
					string what = SReplace(buttonLogic, "Buyback_", "");
					gameEngine.stateFlags["itemOfInterest"] = what;
					int price = combat.equipmentDefinitions[what].price;
					int currentMoney = saveContainer.current.money;
					int currentlyHave = saveContainer.howManyOfThisItemInInventory(what);
					if (price > currentMoney) {
						gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("TRADEDENY");
					}
					if (currentlyHave >= saveContainer.inventoryLimitPerItem) {
						gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("TRADEDENYFULL");
					}
					if (price <= currentMoney and currentlyHave < saveContainer.inventoryLimitPerItem) {
						gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("MERCHANTCONFIRM");
					}
					gameEngine.activeProcedure.eventList.at(1).data["whichMerchant"] = data["whichMerchant"];
					return true;
				}
				if (buttonLogic.find("Sell_") != -1) {
					string what = SReplace(buttonLogic, "Sell_", "");
					gameEngine.stateFlags["itemOfInterest"] = what;
					int price = combat.equipmentDefinitions[what].price;
					int currentMoney = saveContainer.current.money;
					int currentlyHave = saveContainer.howManyOfThisItemInInventory(what);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("MERCHANTCONFIRM");
					gameEngine.activeProcedure.eventList.at(1).data["whichMerchant"] = data["whichMerchant"];
					return true;
				}
				if (buttonLogic == "MERCHANTBUY") {
					gameEngine.storedMenus["MERCHANT"].currentScrolls.clear();
					string whichMerchant = data["whichMerchant"];
					gameEngine.storedMenus["MERCHANT"].data["MERCHANT"] = "Buy";
					Event("Change Merch Buttons", "ADDMERCHANTITEMBUTTONS", Map<string, string>({
						pair<string, string>("whichMerchant", whichMerchant),
						})).run(*&gameEngine);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->resetMessage(*&graphics, Graphics::Text::commonTextReplacements(gameEngine.language, merchants.merchantDefinitions[whichMerchant].getCorrectDialogue(gameEngine.language, "Buy")));
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter(*&graphics);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->animationStyles.internalList = { "TYPEWRITER", "PARCHMENT" };
					return true;
				}
				if (buttonLogic == "MERCHANTSELL") {
					gameEngine.storedMenus["MERCHANT"].currentScrolls.clear();
					string whichMerchant = data["whichMerchant"];
					gameEngine.storedMenus["MERCHANT"].data["MERCHANT"] = "Sell";
					Event("Change Merch Buttons", "ADDMERCHANTITEMBUTTONS", Map<string, string>({
						pair<string, string>("whichMerchant", whichMerchant),
						})).run(*&gameEngine);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->resetMessage(*&graphics, Graphics::Text::commonTextReplacements(gameEngine.language, merchants.merchantDefinitions[whichMerchant].getCorrectDialogue(gameEngine.language, "Sell")));
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter(*&graphics);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->animationStyles.internalList = { "TYPEWRITER", "PARCHMENT" };
					return true;
				}
				if (buttonLogic == "MERCHANTBUYBACK") {
					gameEngine.storedMenus["MERCHANT"].currentScrolls.clear();
					string whichMerchant = data["whichMerchant"];
					gameEngine.storedMenus["MERCHANT"].data["MERCHANT"] = "Buyback";
					Event("Change Merch Buttons", "ADDMERCHANTITEMBUTTONS", Map<string, string>({
						pair<string, string>("whichMerchant", whichMerchant),
						})).run(*&gameEngine);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->resetMessage(*&graphics, Graphics::Text::commonTextReplacements(gameEngine.language, merchants.merchantDefinitions[whichMerchant].getCorrectDialogue(gameEngine.language, "Buyback")));
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter(*&graphics);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->animationStyles.internalList = { "TYPEWRITER", "PARCHMENT" };
					return true;
				}
				if (buttonLogic == "TradeConfirmNo") {
					string whichMerchant = data["whichMerchant"];
					gameEngine.stateFlags["itemOfInterest"] = "";
					for (auto menu : { "MERCHANTCONFIRM", "TRADEDENY", "TRADEDENYFULL" }) {
						Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", menu)).run(*&gameEngine);
					}	
					gameEngine.activeProcedure = Procedure("MenuProcedure", List<Event>({
						Event("HandleMenu", "HANDLEMENU", Map<string, string>({
							pair<string, string>("uniqueID", "MERCHANT"),
							pair<string, string>("whichMerchant", whichMerchant),
							})) }));
					return true;
				}
				if (buttonLogic == "TradeConfirmYes") {
					string whichMerchant = data["whichMerchant"];
					string item = gameEngine.stateFlags["itemOfInterest"];
					string mode = gameEngine.storedMenus["MERCHANT"].data["MERCHANT"];
					int price = combat.equipmentDefinitions[item].price;
					Event("PlaySound", "PLAYSFX", Map<string, string>({
							pair<string, string>("audio", to_string(MONEY_WAV)),
							pair<string, string>("direct", "1"),
						})).run(*&gameEngine);
					if (mode == "Buy") {
						saveContainer.increaseItemInventoryCount(item, combat.equipmentDefinitions[item].category);
						saveContainer.loseMoney(price);
					}
					if (mode == "Buyback") {
						saveContainer.increaseItemInventoryCount(item, combat.equipmentDefinitions[item].category);
						saveContainer.loseMoney(price);
						saveContainer.reduceSoldItemCount(item);
					}
					if (mode == "Sell") {
						saveContainer.reduceItemInventoryCount(item);
						saveContainer.gainMoney(price);
						saveContainer.increaseSoldItemCount(item);
					}
					Event("Change Merch Buttons", "ADDMERCHANTITEMBUTTONS", Map<string, string>({
						pair<string, string>("whichMerchant", whichMerchant),
						})).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "MERCHANTCONFIRM")).run(*&gameEngine);
					gameEngine.activeProcedure = Procedure("MenuProcedure", List<Event>({
						Event("HandleMenu", "HANDLEMENU", Map<string, string>({
							pair<string, string>("uniqueID", "MERCHANT"),
							pair<string, string>("whichMerchant", whichMerchant),
							})) }));
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->resetMessage(*&graphics, strings[gameEngine.language]["GUI"]["MERCHANTTHANKS" + mode]);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter(*&graphics);
					graphics.accessTextViaUniqueID("MONEYSTATUS_TEXT")->resetMessage(*&graphics, to_wstring(saveContainer.current.money));
					return true;
				}
				if (buttonLogic == "AcceptItem") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "YOUFOUNDANITEM")).run(*&gameEngine);
					gameEngine.activeProcedure = Procedure("Explore", { Event("Explore","EXPLORE",{}) });
					string objectName = data["objectID"];
					string flagName = data["objectID"] + "_OPENED";
					string item = data["contents"];
					saveContainer.current.flags[flagName] = true;
					Explorer::mapObject & obj = explorer.getThisMapObject(objectName);
					obj.canInteract = false;
					Event("UpdateMap", "EXPLORE", Map<string, string>({
						pair <string,string>("force","1"),
						})).run(*&gameEngine);
					saveContainer.increaseItemInventoryCount(item, combat.equipmentDefinitions[item].category);
					saveContainer.save();
					return true;
				}
				if (buttonLogic == "TUTYES") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "TUTORIALCONFIRM")).run(*&gameEngine);
					string player = saveContainer.getCurrentMainCharacter();
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "Tutorial", player, "EXPLORE");
					return false;
				}
				if (buttonLogic == "TUTNO") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "TUTORIALCONFIRM")).run(*&gameEngine);
					string player = saveContainer.getCurrentMainCharacter();
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "HorsemanCutscene2$" + player, player, "EXPLORE");
					return false;
				}
				if (buttonLogic == "COMBAT1CANCEL") {
					controller.resetMouseClickPosition();
					controller.resetMouseMovePosition();
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "COMBAT1")).run(*&gameEngine);
					gameEngine.storedMenus["COMBAT1"].buttonReplace(Menu::getDefaultCombatMenuButtons());
					Event("Return", "LOADMENU", pair<string, string>("uniqueID", "COMBAT1")).run(*&gameEngine);

					Event("RestoreOpacity", "RESTORECOMBATANTOPACITY", {}).run(*&gameEngine);
					return true;
				}
				if (buttonLogic == "COMBAT2QUIT") {
						Event("Return", "LOADMENU", pair<string, string>("uniqueID", "COMBATQUIT")).run(*&gameEngine);
						gameEngine.activeProcedure.eventList.push_front(Event("Quit?", "HANDLEMENU", List<pair<string, string>>({
								pair<string, string>("uniqueID", "COMBATQUIT"),
							})));
						return true;
				}
				if (buttonLogic == "COMBATQUITCANCEL") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "COMBATQUIT")).run(*&gameEngine);
					gameEngine.activeProcedure.eventList.pop_front();
					return true;
				}
				if (buttonLogic == "QUITCOMBAT") {
					combat.currentBattle->playerQuitBattle();
					string who = combat.currentBattle->currentRound.whoseTurnIsIt();
					Combat::CombatantInstance* actor = combat.currentBattle->getThisCombatant(who);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "COMBATQUIT")).run(*&gameEngine);
					Event("RestoreOpacity", "RESTORECOMBATANTOPACITY", {}).run(*&gameEngine);
					Event("Lifebars", "RELOADALLCOMBATANTIMAGES", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("mode", "HIDEBARS"),
						}))).run(*&gameEngine);
					Event("RemoveSkillBar", "TEARDOWNTHISSKILLBAR", Map<string, string>({
						pair<string, string>("who", actor->c.uniqueID),
						})).run(*&gameEngine);
					Event("RemoveCursor", "TEARDOWNIMAGE", Map<string, string>({
						pair<string, string>("uniqueID", "whoseTurn"),
						})).run(*&gameEngine);
					Event("RemoveHorizontal", "TEARDOWNLIFEBARSHORIZONTAL", {}).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "COMBAT1")).run(*&gameEngine);
					gameEngine.activeProcedure.eventList.pop_front();
					gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "HANDLECOMBATTICK", {}));
					gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "DETERMINESKILLEFFECTSTACK", List<pair<string, string>>({
							pair<string, string>("passive", "1"),
						})));
					return true;
				}
				if (buttonLogic.find("STARTUSINGSKILLON") != -1) {
					string who = combat.currentBattle->currentRound.whoseTurnIsIt();
					List<string> buttonData = split(buttonLogic, "$");
					string target = buttonData.at(1);
					int skillIndex = stoi(buttonData.at(2));
					Combat::CombatantInstance* actor = combat.currentBattle->getThisCombatant(who);
					if (!actor->c.currentlyCasting) {
						actor->c.startCasting(*&combat, target, skillIndex);
					}
					if (actor->c.getSkillBeingCast().uniqueID != "DEFAULT_WAIT") {
						string type = "MAGICAL";
						if (actor->c.getSkillBeingCast().skillTypeTags.contains("PHYSICAL")) {
							type = "PHYSICAL";
						}
						combat.currentBattle->addCombatMessage(*&combat, "STARTCASTING", List<pair<string, string>>({
							pair<string, string>("language", gameEngine.language),
							pair<string, string>("name", actor->c.uniqueID),
							pair<string, string>("skill", actor->c.getSkillBeingCast().uniqueID),
							pair<string, string>("skillType", type),
							}), 0);
					}
					Event("RestoreOpacity", "RESTORECOMBATANTOPACITY", {}).run(*&gameEngine);
					Event("Lifebars", "RELOADALLCOMBATANTIMAGES", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("mode", "HIDEBARS"),
						}))).run(*&gameEngine);
					Event("RemoveSkillBar", "TEARDOWNTHISSKILLBAR", Map<string, string>({
						pair<string, string>("who", actor->c.uniqueID),
						})).run(*&gameEngine);
					Event("RemoveCursor", "TEARDOWNIMAGE", Map<string, string>({
						pair<string, string>("uniqueID", "whoseTurn"),
						})).run(*&gameEngine);
					Event("RemoveHorizontal", "TEARDOWNLIFEBARSHORIZONTAL", {}).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "COMBAT1")).run(*&gameEngine);
					gameEngine.storedMenus["COMBAT1"].buttonReplace(Menu::getDefaultCombatMenuButtons());
					combat.currentBattle->executingSomething = true;
					gameEngine.activeProcedure.eventList.clear();
					gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "HANDLECOMBAT", {}));
					gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "PRINTSKILLSTACKRESULTS", {}));
					gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "EXECUTESKILLINCOMBAT", List<pair<string, string>>({
							//pair<string, string>("passive", "1"),
						})));
					gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "DETERMINESKILLEFFECTSTACK", List<pair<string, string>>({
							pair<string, string>("passive", "1"),
						})));
					if (actor->c.finishedCasting()) {
						// skill had a 0 round activation cost = cast now

						gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "PRINTSKILLSTACKRESULTS", {}));
						gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "EXECUTESKILLINCOMBAT", List<pair<string, string>>({
							pair<string, string>("active", "1"),
							})));
						gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "DETERMINESKILLEFFECTSTACK", List<pair<string, string>>({
							pair<string, string>("active", "1"),
							})));
					}
					return false;
				}
				if (buttonLogic.find("GOTOCODEXBUTTON") != -1) {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EXPLOREPAUSE")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "ParchmentFold", saveContainer.getCurrentMainCharacter(), "HANDLEMENU");
					return false;
				}
				if (buttonLogic.find("FROMCODEXTOPAUSE") != -1) {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "CODEX1")).run(*&gameEngine);
					Event("Remove", "REMOVEIMAGES", pair<string, string>("toRemove", "Parchment")).run(*&gameEngine);
					Event("Teardown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", "CODEXIMAGE")).run(*&gameEngine);
					Event("Teardown", "TEARDOWNTEXT", pair<string, string>("uniqueID", "CODEXTITLE")).run(*&gameEngine);
					Event("Teardown", "TEARDOWNTEXT", pair<string, string>("uniqueID", "CODEXTEXT")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EXPLOREPAUSE");
					return true;
				}
				if (buttonLogic.find("CODEXBOOK_") != -1) {
					gameEngine.stateFlags["CODEXBOOK"] = SReplace(buttonLogic, "CODEXBOOK_", "");
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "CODEX1")).run(*&gameEngine);
					Event("LoadAMenu", "LOADMENU", Map<string, string>({
						pair<string, string>("uniqueID", "CODEX1") })).run(*&gameEngine);
					return true;
				}
				if (buttonLogic.find("CODEXPAGE_") != -1) {
					string page = SReplace(buttonLogic, "CODEXPAGE_", "");
					string book = gameEngine.stateFlags["CODEXBOOK"];
					string titleSource = codex.nameSources[book];
					string contentSource = codex.contentSources[book];
					string title = gameEngine.language + "_" + titleSource + "_" + page;
					string content = gameEngine.language + "_" + contentSource + "_" + page;
					Event("Teardown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", "CODEXIMAGE")).run(*&gameEngine);
					Event("Teardown", "TEARDOWNTEXT", pair<string, string>("uniqueID", "CODEXTITLE")).run(*&gameEngine);
					Event("Teardown", "TEARDOWNTEXT", pair<string, string>("uniqueID", "CODEXTEXT")).run(*&gameEngine);
					Event("LoadCodexImage", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("Codex",page)),
						pair<string, string>("x", "20"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", "1.5"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"] - 1)),
						pair<string, string>("uniqueID", "CODEXIMAGE"),
						})).run(*&gameEngine);
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",title),
						pair<string, string>("direct", "0"),
						pair<string, string>("format", "HighTowerText_50"),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "2"),
						pair<string, string>("w", "50"),
						pair<string, string>("h", "15"),
						pair<string, string>("colour", "DARKBROWN"),
						pair<string, string>("shadowColour", "BLACK"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "CODEXTITLE"),
						})).run(*&gameEngine);
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",content),
						pair<string, string>("direct", "0"),
						pair<string, string>("format", "HighTowerText_20"),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", "38"),
						pair<string, string>("y", "12"),
						pair<string, string>("w", "55"),
						pair<string, string>("h", "50"),
						pair<string, string>("colour", "DARKBROWN"),
						pair<string, string>("shadowColour", "BLACK"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"] - 1)),
						pair<string, string>("uniqueID", "CODEXTEXT"),
						})).run(*&gameEngine);
				}
				if (buttonLogic == "BootMenuStart") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "BOOTMENU")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("MAINMENU");
					return true;
				}
				if (buttonLogic == "NewGame") {
					combat.newGame();
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "MAINMENU")).run(*&gameEngine);
					Event("Return", "TEARDOWNIMAGE", pair<string, string>("uniqueID", "Logo")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "credits")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "version")).run(*&gameEngine);
					Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("message","$LANGUAGE$_GUI_NewGame1"),
						pair<string, string>("animateExisting","0"),
						pair<string, string>("format", "HighTowerText_50"),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", "10"),
						pair<string, string>("y", "4"),
						pair<string, string>("w", "100"),
						pair<string, string>("h", "10"),
						pair<string, string>("colour", "ELITESKILLYELLOW"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", "4"),
						pair<string, string>("uniqueID", "startMenu1"),
						pair<string, string>("animated", "TRUE"),
						pair<string, string>("styles", "TYPEWRITER"),
						pair<string, string>("typewriterSpeed", "3"),
						}))).run(*&gameEngine);
					Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("message","$LANGUAGE$_NPCNames_Angela Fleuret"),
						pair<string, string>("animateExisting","0"),
						pair<string, string>("format", "HighTowerText_50"),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", "10"),
						pair<string, string>("y", "55"),
						pair<string, string>("w", "100"),
						pair<string, string>("h", "10"),
						pair<string, string>("colour", "ELITESKILLYELLOW"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", "4"),
						pair<string, string>("uniqueID", "startMenu2"),
						pair<string, string>("animated", "TRUE"),
						pair<string, string>("styles", "TYPEWRITER"),
						pair<string, string>("typewriterSpeed", "25"),
						}))).run(*&gameEngine);
					Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("message","$LANGUAGE$_NewGameDescriptions_Angela Fleuret"),
						pair<string, string>("animateExisting","0"),
						pair<string, string>("format", "HighTowerText_30"),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", "15"),
						pair<string, string>("y", "65"),
						pair<string, string>("w", "60"),
						pair<string, string>("h", "50"),
						pair<string, string>("colour", "ELITESKILLYELLOW"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", "4"),
						pair<string, string>("uniqueID", "newGameCharacterDescription"),
						pair<string, string>("animated", "TRUE"),
						pair<string, string>("styles", "TYPEWRITER"),
						pair<string, string>("typewriterSpeed", "3"),
						}))).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("NEWGAME");
					return true;
				}
				if (buttonLogic == "Credits") {
					Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("message","$LANGUAGE$_GUI_CREDITSMESSAGE"),
						pair<string, string>("animateExisting","0"),
						pair<string, string>("format", "Centaur_25"),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", "10"),
						pair<string, string>("y", "10"),
						pair<string, string>("w", "70"),
						pair<string, string>("h", "700"),
						pair<string, string>("colour", "ELITESKILLYELLOW"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", "4"),
						pair<string, string>("uniqueID", "CreditsText"),
						pair<string, string>("animated", "TRUE"),
						}))).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "MAINMENU")).run(*&gameEngine);
					Event("Return", "TEARDOWNIMAGE", pair<string, string>("uniqueID", "Logo")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "credits")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "version")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("CREDITS");
				}


				if (buttonLogic == "LoadGame") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "MAINMENU")).run(*&gameEngine);
					Event("Return", "TEARDOWNIMAGE", pair<string, string>("uniqueID", "Logo")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "credits")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "version")).run(*&gameEngine);
					saveContainer.getAllLocalValidSaves();
					gameEngine.storedMenus["LOADGAME"].buttonReplace( Menu::getLoadGameButtons());
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("LOADGAME");
					return false;
				}
				if (buttonLogic == "NewGameToMain") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "NEWGAME")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "newGameCharacterDescription")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "startMenu2")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "startMenu1")).run(*&gameEngine);
					Event("Return", "LOADBOOTMENU", {}).run(*&gameEngine);
					return false;
				}
				if (buttonLogic == "CreditsToMain") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "CREDITS")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "CreditsText")).run(*&gameEngine);
					Event("Return", "LOADBOOTMENU", {}).run(*&gameEngine);
				}

				if (buttonLogic.find("LoadThisSaveSlot_") != -1) {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "MAINMENU")).run(*&gameEngine);
					Event("Return", "TEARDOWNIMAGE", pair<string, string>("uniqueID", "Logo")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "credits")).run(*&gameEngine);
					Event("Return", "TEARDOWNTEXT", pair<string, string>("uniqueID", "version")).run(*&gameEngine);
					string whichSlot = split(buttonLogic, "_").at(1);
					audio.fadeOutAndStopThis(MENU1_WAV, 3);
					saveContainer.current = saveContainer.slots[stoi(whichSlot)];
					saveContainer.activeSaveSlot = stoi(whichSlot);
					if (!saveContainer.current.flags["IntroFinished"]) {
						gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "NewGameCutscene", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					}
					else {
						gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "TavernTeleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					}
					return false;
				}
				if (buttonLogic == "LoadGameToMain") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "LOADGAME")).run(*&gameEngine);
					Event("Return", "LOADBOOTMENU", {}).run(*&gameEngine);
				}
				if (buttonLogic == "StartGame") {
					Event("Load Image", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", "4286"),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "0.0"),
						pair<string, string>("layer", "25"),
						pair<string, string>("styles", "FADEIN$SINGLE"),
						pair<string, string>("uniqueID", "NewGameLoadingScreen"),
						pair<string, string>("animated", "1"),
						pair<string, string>("animation_speed", "60"),
						}))).run(*&gameEngine);
					saveContainer.current.party = { gameEngine.stateFlags["PARTYEDITSELECTED"] };
					saveContainer.current.allCharacters = { gameEngine.stateFlags["PARTYEDITSELECTED"] };
					audio.fadeOutAndStopThis(MENU1_WAV, 3);
					Event("Play This Song", "PLAYTHISSONG", Map<string, string>(List<pair<string, string>>({ pair<string, string>("uniqueID", to_string(WINDOUTSIDE1_WAV)), }))).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "NewGameCutscene", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					saveContainer.activeSaveSlot = saveContainer.getNextSaveSlot();
					saveContainer.unlockThisCodexPage("people", gameEngine.stateFlags["PARTYEDITSELECTED"]);
					saveContainer.addAllPagesFromThisBook("skill trees");
					return false;
				}
				if (buttonLogic == "FROMPAUSETOTOME") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "PARTYMANAGEMENT")).run(*&gameEngine);
					saveContainer.save();
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("TOME");
					return true;
				}
				if (buttonLogic == "FROMTOMETOPAUSE") {
					Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", "learnedASkill"})).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "TOME")).run(*&gameEngine);
					for (auto buttonName : gameEngine.storedMenus["TOME"].buttons.getKeys().internalList) {
						gameEngine.storedMenus["TOME"].removeThisButton(buttonName);
					}
					gameEngine.storedMenus["TOME"].buttonReplace(Menu::getDefaultMenuButtonsForTomes());
					Event("Reload", "TEARDOWNSKILLEXPLAIN", {}).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("PARTYMANAGEMENT");
					return true;
				}
				if (buttonLogic == "FROMSPIGOTTOEXPLORE") {
					Event("TearDownThisImage", "REMOVEIMAGES", pair<string, string>("toRemove", "SpigotPuzzle$SpigotOil$SpigotPuzzleAnimation")).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "SPIGOTPUZZLE")).run(*&gameEngine);
					explorer.puzzleContainer.resetSpigotPuzzle();
					gameEngine.activeProcedure = Procedure("Explore", { Event("Explore","EXPLORE",{}) });
					return true;
				}
				if (buttonLogic.find("TURNSPIGOTLEVEL") != -1) {
					string whichLever = SReplace(buttonLogic, "TURNSPIGOTLEVEL", "");
					explorer.puzzleContainer.runSpigotPuzzleLogic(whichLever);
					Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
									pair<string,string>("audio",to_string(SPIGOTPUZZLE1_WAV)),
									pair<string,string>("direct","1"),
						})).run(*&gameEngine);
					List<string> goingUp = List<string>({"1","3","4"});
					List<string> goingDown = List<string>({"2","2","5"});
					graphics.accessImageViaUniqueID("SpigotPuzzleAnimation")->animationStyles.internalList = {"SINGLE"};
					if (goingUp.contains(whichLever)) {
						graphics.accessImageViaUniqueID("SpigotPuzzleAnimation")->resetSources(*&graphics, imageLookup.animationFrames["SpigotPuzzle"]["ACTION_FRONT"]);
						graphics.accessImageViaUniqueID("SpigotPuzzleAnimation")->frame = 0;
						Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
									pair<string,string>("audio",to_string(MOREOIL_WAV)),
									pair<string,string>("direct","1"),
							})).run(*&gameEngine);
					}
					else {
						graphics.accessImageViaUniqueID("SpigotPuzzleAnimation")->resetSources(*&graphics, imageLookup.animationFrames["SpigotPuzzle"]["REVERSE_FRONT"]);
						graphics.accessImageViaUniqueID("SpigotPuzzleAnimation")->frame = 0;
						Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
									pair<string,string>("audio",to_string(LESSOIL_WAV)),
									pair<string,string>("direct","1"),
							})).run(*&gameEngine);
					}
					List<int> sources = imageLookup.animationFrames["SpigotOil"]["ACTION_" + to_string(explorer.puzzleContainer.currentSpigotPuzzleValue)];
					if (sources.empty()) {
						throw exception("No sources for this image!");
					}
					graphics.accessImageViaUniqueID("SpigotOil")->resetSources(*&graphics, sources.internalList);
					for (auto button : gameEngine.storedMenus["SPIGOTPUZZLE"].buttons.getKeys().internalList) {
						if (button.find("TURNSPIGOTLEVEL") != -1) {
							string thisLever = SReplace(button, "TURNSPIGOTLEVEL", "");
							string status = explorer.puzzleContainer.spigotPuzzleLevers[thisLever];
							wstring newButtonText = strings[gameEngine.language]["GUI"]["TURNLEVERBASE"] + StringToWString(thisLever) + L" ";
							newButtonText += strings[gameEngine.language]["GUI"]["LEVER" + status];
							graphics.accessTextViaUniqueID(button + "_TEXT")->resetMessage(*&graphics, newButtonText);
						}
					}
					if (explorer.puzzleContainer.spigotPuzzleSolved()) {
						Event("TearDownThisImage", "REMOVEIMAGES", pair<string, string>("toRemove", "SpigotPuzzle$SpigotOil$SpigotPuzzleAnimation")).run(*&gameEngine);
						Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "SPIGOTPUZZLE")).run(*&gameEngine);
						Event("SetFlag", "SETFLAGANDSAVE", Map<string, string>({
								pair<string, string>({"uniqueID", "SpigotPuzzleDone"}),
								pair<string, string>({"status", "1"}),
							})).run(*&gameEngine);

						gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "SpigotActivated", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					}
					return true;
				}
				if (buttonLogic == "SKILLTREETOME") {
					Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", "learnedASkill"})).run(*&gameEngine);
					for (auto buttonName : gameEngine.storedMenus["TOME"].buttons.getKeys().internalList) {
						if (buttonName.find("TOMETEACH_") != -1) {
							gameEngine.storedMenus["TOME"].removeThisButton(buttonName);
						}
					}
					gameEngine.storedMenus["TOME"].buttonReplace(Menu::getDefaultMenuButtonsForTomes());
					gameEngine.storedMenus["TOME"].editExistingButton(gameEngine.language, "SKILLTREETOME", Menu::smallButton("CANCELSKILLTREECHOICETOME", "GUI_CANCELSKILLTREECHOICE", { 10, 10 }));
					List<string> skillTreeNames;
					Map<string, int> tomes = combat.getInventoryItemsOfXCategory("Tome");
					for (auto tome : tomes.getKeys().internalList) {
						if (tomes[tome] < 1) { continue; }
						string skillName = SReplace(tome, "Tome of ", "");
						if (!combat.skillDefinitions.hasKey(skillName)) { continue; }
						string skillTreeName = combat.skillDefinitions[skillName].skillTree;
						List<string> doNotDraw; doNotDraw.internalList = { "Debug", "Default" };
						if (doNotDraw.contains(skillTreeName)) { continue; }
						skillTreeNames.addToBackIfNotAlreadyInList(skillTreeName);
					}
					skillTreeNames.internalList.sort();
					pair<int, int> pos = { 20,15 };
					int distance = 4;
					for (auto name : skillTreeNames.internalList) {
						string buttonName = "TOMETREE_" + name;
						gameEngine.storedMenus["TOME"].buttons[buttonName] = { Menu::smallButton(buttonName, "Skill Tree Names_" + name, pos) };
						pos.second += distance;
						Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
						pair<string, string>("uniqueID", "TOME"),
						pair<string, string>("which", buttonName),
						pair<string, string>("layer", "20"),
						pair<string, string>("format", gameEngine.storedMenus["TOME"].buttons[buttonName].extras["format"]),
							})).run(*&gameEngine);
					}
					return true;
				}
				if (buttonLogic == "CANCELSKILLTREECHOICETOME") {
					Event("Reload", "TEARDOWNSKILLEXPLAIN", {}).run(*&gameEngine);
					for (auto buttonName : gameEngine.storedMenus["TOME"].buttons.getKeys().internalList) {
						if (buttonName.find("TOMETEACH_") != -1 or buttonName.find("TOMETREE_") != -1 or buttonName.find("TOMETEACHTHESKILL_") != -1) {
							gameEngine.storedMenus["TOME"].removeThisButton(buttonName);
						}
					}
					gameEngine.storedMenus["TOME"].buttonReplace(Menu::getDefaultMenuButtonsForTomes());
					gameEngine.storedMenus["TOME"].editExistingButton(gameEngine.language, "CANCELSKILLTREECHOICETOME", gameEngine.storedMenus["TOME"].buttons["SKILLTREETOME"]);
					return true;
				}
				if (buttonLogic.find("TOMETREE_") != -1) {
					Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", "learnedASkill"})).run(*&gameEngine);
					gameEngine.storedMenus["TOME"].editExistingButton(gameEngine.language, "CANCELSKILLTREECHOICETOME", gameEngine.storedMenus["TOME"].buttons["SKILLTREETOME"]);
					for (auto buttonName : gameEngine.storedMenus["TOME"].buttons.getKeys().internalList) {
						if (buttonName.find("TOMETREE_") != -1) {
							gameEngine.storedMenus["TOME"].removeThisButton(buttonName);
						}
					}

					string treeName = SReplace(buttonLogic, "TOMETREE_", "");
					Map<string, int> tomes = combat.getInventoryItemsOfXCategory("Tome");
					pair<int, int> pos = { 20,15 };
					int distance = 4;
					for (auto tome : tomes.getKeys().internalList) {
						if (tomes[tome] < 1) { continue; }
						string skillName = SReplace(tome, "Tome of ", "");
						if (!combat.skillDefinitions.hasKey(skillName)) { continue; }
						string skillTreeName = combat.skillDefinitions[skillName].skillTree;
						string colour = "WHITE";
						if (combat.skillDefinitions[skillName].skillTypeTags.contains("ELITE")) {
							colour = "ELITESKILLYELLOW";
						}
						if (skillTreeName == treeName) {
							string buttonName = ("TOMETEACH_" + skillName);
							gameEngine.storedMenus["TOME"].buttons[buttonName] = { Menu::smallButton(buttonName, "Skill Names_" + skillName, pos) };
							gameEngine.storedMenus["TOME"].buttons[buttonName].extras["colour"] = colour;
							pos.second += distance;
							if (pos.second > 90) {
								pos.first += 20;
								pos.second = 15;
							}
							Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
								pair<string, string>("uniqueID", "TOME"),
								pair<string, string>("which", buttonName),
								pair<string, string>("layer", "20"),
								pair<string, string>("format", gameEngine.storedMenus["TOME"].buttons[buttonName].extras["format"]),
								pair<string, string>("colour", colour),
								})).run(*&gameEngine);
						}
					}
					return true;
				}
				if (buttonLogic.find("TOMETEACH_") != -1) {
					Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", "learnedASkill"})).run(*&gameEngine);
					for (auto buttonName : gameEngine.storedMenus["TOME"].buttons.getKeys().internalList) {
						if (buttonName.find("TOMETEACHTHESKILL_") != -1 or buttonName == "TOMETEACH_X") {
							gameEngine.storedMenus["TOME"].removeThisButton(buttonName);
						}
					}

					string skillName = SReplace(buttonLogic, "TOMETEACH_", "");
					Event("Reload", "TEARDOWNSKILLEXPLAIN", {}).run(*&gameEngine);
					Event("LoadSkillDesc", "SETUPSKILLEXPLAIN", Map<string, string>({
						pair<string, string>("whichSkill", skillName),
						pair<string, string>("method", "teachingTomes"),
						pair<string, string>("scale", "1"),
						pair<string, string>("x", "40"),
						pair<string, string>("y", "20"),
						})).run(*&gameEngine);

					List<string> allPeople = saveContainer.getAllCharacterNames();
					pair<int, int> pos = { 75,10 };
					int distance = 5;
					strings[gameEngine.language]["GUI"]["TEACHTO"] = WSReplace(strings[gameEngine.language]["GUI"]["TEACHTOBASE"], L"$", strings[gameEngine.language]["Skill Names"][skillName]);
					gameEngine.storedMenus["TOME"].buttons["TOMETEACH_X"] = {Menu::TextBox("TOMETEACH_X", "GUI_TEACHTO", "TINY", pos)};
					gameEngine.storedMenus["TOME"].buttons["TOMETEACH_X"].clickable = false;

					Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
								pair<string, string>("uniqueID", "TOME"),
								pair<string, string>("which", "TOMETEACH_X"),
								pair<string, string>("layer", "20"),
								pair<string, string>("format", gameEngine.storedMenus["TOME"].buttons["TOMETEACH_X"].extras["format"]), })).run(*&gameEngine);

					pos.second += 5;
					
					for (auto person : allPeople.internalList) {
						string buttonName = "TOMETEACHTHESKILL_" + skillName + "_TO_" + person;
						string sourceOfText = "GUI_" + person + "doesn'tknowThisSkill";

						strings[gameEngine.language]["GUI"][person + "doesn'tknowThisSkill"] = strings[gameEngine.language]["NPCNames"][person];

						if (saveContainer.getKnownSkillNames(person).contains(skillName)) {
							strings[gameEngine.language]["GUI"][person + "knowsThisSkill"] = WSReplace(strings[gameEngine.language]["GUI"]["knowsThisSkill"], L"$", strings[gameEngine.language]["NPCNames"][person]);
							sourceOfText = "GUI_" + person + "knowsThisSkill";
						}
						gameEngine.storedMenus["TOME"].buttons[buttonName] = { Menu::smallButton(buttonName, sourceOfText, pos) };
						pos.second += distance;
						Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
								pair<string, string>("uniqueID", "TOME"),
								pair<string, string>("which", buttonName),
								pair<string, string>("layer", "20"),
								pair<string, string>("format", gameEngine.storedMenus["TOME"].buttons[buttonName].extras["format"]),
							})).run(*&gameEngine);
					}
					return true;
				}
				if (buttonLogic == "DebugButton_StartAFight") {
					gameEngine.activeProcedure.eventList.push_front(Event("Debug Combat", "DEBUGCOMBAT", {}));
					return false;
				}
				if (buttonLogic == "DebugButton_TeleportToChapel") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "ChapelTeleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return false;
				}
				if (buttonLogic == "DebugButton_ShowFPS") {
					gameEngine.stateFlags["SHOWFPS"] = "1";
					return true;
				}
				if (buttonLogic == "DebugButton_FastMove") {
					Args.toggle("SPEEDCHEAT");
					return true;
				}
				if (buttonLogic == "DebugButton_BackToExplore") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "DEBUGMENU")).run(*&gameEngine);
					gameEngine.activeProcedure = Procedure("Explore", { Event("Explore","EXPLORE",{}) });
					return true;
				}
				if (buttonLogic == "DebugButton_Amnesia") {
					saveContainer.current.knownSkills.clear();
					return true;
				}
				if (buttonLogic == "DebugButton_TeleportToChapelR1") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "ChapelR1Teleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return false;
				}
				if (buttonLogic == "DebugButton_TeleportToChapelR2") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "ChapelR2Teleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return false;
				}
				if (buttonLogic == "DebugButton_TeleportToChapelR2ByStairs") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "ChapelR2ByStairsTeleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return false;
				} 
				if (buttonLogic == "DebugButton_TeleportToChapelR3") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "ChapelR3Teleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return false;
				}
				if (buttonLogic == "DebugButton_TeleportToTown") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "TownTeleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_TeleportToMerchant") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "MerchantTeleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_TeleportToEstateSecret") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "EstateBalconyTeleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				} 
				if (buttonLogic == "DebugButton_TeleportToChapelCrackInWall") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "ChapelEntryHallCrackInWallTeleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_TeleportToChapelLeft1") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "Chapel2LeftWingTeleport", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_TeleportToSpigotPuzzle") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "TeleportToSpigotPuzzle", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_BloodWallDefeated") {
					saveContainer.current.flags["BloodWallDestroyed"] = true;
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "PostBloodWall", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_OudinDefeated") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "OudinDefeated", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_DreamSequence1") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "TavernLetter$PLAYER", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_TeleportToSpain") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "Teleport to Spain", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_RunCutscene") {
					Event("ResetMenu", "TEARDOWNMENU", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("uniqueID", "DEBUGMENU"),
						}))).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "Tutorial", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return false;
				}
				if (buttonLogic == "DebugButton_AddAllPlayable") {
					for (auto s : saveContainer.getAllPlayableCharacters().internalList) {
						saveContainer.addToCharacterList(s);
					}
					return true;
				}
				if (buttonLogic == "DebugButton_VariousChapelR2Flags") {
					List<string> toToggle = List<string>({"OudinIntro_TRIGGERED", "OudinCutscenePlanks_TRIGGERED", "VisionRoom_TRIGGERED", "VisionRoom2_TRIGGERED", "OudinHostileTriggered", "SawOudinThroughGap",
						"BloodWallIntro_TRIGGERED", "WaterPuzzleFinished", "ChapelRightCombat1_TRIGGERED","ChapelRightCorridor+PAIR_TRIGGERED",
						});
					for (auto t : toToggle.internalList) {
						Event("DoButton", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "DebugButton_ToggleFlag_" + t),
							})).run(*&gameEngine);
					}
					return true;
				}
				if (buttonLogic == "DebugButton_VariousChapelL1Flags") {
					List<string> toToggle = List<string>({ "FireSpigotLeftActivated", "FireSpigotTopLeftActivated", "SpigotPuzzleDone", "VisionLCutscene_TRIGGERED",
						});
					for (auto t : toToggle.internalList) {
						Event("DoButton", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "DebugButton_ToggleFlag_" + t),
							})).run(*&gameEngine);
					}
					return true;
				}
				if (buttonLogic.find("DebugButton_ToggleFlag_") != -1) {
					string flagName = SReplace(buttonLogic, "DebugButton_ToggleFlag_", "");
					saveContainer.current.flags[flagName] = !saveContainer.current.flags[flagName];
					return true;
				}
				if (buttonLogic == "DebugButton_Player2Cutscene") {
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "DebugPlayer2", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return false;
				}
				if (buttonLogic == "DebugButton_ChapelFinished") {
					List<string> toToggle = List<string>({ "HorsemanCutscene1_TRIGGERED", "IntroFinished", "OudinIntro_TRIGGERED", "ChapelRightCorridor+PAIR_TRIGGERED", "OudinCutscenePlanks_TRIGGERED", "VisionRoom_TRIGGERED", "VisionRoom2_TRIGGERED", "OudinHostileTriggered", "SawOudinThroughGap",
						"BloodWallIntro_TRIGGERED", "BloodWallIntro", "WaterPuzzleFinished","FireSpigotLeftActivated", "FireSpigotTopLeftActivated", "SpigotPuzzleDone", "VisionLCutscene_TRIGGERED", "BloodWallDestroyed", "OudinDefeated",
						});
					for (auto t : toToggle.internalList) {
						Event("DoButton", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "DebugButton_ToggleFlag_" + t),
							})).run(*&gameEngine);
					}
					return true;
				}
				if (buttonLogic == "DebugButton_RiderDefeatedCutscene") {
					Event("DoButton", "HANDLEBUTTON", Map<string, string>({pair<string, string>("uniqueID", "DebugButton_ChapelFinished"),})).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "MadRiderDefeated", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_BloodWallIntro") {
					List<string> toToggle = List<string>({ "OudinIntro_TRIGGERED", "OudinCutscenePlanks_TRIGGERED", "VisionRoom_TRIGGERED", "VisionRoom2_TRIGGERED", "OudinHostileTriggered", "SawOudinThroughGap",});
					for (auto t : toToggle.internalList) {
						Event("DoButton", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "DebugButton_ToggleFlag_" + t),
							})).run(*&gameEngine);
					}
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "DEBUGMENU")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "BloodWallIntro", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_WaterRoomDebug") {
					List<string> toToggle = List<string>({ "WaterPuzzleActivated", });
					for (auto t : toToggle.internalList) {
						Event("DoButton", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "DebugButton_ToggleFlag_" + t),
							})).run(*&gameEngine);
					}
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "DEBUGMENU")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "WaterRoomDebug", saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return true;
				}
				if (buttonLogic == "DebugButton_LookingForMichelet") {
					Event("DoButton", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "DebugButton_ChapelFinished"),
						})).run(*&gameEngine);
					Event("DoButton", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "DebugButton_ToggleFlag_LookingForMichelet"),
						})).run(*&gameEngine);
					return true;
				}
				if (buttonLogic == "DebugButton_FoundMichelet") {
					Event("DoButton", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "DebugButton_LookingForMichelet"),
						})).run(*&gameEngine);
					Event("DoButton", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "DebugButton_ToggleFlag_FoundMichelet"),
						})).run(*&gameEngine);
				}

				if (buttonLogic.find("TOMETEACHTHESKILL_") != -1) {
					List<string> data = split(buttonLogic, "_");
					string theSkill = data.at(1);
					Combat::Skill& skillDef = combat.skillDefinitions[theSkill];
					string who = data.at(3);
					if (saveContainer.getKnownSkillNames(who).contains(theSkill)) {
						return true; // do nothing because they already know the skill
					}
					saveContainer.useTome(theSkill, who);


					Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
									pair<string,string>("audio",to_string(LEARNSKILL_WAV)),
									pair<string,string>("direct","1"),
						})).run(*&gameEngine);
					wstring message = strings[gameEngine.language]["GUI"]["LEARNEDASKILLBASE"];
					wstring whoName = strings[gameEngine.language]["NPCNames"][who];
					wstring skillName = strings[gameEngine.language]["Skill Names"][theSkill];
					message = WSReplace(message, L"$", whoName);
					message = WSReplace(message, L"&", skillName);
					strings[gameEngine.language]["GUI"]["LEARNEDASKILL"] = message;
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message","$LANGUAGE$_GUI_LEARNEDASKILL"),
						pair<string, string>("format", "HighTowerText_20"),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "20"),
						pair<string, string>("w", "70"),
						pair<string, string>("h", "50"),
						pair<string, string>("colour", "WHITE"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "learnedASkill"),
						pair<string, string>("direct", "0"),
						})).run(*&gameEngine);

					Event("Autoselect", "HANDLEBUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "CANCELSKILLTREECHOICETOME"),
						})).run(*&gameEngine);
				}
				if (buttonLogic == "HelpMenu_BackToPartyManage") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "HELPMENU")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EXPLOREPAUSE");
					Event("TearDownThisSkillIcon", "TEARDOWNIMAGE", pair<string, string>("uniqueID", "HELPIMAGE")).run(*&gameEngine);
					Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
							pair<string, string>("uniqueID","HELPTEXT"),
						})).run(*&gameEngine);
					return true;
				}
				return true;
}
			if (type == "LOADBOOTMENU") {
				Event("Load Image", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("LOGO","ACTION_1")),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "30"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "0.0"),
						pair<string, string>("layer", "2"),
						pair<string, string>("styles", "FADEIN$LOOP"),
						pair<string, string>("uniqueID", "Logo"),
						pair<string, string>("animated", "1"),
						pair<string, string>("animation_speed", "120"),
					}))).run(*&gameEngine);
				Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
							pair<string, string>("message","$LANGUAGE$_GUI_VERSION"),
							pair<string, string>("animateExisting","0"),
							pair<string, string>("format", "Centaur_25"),
							pair<string, string>("anchorStyle", "TOPLEFT"),
							pair<string, string>("x", "88"),
							pair<string, string>("y", "95"),
							pair<string, string>("w", "100"),
							pair<string, string>("h", "10"),
							pair<string, string>("colour", "ELITESKILLYELLOW"),
							pair<string, string>("shadowColour", "DARKBROWN"),
							pair<string, string>("layer", "4"),
							pair<string, string>("uniqueID", "version"),
							pair<string, string>("animated", "TRUE"),
							pair<string, string>("styles", "TYPEWRITER"),
							pair<string, string>("typewriterSpeed", "25"),
							pair<string, string>("nowait", "1"),
					}))).run(*&gameEngine);
				Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
									pair<string, string>("message","$LANGUAGE$_GUI_CREDIT"),
									pair<string, string>("animateExisting","0"),
									pair<string, string>("format", "Centaur_25"),
									pair<string, string>("anchorStyle", "TOPLEFT"),
									pair<string, string>("x", "2"),
									pair<string, string>("y", "95"),
									pair<string, string>("w", "100"),
									pair<string, string>("h", "10"),
									pair<string, string>("colour", "ELITESKILLYELLOW"),
									pair<string, string>("shadowColour", "DARKBROWN"),
									pair<string, string>("layer", "4"),
									pair<string, string>("uniqueID", "credits"),
									pair<string, string>("animated", "TRUE"),
									pair<string, string>("styles", "TYPEWRITER"),
									pair<string, string>("typewriterSpeed", "25"),
									pair<string, string>("nowait", "1"),
					}))).run(*&gameEngine);
				gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("MAINMENU");
				return false;
			}
			if (type == "LOADSKILLBARHERE") {
				bool drawFullSkillbar = data["full"] == "1"; // draw background and default skills
				string who = data["who"];
				float scale = stof(data["scale"]);
				pair<float, float> centreAnchor = { stof(data["x"]), stof(data["y"]) };
				Map<string, string> skills = saveContainer.getNamesOfCurrentEquippedSkills(who, drawFullSkillbar);
				float widthOfOneSkill = Graphics::Drawable().convertActualToPercent(graphics.hwndRenderTarget->GetSize(), { 100,100 }).first * scale * controller.initialGUIScale;
				float firstXSlot = centreAnchor.first - (widthOfOneSkill * 3);
				float difference = widthOfOneSkill;
				if (drawFullSkillbar) {
					Event("LoadThisSkillIcon", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", to_string(SKILLBAR)),
						pair<string, string>("x", to_string(centreAnchor.first)),
						pair<string, string>("y", to_string(centreAnchor.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", data["scale"]),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"] - 1)),
						pair<string, string>("uniqueID", "SKILLBARBACKGROUND"),
						})).run(*&gameEngine);
				}
				Map<string, Combat::Skill> skillsToDraw = combat.getSkillsOnASkillBar(skills, drawFullSkillbar);
				for (auto skillSlot : skillsToDraw.getKeys().internalList) {
					float xLocation = firstXSlot + (difference * stoi(skillSlot));
					Event("LoadThisSkillIcon", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", to_string(skillsToDraw[skillSlot].imageSource)),
						pair<string, string>("x", to_string(xLocation)),
						pair<string, string>("y", to_string(centreAnchor.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", data["scale"]),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", who + "_SKILLSLOT_" + skillSlot),
						})).run(*&gameEngine);
					int border = skillsToDraw[skillSlot].getBorderSource();
					Event("LoadThisSkillIcon", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", to_string(border)),
						pair<string, string>("x", to_string(xLocation)),
						pair<string, string>("y", to_string(centreAnchor.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", data["scale"]),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"]+1)),
						pair<string, string>("uniqueID", who + "_SKILLSLOTBORDER_" + skillSlot),
						})).run(*&gameEngine);
				}
				if (data.hasKey("combat")) {
					// add stuff to say if the skill is usable or not
				}
			}
			if (type == "TEARDOWNTHISSKILLBAR") {
				string who = data["who"];
				Map<string, string> skills; skills.internalMap = saveContainer.current.equippedSkills[who];
				Map<string, Combat::Skill> skillsToDraw = combat.getSkillsOnASkillBar(skills, true);
				for (auto skillSlot : skillsToDraw.getKeys().internalList) {
					string imageID = who + "_SKILLSLOT_" + skillSlot;
					string borderImageID = who + "_SKILLSLOTBORDER_" + skillSlot;
					if (graphics.doesThisImageAlreadyExist(imageID)) {
						Event("TearDownThisSkillIcon", "TEARDOWNIMAGE", pair<string, string>("uniqueID", imageID)).run(*&gameEngine);
						Event("TearDownThisSkillIcon", "TEARDOWNIMAGE", pair<string, string>("uniqueID", borderImageID)).run(*&gameEngine);
					}
					if (graphics.doesThisImageAlreadyExist("SKILLBARBACKGROUND")) {
						Event("TearDownBackground", "TEARDOWNIMAGE", pair<string, string>("uniqueID", "SKILLBARBACKGROUND")).run(*&gameEngine);
					}
				}
				return true;
			}
			if (type == "HANDLESKILLEXPLAIN") {
				if (graphics.beingDragged.size() > 0) {
					return true; // don't change skill explain if doing a click and drag
				}
				bool inCombat = false;
				if (data.hasKey("combat")) {
					inCombat = true;
					gameEngine.stateFlags["includeEquipmentInStatView"] = "1";
				}
				float scale = stof(data["scale"]);
				bool full = data["full"] == "1";
				pair<float, float> centreAnchor = { stof(data["x"]), stof(data["y"]) };
				bool isMouseHoveredOverAnySkill = false;
				string hoveredOverSkillSlot = "";
				string skillExplainID = "skill_explain";
				string skillExplainBorderID = "skill_explainBorder";
				string who = "";
				string skill_slot = "";
				string whichSkill = "";
				string textColour = "WHITE";
				bool isElite = false;
				Map<string, string> skills;
				float textWidth = 24;
				Map<string, pair<string, string>> desc;
				desc["name"] = { to_string(centreAnchor.first + 4.5), to_string(centreAnchor.second - 8) };
				desc["cost"] = { to_string(centreAnchor.first + 6), to_string(centreAnchor.second - 5.2) };
				desc["activation"] = { to_string(centreAnchor.first + 11), to_string(centreAnchor.second - 5.2) };
				desc["recharge"] = { to_string(centreAnchor.first + 16), to_string(centreAnchor.second - 5.2) };
				desc["skillTree"] = { to_string(centreAnchor.first + 4.5), to_string(centreAnchor.second - 2.5) };
				desc["description"] = { to_string(centreAnchor.first - 4), to_string(centreAnchor.second + 7) };
				string namingStyle = "SKILLSLOT";
				for (Graphics::Image* theImage : graphics.allImages().internalList) {
					bool hoveredOver = false;
					bool hoverable = false;

					if (inCombat and theImage->unique_ID.find("$COMBATEFFECT") != -1) { hoverable = true; namingStyle = "COMBAT"; }
					if (theImage->unique_ID.find("_SKILLSLOT_") != -1) { hoverable = true; namingStyle = "SKILLSLOT";}
					if (theImage->unique_ID.find("_SKILLSELECTIONGRID") != -1) {hoverable = true; namingStyle = "GRID";}

					if (theImage->hasThisBeenClickedOn(*&graphics, controller.mouseMovePosition)) {hoveredOver = true;}

					if (hoverable and hoveredOver) {
						isMouseHoveredOverAnySkill = true;
						if (namingStyle == "SKILLSLOT") {
							hoveredOverSkillSlot = theImage->unique_ID;
							who = split(hoveredOverSkillSlot, "_").at(0);
							skill_slot = split(hoveredOverSkillSlot, "_").at(2);
							skills = saveContainer.getNamesOfCurrentEquippedSkills(who, full);
							whichSkill = skills[skill_slot];
						}
						if (namingStyle == "GRID") {
							who = gameEngine.stateFlags["PARTYEDITSELECTED"];
							whichSkill = split(theImage->unique_ID, "_").at(0);
						}
						if (namingStyle == "COMBAT") {
							List<string> imageData = split(theImage->unique_ID, "$");
							who = imageData.at(0);
							whichSkill = imageData.at(1);
							}
						break;
						}
					}
				if (!isMouseHoveredOverAnySkill and graphics.doesThisImageAlreadyExist(skillExplainID)) {
					Event("Teardown", "TEARDOWNSKILLEXPLAIN", Map<string, string>({
							pair<string, string>("skillExplainID", skillExplainID),
							pair<string, string>("skillExplainBorderID", skillExplainBorderID),
						})).run(*&gameEngine);
					return true;
				}
				if (isMouseHoveredOverAnySkill and namingStyle != "COMBAT") {
					Combat::Skill toDraw = combat.skillDefinitions[whichSkill];
					string textColour = "WHITE";
					int border = toDraw.getBorderSource();
					isElite = toDraw.isElite();
					int cost = combat.loadPartyMemberAsCombatant(who).getSkillCost(toDraw, gameEngine.stateFlags["includeEquipmentInStatView"] == "1", *&combat);
					int activation = combat.loadPartyMemberAsCombatant(who).getSkillActivationTime(toDraw, gameEngine.stateFlags["includeEquipmentInStatView"] == "1", *&combat);
					int recharge = combat.loadPartyMemberAsCombatant(who).getSkillRecharge(toDraw, gameEngine.stateFlags["includeEquipmentInStatView"] == "1", *&combat);
					if (isElite) {
						textColour = "ELITESKILLYELLOW";
					}
					if (!graphics.doesThisImageAlreadyExist(skillExplainID)) {
						Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",who),
						pair<string, string>("skill",whichSkill),
						pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
						pair<string, string>("format", "HighTowerText_" + to_string(int(scale*20))),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", desc["description"].first),
						pair<string, string>("y", desc["description"].second),
						pair<string, string>("w", to_string(textWidth)),
						pair<string, string>("h", "60"),
						pair<string, string>("colour", textColour),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "skillExplainText"),
						pair<string, string>("direct", "1"),
						pair<string, string>("getStringFromCombatant", "skillprintout"),
							})).run(*&gameEngine);
						Event("DrawText", "DRAWTEXT", Map<string, string>({
							pair<string, string>("message",who),
							pair<string, string>("skill",whichSkill),
							pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
							pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
							pair<string, string>("anchorStyle", "TOPLEFT"),
							pair<string, string>("x", desc["name"].first),
							pair<string, string>("y", desc["name"].second),
							pair<string, string>("w", to_string(textWidth)),
							pair<string, string>("h", "60"),
							pair<string, string>("colour", textColour),
							pair<string, string>("shadowColour", "DARKBROWN"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", "skillExplainText_Name"),
							pair<string, string>("direct", "1"),
							pair<string, string>("getStringFromCombatant", "skillName"),
							})).run(*&gameEngine);
						Event("DrawText", "DRAWTEXT", Map<string, string>({
							pair<string, string>("message",gameEngine.language + "_Skill Tree Names_" + toDraw.skillTree),
							pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
							pair<string, string>("anchorStyle", "TOPLEFT"),
							pair<string, string>("x", desc["skillTree"].first),
							pair<string, string>("y", desc["skillTree"].second),
							pair<string, string>("w", to_string(textWidth)),
							pair<string, string>("h", "60"),
							pair<string, string>("colour", textColour),
							pair<string, string>("shadowColour", "DARKBROWN"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", "skillExplainText_skillTreeName"),
							pair<string, string>("getStringFromCombatant", "skillTreeName"),
							})).run(*&gameEngine);
						Event("DrawText", "DRAWTEXT", Map<string, string>({
							pair<string, string>("message",to_string(cost)),
							pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
							pair<string, string>("anchorStyle", "TOPLEFT"),
							pair<string, string>("x", desc["cost"].first),
							pair<string, string>("y", desc["cost"].second),
							pair<string, string>("w", to_string(textWidth)),
							pair<string, string>("h", "60"),
							pair<string, string>("colour", textColour),
							pair<string, string>("shadowColour", "DARKBROWN"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", "skillExplainText_Cost"),
							pair<string, string>("direct", "1"),
							})).run(*&gameEngine);
						Event("DrawText", "DRAWTEXT", Map<string, string>({
							pair<string, string>("message",to_string(activation)),
							pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
							pair<string, string>("anchorStyle", "TOPLEFT"),
							pair<string, string>("x", desc["activation"].first),
							pair<string, string>("y", desc["activation"].second),
							pair<string, string>("w", to_string(textWidth)),
							pair<string, string>("h", "60"),
							pair<string, string>("colour", textColour),
							pair<string, string>("shadowColour", "DARKBROWN"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", "skillExplainText_Activation"),
							pair<string, string>("direct", "1"),
							})).run(*&gameEngine);
						Event("DrawText", "DRAWTEXT", Map<string, string>({
							pair<string, string>("message",to_string(recharge)),
							pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
							pair<string, string>("anchorStyle", "TOPLEFT"),
							pair<string, string>("x", desc["recharge"].first),
							pair<string, string>("y", desc["recharge"].second),
							pair<string, string>("w", to_string(textWidth)),
							pair<string, string>("h", "60"),
							pair<string, string>("colour", textColour),
							pair<string, string>("shadowColour", "DARKBROWN"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", "skillExplainText_Recharge"),
							pair<string, string>("direct", "1"),
							})).run(*&gameEngine);
						Event("LoadImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(toDraw.imageSource)),
							pair<string, string>("x", data["x"]),
							pair<string, string>("y", data["y"]),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("scale", data["scale"]),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", skillExplainID)
							})).run(*&gameEngine);
						Event("LoadImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(border)),
							pair<string, string>("x", data["x"]),
							pair<string, string>("y", data["y"]),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("scale", data["scale"]),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", skillExplainBorderID)
							})).run(*&gameEngine);
						Event("LoadImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(MANA_COST_ICON)),
							pair<string, string>("x", to_string(centreAnchor.first + 5)),
							pair<string, string>("y", to_string(centreAnchor.second - 3.3)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("scale", data["scale"]),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", skillExplainID + "COST")
							})).run(*&gameEngine);
						Event("LoadImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(ACTIVATION_ICON)),
							pair<string, string>("x", to_string(centreAnchor.first + 10)),
							pair<string, string>("y", to_string(centreAnchor.second - 3.3)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("scale", data["scale"]),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", skillExplainID + "ACTIVATION")
							})).run(*&gameEngine);
						Event("LoadImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(RECHARGE_ICON)),
							pair<string, string>("x", to_string(centreAnchor.first + 15)),
							pair<string, string>("y", to_string(centreAnchor.second - 3.3)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("scale", data["scale"]),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", skillExplainID + "RECHARGE")
							})).run(*&gameEngine);
						return true;
					}
					if (graphics.accessImageViaUniqueID(skillExplainID) != NULL and graphics.accessImageViaUniqueID(skillExplainID)->sources.front() == toDraw.imageSource) {}
					else {
						Combat::Skill toDraw = combat.skillDefinitions[whichSkill];
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(skillExplainID);
						Graphics::Image* theBorder = graphics.accessImageViaUniqueID(skillExplainBorderID);
						if (theImage->sources.front() != toDraw.imageSource) {
							theImage->resetSources(*&graphics, { toDraw.imageSource });
							theBorder->resetSources(*&graphics, { toDraw.getBorderSource() });
							Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
							pair<string, string>("uniqueID","skillExplainText_skillTreeName"),
								})).run(*&gameEngine);
							Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
								pair<string, string>("uniqueID","skillExplainText"),
								})).run(*&gameEngine);
							Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
								pair<string, string>("uniqueID","skillExplainText_Name"),
								})).run(*&gameEngine);
							Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
								pair<string, string>("uniqueID","skillExplainText_Cost"),
								})).run(*&gameEngine);
							Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
								pair<string, string>("uniqueID","skillExplainText_Activation"),
								})).run(*&gameEngine);
							Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
								pair<string, string>("uniqueID","skillExplainText_Recharge"),
								})).run(*&gameEngine);
							Event("DrawText", "DRAWTEXT", Map<string, string>({
								pair<string, string>("message",who),
								pair<string, string>("skill",whichSkill),
								pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
								pair<string, string>("format", "HighTowerText_20"),
								pair<string, string>("anchorStyle", "TOPLEFT"),
								pair<string, string>("x", desc["description"].first),
								pair<string, string>("y", desc["description"].second),
								pair<string, string>("w", to_string(textWidth)),
								pair<string, string>("h", "60"),
								pair<string, string>("colour", textColour),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("uniqueID", "skillExplainText"),
								pair<string, string>("direct", "1"),
								pair<string, string>("getStringFromCombatant", "skillprintout"),
								})).run(*&gameEngine);
							Event("DrawText", "DRAWTEXT", Map<string, string>({
								pair<string, string>("message",gameEngine.language + "_Skill Tree Names_" + toDraw.skillTree),
								pair<string, string>("format", "HighTowerText_20"),
								pair<string, string>("anchorStyle", "TOPLEFT"),
								pair<string, string>("x", desc["skillTree"].first),
								pair<string, string>("y", desc["skillTree"].second),
								pair<string, string>("w", to_string(textWidth)),
								pair<string, string>("h", "60"),
								pair<string, string>("colour", textColour),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("uniqueID", "skillExplainText_skillTreeName"),
								pair<string, string>("getStringFromCombatant", "skillTreeName"),
								})).run(*&gameEngine);
							Event("DrawText", "DRAWTEXT", Map<string, string>({
								pair<string, string>("message",who),
								pair<string, string>("skill",whichSkill),
								pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
								pair<string, string>("format", "HighTowerText_20"),
								pair<string, string>("anchorStyle", "TOPLEFT"),
								pair<string, string>("x", desc["name"].first),
								pair<string, string>("y", desc["name"].second),
								pair<string, string>("w", to_string(textWidth)),
								pair<string, string>("h", "60"),
								pair<string, string>("colour", textColour),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("uniqueID", "skillExplainText_Name"),
								pair<string, string>("getStringFromCombatant", "skillName"),
								pair<string, string>("direct", "1"),
								})).run(*&gameEngine);
							Event("DrawText", "DRAWTEXT", Map<string, string>({
								pair<string, string>("message",to_string(cost)),
								pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
								pair<string, string>("format", "HighTowerText_20"),
								pair<string, string>("anchorStyle", "TOPLEFT"),
								pair<string, string>("x", desc["cost"].first),
								pair<string, string>("y", desc["cost"].second),
								pair<string, string>("w", to_string(textWidth)),
								pair<string, string>("h", "60"),
								pair<string, string>("colour", textColour),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("uniqueID", "skillExplainText_Cost"),
								pair<string, string>("direct", "1"),
								})).run(*&gameEngine);
							Event("DrawText", "DRAWTEXT", Map<string, string>({
								pair<string, string>("message",to_string(activation)),
								pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
								pair<string, string>("format", "HighTowerText_20"),
								pair<string, string>("anchorStyle", "TOPLEFT"),
								pair<string, string>("x", desc["activation"].first),
								pair<string, string>("y", desc["activation"].second),
								pair<string, string>("w", to_string(textWidth)),
								pair<string, string>("h", "60"),
								pair<string, string>("colour", textColour),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("uniqueID", "skillExplainText_Activation"),
								pair<string, string>("direct", "1"),
								})).run(*&gameEngine);
							Event("DrawText", "DRAWTEXT", Map<string, string>({
								pair<string, string>("message",to_string(recharge)),
								pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
								pair<string, string>("format", "HighTowerText_20"),
								pair<string, string>("anchorStyle", "TOPLEFT"),
								pair<string, string>("x", desc["recharge"].first),
								pair<string, string>("y", desc["recharge"].second),
								pair<string, string>("w", to_string(textWidth)),
								pair<string, string>("h", "60"),
								pair<string, string>("colour", textColour),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("uniqueID", "skillExplainText_Recharge"),
								pair<string, string>("direct", "1"),
								})).run(*&gameEngine);
						}
					}
				}	
				if (isMouseHoveredOverAnySkill and namingStyle == "COMBAT") {
					string textColour = "WHITE";
					Combat::EffectObjectInstance* toDraw = NULL;
					toDraw = combat.currentBattle->getThisEffect(who, whichSkill);
					if (toDraw == NULL) {
						return false;
					}
					if (!graphics.doesThisImageAlreadyExist(skillExplainID)) {
						Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",""),
						pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", desc["description"].first),
						pair<string, string>("y", desc["description"].second),
						pair<string, string>("w", to_string(textWidth)),
						pair<string, string>("h", "60"),
						pair<string, string>("colour", textColour),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "skillExplainText"),
						pair<string, string>("direct", "1"),
							})).run(*&gameEngine);
						Event("DrawText", "DRAWTEXT", Map<string, string>({
							pair<string, string>("message",""),
							pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
							pair<string, string>("anchorStyle", "TOPLEFT"),
							pair<string, string>("x", desc["name"].first),
							pair<string, string>("y", desc["name"].second),
							pair<string, string>("w", to_string(textWidth)),
							pair<string, string>("h", "60"),
							pair<string, string>("colour", textColour),
							pair<string, string>("shadowColour", "DARKBROWN"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", "skillExplainText_Name"),
							pair<string, string>("direct", "1"),
							})).run(*&gameEngine);
						Event("LoadImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(toDraw->e.imageRes)),
							pair<string, string>("x", data["x"]),
							pair<string, string>("y", data["y"]),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("scale", data["scale"]),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", skillExplainID)
							})).run(*&gameEngine);
						Event("LoadImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", to_string(toDraw->e.getBorder())),
							pair<string, string>("x", data["x"]),
							pair<string, string>("y", data["y"]),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("scale", data["scale"]),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"]+1)),
							pair<string, string>("uniqueID", skillExplainBorderID)
							})).run(*&gameEngine);
					}
					graphics.accessImageViaUniqueID(skillExplainID)->resetSources(*&graphics, { toDraw->e.imageRes });
					graphics.accessImageViaUniqueID(skillExplainBorderID)->resetSources(*&graphics, { toDraw->e.getBorder() });
					graphics.accessTextViaUniqueID("skillExplainText")->resetMessage(*&graphics, toDraw->e.getDescription(*&combat, gameEngine.language));
					graphics.accessTextViaUniqueID("skillExplainText_Name")->resetMessage(*&graphics, toDraw->e.getName(*&combat, gameEngine.language));
				}
				if (inCombat and isMouseHoveredOverAnySkill) {
					Map<string, string> skills = saveContainer.getNamesOfCurrentEquippedSkills(who, true);
					Map<string, Combat::Skill> skillsToDraw = combat.getSkillsOnASkillBar(skills, true);
					Combat::CombatantInstance* currentActor = combat.currentBattle->getCurrentCombatant();
					List<string> validSkills = currentActor->c.getSkillsThatCanBeUsedbyName(*&combat);
					string clickedOn;
					string slot = "";
					// add something to use keyboard 1 to 5 to use skills

					bool userHasClicked = controller.mouseInstructionsInOrder.contains("LButtonDown");
					pair<float, float> whereWasMouse = controller.mouseMovePosition;
					for (auto skillSlot : skillsToDraw.getKeys().internalList) {
						string imageName = who + "_SKILLSLOT_" + skillSlot;
						if (namingStyle != "COMBAT" and userHasClicked and graphics.accessImageViaUniqueID(imageName)->hasThisBeenClickedOn(*&graphics, controller.mouseMovePosition)) {
							if (CLOCK.hasEnoughTimePassed("ClickOnThisSkill", 100)) {
								Event("PlayHoverSound", "PLAYSFX", Map<string, string>({ pair<string,string>("audio",to_string(BUTTON_CLICK_WAV)),pair<string,string>("direct","1"), })).run(*&gameEngine);
							}
							clickedOn = skillsToDraw[skillSlot].uniqueID;
							slot = skillSlot;
						}
					}
					if (validSkills.contains(clickedOn)) {
						Event("ShowTargetButtons", "LOADCOMBATTARGETBUTTONS", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("whichSkillSlot", slot),
							}))).run(*&gameEngine);
					}
				}
				return false;
			}
			if (type == "SETUPSKILLEXPLAIN") {
				string whichSkill = data["whichSkill"];
				string method = data["method"];
				string who = data["who"];
				string skillExplainID = "skill_explain";
				string skillExplainBorderID = "skill_explainBorder";
				Combat::Skill toDraw = combat.skillDefinitions[whichSkill];
				string textColour = "WHITE";
				int border = toDraw.getBorderSource();
				bool isElite = toDraw.isElite();
				float textWidth = 24;
				float scale = stof(data["scale"]);
				int cost = 0;
				int activation = 0;
				int recharge = 0;
				string getStringFromCombatant = "skillprintout";
				bool influences = gameEngine.stateFlags["includeEquipmentInStatView"] == "1";
				Map<string, pair<string, string>> desc;
				pair<float, float> centreAnchor = { stof(data["x"]), stof(data["y"]) };
				desc["name"] = { to_string(centreAnchor.first + 4.5), to_string(centreAnchor.second - 8) };
				desc["cost"] = { to_string(centreAnchor.first + 6), to_string(centreAnchor.second - 5.2) };
				desc["activation"] = { to_string(centreAnchor.first + 11), to_string(centreAnchor.second - 5.2) };
				desc["recharge"] = { to_string(centreAnchor.first + 16), to_string(centreAnchor.second - 5.2) };
				desc["skillTree"] = { to_string(centreAnchor.first + 4.5), to_string(centreAnchor.second - 2.5) };
				desc["description"] = { to_string(centreAnchor.first - 4), to_string(centreAnchor.second + 7) };
				string namingStyle = "SKILLSLOT";
				if (isElite) {
					textColour = "ELITESKILLYELLOW";
				}
				if (method == "fromParty") {
					cost = combat.loadPartyMemberAsCombatant(who).getSkillCost(toDraw, influences, *&combat);
					activation = combat.loadPartyMemberAsCombatant(who).getSkillActivationTime(toDraw, influences, *&combat);
					recharge = combat.loadPartyMemberAsCombatant(who).getSkillRecharge(toDraw, influences, *&combat);
				}
				if (method == "teachingTomes") {
					cost = combat.definedCombatants["Tomes"].getSkillCost(toDraw, false, *&combat);
					activation = combat.definedCombatants["Tomes"].getSkillActivationTime(toDraw, false, *&combat);
					recharge = combat.definedCombatants["Tomes"].getSkillRecharge(toDraw, false, *&combat);
					getStringFromCombatant = "learningTomes";
				}
				if (!graphics.doesThisImageAlreadyExist(skillExplainID)) {
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",who),
						pair<string, string>("skill",whichSkill),
						pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
						pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", desc["description"].first),
						pair<string, string>("y", desc["description"].second),
						pair<string, string>("w", to_string(textWidth)),
						pair<string, string>("h", "60"),
						pair<string, string>("colour", textColour),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "skillExplainText"),
						pair<string, string>("direct", "1"),
						pair<string, string>("getStringFromCombatant", getStringFromCombatant),
						})).run(*&gameEngine);
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",who),
						pair<string, string>("skill",whichSkill),
						pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
						pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", desc["name"].first),
						pair<string, string>("y", desc["name"].second),
						pair<string, string>("w", to_string(textWidth)),
						pair<string, string>("h", "60"),
						pair<string, string>("colour", textColour),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "skillExplainText_Name"),
						pair<string, string>("direct", "1"),
						pair<string, string>("getStringFromCombatant", "skillName"),
						})).run(*&gameEngine);
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",gameEngine.language + "_Skill Tree Names_" + toDraw.skillTree),
						pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", desc["skillTree"].first),
						pair<string, string>("y", desc["skillTree"].second),
						pair<string, string>("w", to_string(textWidth)),
						pair<string, string>("h", "60"),
						pair<string, string>("colour", textColour),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "skillExplainText_skillTreeName"),
						pair<string, string>("getStringFromCombatant", "skillTreeName"),
						})).run(*&gameEngine);
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",to_string(cost)),
						pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", desc["cost"].first),
						pair<string, string>("y", desc["cost"].second),
						pair<string, string>("w", to_string(textWidth)),
						pair<string, string>("h", "60"),
						pair<string, string>("colour", textColour),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "skillExplainText_Cost"),
						pair<string, string>("direct", "1"),
						})).run(*&gameEngine);
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",to_string(activation)),
						pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", desc["activation"].first),
						pair<string, string>("y", desc["activation"].second),
						pair<string, string>("w", to_string(textWidth)),
						pair<string, string>("h", "60"),
						pair<string, string>("colour", textColour),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "skillExplainText_Activation"),
						pair<string, string>("direct", "1"),
						})).run(*&gameEngine);
					Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",to_string(recharge)),
						pair<string, string>("format", "HighTowerText_" + to_string(int(scale * 20))),
						pair<string, string>("anchorStyle", "TOPLEFT"),
						pair<string, string>("x", desc["recharge"].first),
						pair<string, string>("y", desc["recharge"].second),
						pair<string, string>("w", to_string(textWidth)),
						pair<string, string>("h", "60"),
						pair<string, string>("colour", textColour),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", "skillExplainText_Recharge"),
						pair<string, string>("direct", "1"),
						})).run(*&gameEngine);
					Event("LoadImage", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", to_string(toDraw.imageSource)),
						pair<string, string>("x", data["x"]),
						pair<string, string>("y", data["y"]),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", data["scale"]),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", skillExplainID)
						})).run(*&gameEngine);
					Event("LoadImage", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", to_string(border)),
						pair<string, string>("x", data["x"]),
						pair<string, string>("y", data["y"]),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", data["scale"]),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", skillExplainBorderID)
						})).run(*&gameEngine);
					Event("LoadImage", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", to_string(MANA_COST_ICON)),
						pair<string, string>("x", to_string(centreAnchor.first + 5)),
						pair<string, string>("y", to_string(centreAnchor.second - 3.3)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", data["scale"]),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", skillExplainID + "COST")
						})).run(*&gameEngine);
					Event("LoadImage", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", to_string(ACTIVATION_ICON)),
						pair<string, string>("x", to_string(centreAnchor.first + 10)),
						pair<string, string>("y", to_string(centreAnchor.second - 3.3)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", data["scale"]),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", skillExplainID + "ACTIVATION")
						})).run(*&gameEngine);
					Event("LoadImage", "LOADIMAGE", Map<string, string>({
						pair<string, string>("sources", to_string(RECHARGE_ICON)),
						pair<string, string>("x", to_string(centreAnchor.first + 15)),
						pair<string, string>("y", to_string(centreAnchor.second - 3.3)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("scale", data["scale"]),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", skillExplainID + "RECHARGE")
						})).run(*&gameEngine);
				}
				return true;
			}
			if (type == "TEARDOWNSKILLEXPLAIN") {
				string skillExplainID = "skill_explain";
				string skillExplainBorderID = "skill_explainBorder";
				Event("TearDownImage", "TEARDOWNIMAGE", Map<string, string>({
						pair<string, string>("uniqueID", skillExplainID) })).run(*&gameEngine);
				Event("TearDownImage", "TEARDOWNIMAGE", Map<string, string>({
					pair<string, string>("uniqueID", skillExplainBorderID) })).run(*&gameEngine);
				Event("TearDownImage", "TEARDOWNIMAGE", Map<string, string>({
					pair<string, string>("uniqueID", skillExplainID + "ACTIVATION") })).run(*&gameEngine);
				Event("TearDownImage", "TEARDOWNIMAGE", Map<string, string>({
					pair<string, string>("uniqueID", skillExplainID + "COST") })).run(*&gameEngine);
				Event("TearDownImage", "TEARDOWNIMAGE", Map<string, string>({
					pair<string, string>("uniqueID", skillExplainID + "RECHARGE") })).run(*&gameEngine);
				Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
					pair<string, string>("uniqueID","skillExplainText"),
					})).run(*&gameEngine);
				Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
					pair<string, string>("uniqueID","skillExplainText_Name"),
					})).run(*&gameEngine);
				Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
					pair<string, string>("uniqueID","skillExplainText_skillTreeName"),
					})).run(*&gameEngine);
				Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
					pair<string, string>("uniqueID","skillExplainText_Cost"),
					})).run(*&gameEngine);
				Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
					pair<string, string>("uniqueID","skillExplainText_Activation"),
					})).run(*&gameEngine);
				Event("TearDownText", "TEARDOWNTEXT", Map<string, string>({
					pair<string, string>("uniqueID","skillExplainText_Recharge"),
					})).run(*&gameEngine);
				return true;
			}
			if (type == "HANDLESKILLBAREDIT") {
				string theObjects;
				List<string> canBeDragged;
				for (auto x : { 20,21,22,23 }) {
					for (Graphics::Image* Image : graphics.ImageMap[x].internalList) {
						if (Image->unique_ID.find("SKILLSLOT_0") != -1 or
							Image->unique_ID.find("SKILLSLOT_6") != -1 or 
							Image->unique_ID.find("SKILLSLOTBORDER_0") != -1 or
							Image->unique_ID.find("SKILLSLOTBORDER_6") != -1 ) {
							continue;
						}
						if (Image->unique_ID.find("_SKILLSELECTIONGRID") != -1 or
							Image->unique_ID.find("_SKILLSLOT_") != -1 or
							Image->unique_ID.find("_SKILLSLOTBORDER_") != -1 or
							Image->unique_ID.find("_SKILLSELECTIONBORDER") != -1
							) {
							theObjects += Image->unique_ID + ",";
						}
					}
				}
				Event("ClickAndDrag", "CLICKANDDRAG", Map<string, string>({
					pair<string, string>("objects", theObjects),
					})).run(*&gameEngine);
				string scale = data["scale"];

				if (!graphics.recentlyFinishedBeingDragged.empty()) {
					bool skillbarNeedsToBeRedrawn = false;
					string who = gameEngine.stateFlags["PARTYEDITSELECTED"];
					Combat::Combatant theCharacter = combat.loadPartyMemberAsCombatant(who);
					Map<string, RECT> skillbarEditPositions = Menu::getSkillBarEditPositions();
					Map<string, string> equippedSkills; equippedSkills.internalMap = saveContainer.current.equippedSkills[who];


					string imageThatWasDragged = graphics.recentlyFinishedBeingDragged.front();
					string associatedBorder = SReplace(imageThatWasDragged, "SELECTIONGRID", "SELECTIONBORDER");
					string imageSource = "FROMGRID";
					if (imageThatWasDragged.find("SKILLSLOT") != -1) {
						imageSource = "FROMSKILLBAR";
					}
					Graphics::Image* draggedSkill = graphics.accessImageViaUniqueID(imageThatWasDragged);
					Graphics::Image* draggedBorder = graphics.accessImageViaUniqueID(associatedBorder);

					pair<float, float> whereDidUserUnclick = controller.mouseUnclickPosition;
					string unclickLocation = "OUTSIDE"; // unclicked off the skillbar
					for (auto [key, value] : skillbarEditPositions.internalMap) {
						if (graphics.isThisInsideRect(whereDidUserUnclick, value)) {
							unclickLocation = key;
						}
					}
					// moving from grid to bar
					if (imageSource == "FROMGRID" and unclickLocation != "OUTSIDE") {
						string skillName = split(imageThatWasDragged, "_").at(0);
						Combat::Skill skillDefinition = combat.skillDefinitions[skillName];
						if (skillDefinition.isElite()) {
							unclickLocation = "SKILLSLOT5";
						}
						if (skillDefinition.isElite() or (!skillDefinition.isElite() and unclickLocation != "SKILLSLOT5")) {
							string whichTargetSlot = SReplace(unclickLocation, "SKILLSLOT", "");
							if (!equippedSkills.getValues().contains(skillName)) {
								// not on skillbar so just replace that slot
								equippedSkills[whichTargetSlot] = skillName;
							}
							else {
								// it was already on the skillbar so rearrange
								string alreadyInWhichSlot = "";
								for (auto slot : equippedSkills.getKeys().internalList) {
									if (equippedSkills[slot] == skillName) { alreadyInWhichSlot = slot; break; }
								}
								equippedSkills.internalMap.erase(alreadyInWhichSlot);
								equippedSkills[whichTargetSlot] = skillName;
							}
						}
						skillbarNeedsToBeRedrawn = true;
					}
					// moving from skillbar to skillbar
					if (imageSource == "FROMSKILLBAR" and unclickLocation != "OUTSIDE") {
						string skillIndex = split(imageThatWasDragged, "_").at(2);
						string skillName = saveContainer.current.equippedSkills[who][skillIndex];
						string destinationSlot = SReplace(unclickLocation, "SKILLSLOT", "");
						Combat::Skill skillDefinition = combat.skillDefinitions[skillName];
						skillbarNeedsToBeRedrawn = true;
						if (!skillDefinition.isElite() and destinationSlot != "5") {
							if (equippedSkills.hasKey(destinationSlot)) {
								// swap two skills
								string toSwap = equippedSkills[destinationSlot];
								equippedSkills[destinationSlot] = skillName;
								equippedSkills[skillIndex] = toSwap;
							}
							else {
								// move the skill into the empty slot
								equippedSkills.internalMap.erase(skillIndex);
								equippedSkills[destinationSlot] = skillName;
							}
						}
					}
					if (imageSource == "FROMSKILLBAR" and unclickLocation == "OUTSIDE") {
						string skillIndex = split(imageThatWasDragged, "_").at(2);
						equippedSkills.internalMap.erase(skillIndex);
						skillbarNeedsToBeRedrawn = true;
					}
					if (skillbarNeedsToBeRedrawn) {
						Event("TeardownSkillBar", "TEARDOWNTHISSKILLBAR", Map<string, string>({pair<string, string>("who", who),})).run(*&gameEngine);
						Map<string, string> locs = Menu::getLocAndScaleOfSkillBarEdit();
						string scale = locs["scale"];
						string x = locs["x"];
						string y = locs["y"];
						saveContainer.current.equippedSkills[who] = equippedSkills.internalMap;
						Event("LoadSkillBar", "LOADSKILLBARHERE", Map<string, string>({
								pair<string, string>("who", who),
								pair<string, string>("scale", scale),
								pair<string, string>("x", x),
								pair<string, string>("y", y),
								pair<string, string>("full", "1"),
							})).run(*&gameEngine);
					}
					Map<string, string> equippedSkillTrees; equippedSkillTrees.internalMap = saveContainer.current.equippedSkillTrees[who];
					Map<string, string> xPositions = Menu::getSkillGridPositions();
					for (auto whichTree : equippedSkillTrees.getKeys().internalList) {
						Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
							pair<string, string>("offsetX", xPositions[whichTree]),
							pair<string, string>("offsetY", "20"),
							pair<string, string>("what", "SKILLS"),
							pair<string, string>("scale", "0.5"),
							pair<string, string>("skillTreeName",equippedSkillTrees[whichTree]),
							pair<string, string>("who", who),
							pair<string, string>("moveExisting", "1"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							})).run(*&gameEngine);
						Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
							pair<string, string>("offsetX", xPositions[whichTree]),
							pair<string, string>("offsetY", "20"),
							pair<string, string>("what", "SKILLBORDERS"),
							pair<string, string>("scale", "0.5"),
							pair<string, string>("skillTreeName",equippedSkillTrees[whichTree]),
							pair<string, string>("who", who),
							pair<string, string>("moveExisting", "1"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"] + 1)),
							})).run(*&gameEngine);
					}
				}
				graphics.recentlyFinishedBeingDragged.clear();
}
			if (type == "ADDEQUIPBUTTONSTOMENU") {
				string category = data["category"];
				string who = data["who"];
				Map<string, int> items;
				if (who != "") {
					items = combat.getInventoryItemsOfXCategory(category);
				}
				Map<int, pair<float, float>> positions = Menu::getItemGridPositions();
				List<Menu::Button> result = Menu::getDefaultButtonsForEquipmentSelect();
				if (category != "None") {
					result.at(1).buttonContent = "GUI_" + category;
				}
				for (int x = 0; x < items.getKeys().size(); x++) {
					string itemID = items.getKeys().at(x);
					Menu::Button current = Menu::smallButton("EQUIP_" + items.getKeys().at(x),"Item Names_" + itemID, positions[x]);
					current.extras["getColourFromEquipment"] = "1";
					current.extras["sayHowManyPlayerHas"] = "1";
					current.extras["itemName"] = itemID;
					current.extras["hasHoverText"] = "1";
					current.extras["hoverTextName"] = "explainEquipmentHover";
					current.extras["hoverTextContent"] = "explainEquipmentHover";
					current.extras["hoverTextContentX"] = "35";
					current.extras["hoverTextContentY"] = "15";
					current.extras["hoverAnchorStyle"] = "TOPLEFT";
					current.extras["hoverTextFormat"] =  "Centaur_12",
					result.push_back(current);
				}
				gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttonReplace(result);
				return true;
			}
			if (type == "ADDMERCHANTITEMBUTTONS") {
				string whichMerchant = data["whichMerchant"];
				string mode = gameEngine.storedMenus["MERCHANT"].data["MERCHANT"];
				List<Menu::Button> toHandle = gameEngine.storedMenus["MERCHANT"].buttons.getValues();
				for (auto button : toHandle.internalList) {
					if (button.uniqueID != "MERCHANTDIALOGUE") {
						Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
							pair<string,string>("textID", button.textID),
							pair<string,string>("imageID", button.imageID),
							})).run(*&gameEngine);
					}
				}
				List<Menu::Button> result = Menu::getDefaultButtonsForMerchant();
				List<string> toAdd;
				if (mode == "Buy") {
					toAdd = merchants.merchantDefinitions[whichMerchant].getCorrectItemsForSale();
				}
				if (mode == "Sell") {
					for (auto const& [key, val] : saveContainer.current.inventory) {
						if (val > 0) {
							toAdd.push_back(key);
						}
					}
				}
				if (mode == "Buyback") {
					for (auto const& [key, val] : saveContainer.current.itemsSold) {
						if (val > 0) {
							toAdd.push_back(key);
						}
					}
				}
				Map<int, pair<float, float>> positions = Menu::getMerchantGridPositions();
				int currentScrollIndex = gameEngine.storedMenus["MERCHANT"].currentScrolls[mode];
				int maxItemButtons = positions.getKeys().size();
				
				gameEngine.storedMenus["MERCHANT"].data["scrollingLimit_" + mode] = to_string(toAdd.size() - maxItemButtons);
				int counter = 0;
				for (int x = currentScrollIndex; x < currentScrollIndex + maxItemButtons and x < toAdd.size(); x++) {
					Combat::Equipment def = combat.equipmentDefinitions[toAdd.at(x)];
					string itemID = toAdd.at(x);
					Menu::Button current = Menu::smallButton(mode + "_" + toAdd.at(x), "Item Names_" + itemID, positions[counter]);
					current.extras["getColourFromEquipment"] = "1";
					current.extras["addPriceToEquipment"] = "1";
					current.extras["itemName"] = itemID;
					current.extras["merchantExplain"] = "1";
					current.extras["hasHoverText"] = "1";
					current.extras["hoverTextName"] = "explainEquipmentHover";
					current.extras["hoverTextOneLine"] = "1";
					current.extras["hoverTextContent"] = "explainEquipmentHover";
					current.extras["hoverTextContentX"] = "50";
					current.extras["hoverTextContentY"] = "26";
					current.extras["hoverAnchorStyle"] = "CENTRE";
					current.extras["hoverTextFormat"] = "Centaur_13";
					current.scrollable = true;
					current.extras["whichScroll"] = mode;
					result.push_back(current);
					counter++;
				}
				string layer = to_string(imageLookup.layerDefaults["BUTTONS"]);
				if (data["reset"] == "1") {
					for (auto button : gameEngine.storedMenus["MERCHANT"].buttons.getKeys().internalList) {
						if (button.find("_") != -1 and split(button, "_").at(0) == mode) {
							gameEngine.storedMenus["MERCHANT"].removeThisButton(button);
						}
					}
				}
				gameEngine.storedMenus["MERCHANT"].buttonReplace(result);
				for (auto button : result.internalList) {
					if (button.uniqueID == "MERCHANTDIALOGUE") { continue; }
					Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
						pair<string, string>("uniqueID", "MERCHANT"),
						pair<string, string>("which",button.uniqueID),
						pair<string, string>("layer", layer),
						pair<string, string>("format", button.extras["format"]),
						})).run(*&gameEngine);
				}
			}
			if (type == "HANDLECHESTANIMATION") {
				string imageName = data["imageID"];
				string chestImageSource = data["ChestImageSource"];
				Graphics::Image* theImage = graphics.accessImageViaUniqueID(imageName);
				if (theImage->action == "STAND") {
					theImage->action = "ACTION";
					theImage->resetSources(*&graphics, imageLookup.animationFrames[chestImageSource]["ACTION_FRONT"]);
					theImage->frame = 0;
					theImage->animationStyles.clear();
					theImage->animationStyles.push_back("SINGLE");
					theImage->animationSpeed = 100;
					CLOCK.startClock(imageName + "_CHESTWAIT");
				}
				if (theImage->frame == theImage->sources.size() - 1 and CLOCK.hasEnoughTimePassed(imageName + "_CHESTWAIT", 1400)) {
					return true;
				}
				
				return false;
			}
			if (type == "MOVECAMERA") {
				// moving the camera independently of the player
				explorer.perspective = "FOLLOW_CAMERA";
				int speed = stoi(data["speed"]);
				float unitOfMovement = explorer.unitOfMovement / 2;
				float ignore = -1; // use this to move camera only along x or y
				pair<float, float> currentPosition = explorer.activeCamera.position;
				pair<float, float> targetPosition = { stof(data["x"]), stof(data["y"]) };
				bool reachedDestination = true;
				if (CLOCK.hasEnoughTimePassed("CAMERAMOVE", speed)) {
					if (targetPosition.first != ignore) {
						if (abs(currentPosition.first - targetPosition.first) <= unitOfMovement) {
							currentPosition.first = targetPosition.first;
						}
						if (currentPosition.first < targetPosition.first) {
							currentPosition.first += unitOfMovement;
						}
						if (currentPosition.first > targetPosition.first) {
							currentPosition.first -= unitOfMovement;
						}
						if (currentPosition.first != targetPosition.first) {
							reachedDestination = false;
						}
					}
					if (targetPosition.second != ignore) {
						if (abs(currentPosition.second - targetPosition.second) <= unitOfMovement) {
							currentPosition.second = targetPosition.second;
						}
						if (currentPosition.second < targetPosition.second) {
							currentPosition.second += unitOfMovement;
						}
						if (currentPosition.second > targetPosition.second) {
							currentPosition.second -= unitOfMovement;
						}
					}
					if (currentPosition.second != targetPosition.second) {
						reachedDestination = false;
					}
					explorer.activeCamera.position = currentPosition;
					Event("UpdateMap", "MAPMOVE", {}).run(*&gameEngine);
					return reachedDestination;
				}
				return false;
			}
			if (type == "MOVEPLAYERCUTSCENE") {
				pair<float, float> currentPosition = explorer.playerOnMap.position;
				pair<float, float> targetPosition = { stof(data["targetX"]), stof(data["targetY"]) };
				pair<float, float> distance = {abs(currentPosition.first-targetPosition.first), abs(currentPosition.second-targetPosition.second)};
				bool needToChangeDirection = true;
				if ((distance.first < 1 and targetPosition.first !=-1) or (distance.second < 1 and targetPosition.second != -1)) {
					needToChangeDirection = false;
				}
				string audioName = data["audio"];
				int speed = stoi(data["speed"]);
				if (targetPosition.first == -1) {
					targetPosition.first = currentPosition.first;
				}
				if (targetPosition.second == -1) {
					targetPosition.second = currentPosition.second;
				}
				string objectName = "PLAYER";
				List<int> sources;
				List<int> shadowSources;
				string character = saveContainer.getPossibleCutsceneParticipants().front();
				if (!graphics.doesThisImageAlreadyExist(character + "_Explore")) {
					// some cutscenes use the player as an invisible camera so we still want to access them even if they are "not in the cutscene"
					character = saveContainer.getCurrentMainCharacter();
				}
				string newDirection = explorer.decideDirectionDependingOnTwoPoints(currentPosition, targetPosition);
				int animationSpeed = explorer.getAnimationSpeeds()["WALK"];
				if (newDirection != "" and needToChangeDirection) {
					sources = imageLookup.animationFrames[character]["WALK_" + newDirection];
					shadowSources = imageLookup.animationFrames["Shadow " + character]["WALK_" + newDirection];
					if (sources.size() == 0) {
						throw exception("There were no image sources to use!");
					}
					for (auto const& x : { graphics.accessImageViaUniqueID(character + "_Explore"), graphics.accessImageViaUniqueID(character + "_Shadow") }) {
						x->action = "WALK";
						x->animationSpeed = animationSpeed;
					}
					graphics.accessImageViaUniqueID(character + "_Explore")->direction = newDirection;
					graphics.accessImageViaUniqueID(character + "_Shadow")->direction = newDirection;
					graphics.accessImageViaUniqueID(character + "_Explore")->resetSources(*&graphics, sources);
					graphics.accessImageViaUniqueID(character + "_Shadow")->resetSources(*&graphics, shadowSources);
				}
				float unitOfMovement = explorer.unitOfMovement / 2;
				if (speed == 0) {
					explorer.playerOnMap.position = targetPosition;
					Event("UpdateMap", "MAPMOVE", {}).run(*&gameEngine);
				}
				else {
					if (CLOCK.hasEnoughTimePassed("MOVEOBJECTSON", speed)) {
						explorer.playerOnMap.position = explorer.moveLHSCloserToRHSByValue(currentPosition, targetPosition, false, false, unitOfMovement, unitOfMovement);
						if (CLOCK.hasEnoughTimePassed("PLAYERPLAYAUDIO", 500) and audioName != "NOAUDIO") {
							Event("Audio", "PLAYSFX", Map<string, string>({
											pair<string, string>("audio", audioName) })).run(*&gameEngine);
						}
						Event("UpdateMap", "MAPMOVE", {}).run(*&gameEngine);
					}
				}
				if (currentPosition == targetPosition) {
					int animationSpeed = explorer.getAnimationSpeeds()["STAND"];
					string direction = graphics.accessImageViaUniqueID(character + "_Explore")->direction;
					sources = imageLookup.animationFrames[character]["STAND_" + direction];
					shadowSources = imageLookup.animationFrames["Shadow " + character]["STAND_" + direction];
					for (auto const& x : { graphics.accessImageViaUniqueID(character + "_Explore"), graphics.accessImageViaUniqueID(character + "_Shadow") }) {
						x->action = "STAND";
						x->animationSpeed = animationSpeed;
					}
					graphics.accessImageViaUniqueID(character + "_Explore")->resetSources(*&graphics, sources);
					graphics.accessImageViaUniqueID(character + "_Shadow")->resetSources(*&graphics, shadowSources);
					
					return true;
				}
				return false;
			}
			if (type == "CHANGECAMERAOPERTATION") {
				if (data["followPlayer"] == "1") {
					explorer.perspective = "FOLLOW_PLAYER";
				}
				else {
					explorer.perspective = "FOLLOW_CAMERA";
				}
				Event("Update", "MAPMOVE", {}).run(*&gameEngine);
				return true;
			}
			if (type == "TELEPORTCAMERA") {
				pair<float, float> position = {stof(data["x"]), stof(data["y"])};
				explorer.activeCamera.position = position;
				Event("Update", "MAPMOVE", {}).run(*&gameEngine);
				return true;
			}
			if (type == "MOVEOBJECTS") {
				List<string> objectsToMove = split(data["whichObjects"], "_");
				List<pair<float, float>> targets;
				List<string> audioToPlay;
				int speed = stoi(data["speed"]);
				for (auto node : split(data["targetPositions"], "_").internalList) {
					List<string> current = split(node, ",");
					pair<float, float> currentPos = { stof(current.at(0)), stof(current.at(1)) };
					targets.push_back(currentPos);
					audioToPlay.push_back(current.at(2));
				}
				Map<string, pair<float, float>> currentPositions; 
				Map<string, string> objectAudio;
				Map<string, pair<float, float>> allObjectPositions = explorer.getPositionsOfAllObjects();
				for (auto object : objectsToMove.internalList) {
					if (object == "CAMERA") { 
						explorer.perspective = "FOLLOW_CAMERA";
						continue; }
					currentPositions[object] = allObjectPositions[object];
				}
				Map<string, pair<float, float>> targetPositions;
				for (int x = 0; x < objectsToMove.size(); x++) {
					targetPositions[objectsToMove.at(x)] = targets.at(x);
					objectAudio[objectsToMove.at(x)] = audioToPlay.at(x);
				}

				bool allFinished = true;
				float unitOfMovement = explorer.unitOfMovement / 2;
				float ignore = -1; // use this to move camera only along x or y

				for (auto objectName : objectsToMove.internalList) {
					pair<float, float> currentPosition = currentPositions[objectName];
					pair<float, float> targetPosition = targetPositions[objectName];
					if (objectName == "PLAYER1" or objectName == "Shadow PLAYER1") {
						currentPosition = explorer.playerOnMap.position;
						}
					if (objectName == "CAMERA") {
						currentPosition = explorer.activeCamera.position;
					}
					if (!explorer.areThesePointsInSamePlace(currentPosition, targetPosition, (targetPosition.first == ignore), (targetPosition.second == ignore))) {
						allFinished = false;
					}
				}
				if (allFinished) {
					return true;
				}

				if (CLOCK.hasEnoughTimePassed("MOVEOBJECTSON", speed)) {
					for (auto objectName : objectsToMove.internalList) {
						if (objectName == "CAMERA") {
							Event("MoveCamera", "MOVECAMERA", Map<string, string>({
								pair<string, string>("x", to_string(targetPositions[objectName].first)),
								pair<string, string>("y", to_string(targetPositions[objectName].second)),
								pair<string, string>("speed", data["speed"]),
								})).run(*&gameEngine);
							continue;
						}

						if (objectName == "PLAYER1") {
							pair<float, float> current = explorer.playerOnMap.position;
							pair<float, float> target = targetPositions["PLAYER1"];
							explorer.playerOnMap.position = explorer.moveLHSCloserToRHSByValue(current, target, (targetPositions[objectName].first == ignore), (targetPositions[objectName].second == ignore), unitOfMovement, unitOfMovement);
						}
						else {
							if (explorer.currentMap.doesAnObjectWithThisNameExist(objectName)) {
								currentPositions[objectName] = explorer.moveLHSCloserToRHSByValue(currentPositions[objectName], targetPositions[objectName], (targetPositions[objectName].first == ignore), (targetPositions[objectName].second == ignore), unitOfMovement, unitOfMovement);
								if (abs(currentPositions[objectName].first - targetPositions[objectName].first) < unitOfMovement) {
									currentPositions[objectName].first = targetPositions[objectName].first;
								}
								if (abs(currentPositions[objectName].second - targetPositions[objectName].second) < unitOfMovement) {
									currentPositions[objectName].second = targetPositions[objectName].second;
								}
							}
						}
						if (CLOCK.hasEnoughTimePassed(objectName + "PLAYAUDIO", 500)) {
							string audioName = objectAudio[objectName];
							if (audioName != "NOAUDIO") {
								Event("Audio", "PLAYSFX", Map<string, string>({
									pair<string, string>("audio", audioName) })).run(*&gameEngine);
							}
						}
						for (int index = 0; index < explorer.currentMap.objects.size(); index++) {
							if (explorer.currentMap.objects.at(index).name == objectName) {
								explorer.currentMap.objects.at(index).positionOnMap = currentPositions[objectName];
							}
						}
					}
				}

				Event("UpdateMap", "MAPMOVE", {}).run(*&gameEngine);
   				return false;
			}
			if (type == "LOADOBJECT") {
				string name = data["name"];
				string animationSpeed = data["animationSpeed"];
				if (data["sources_1"].find("PLAYER2") != -1) {
					data["sources_1"] = SReplace(data["sources_1"], "PLAYER2", saveContainer.getNamesOfTwoPeopleForScene().second);
				}

				string sources = imageLookup.getSequenceAsString(data["sources_1"], data["sources_2"] + "_" + data["sources_3"]);
				string layer = data["layer"];
				string x = data["x"];
				string y = data["y"];
				string opacity = data["opacity"];
				bool followPlayer = data["followPlayer"] == "1";
				bool useScreenNotMapForLoc = data["followPlayer"] == "2";
				string anchor = data["anchor"];
				string scale = "1.0";
				if (data.hasKey("scale")) {
					scale = data["scale"];
				}

				if (useScreenNotMapForLoc) {
					// this object is not on the map so don't add it as an object
					Event("Load" + name, "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources",sources),
						pair<string, string>("x", x),
						pair<string, string>("y", y),
						pair<string, string>("anchor", anchor),
						pair<string, string>("layer", layer),
						pair<string, string>("animated", "1"),
						pair<string, string>("animation_speed", animationSpeed),
						pair<string, string>("styles", "LOOP"),
						pair<string, string>("scale", scale),
						pair<string, string>("opacity", opacity),
						pair<string, string>("uniqueID", name), }))).run(*&gameEngine);
					return true;
				}

				Explorer::mapObject objectToAdd(name, false, true, followPlayer, sources, "1", stoi(animationSpeed), stoi(layer), "1.0", scale, anchor, {stof(x), stof(y)}, false, {}, {});
				explorer.currentMap.objects.push_back(objectToAdd);
				Event("Load" + name, "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources",sources),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", anchor),
						pair<string, string>("layer", layer),
						pair<string, string>("animated", "1"),
						pair<string, string>("animation_speed", animationSpeed),
						pair<string, string>("styles", "LOOP"),
						pair<string, string>("scale", scale),
						pair<string, string>("opacity", opacity),
						pair<string, string>("uniqueID", name), }))).run(*&gameEngine);
				Event("UpdateMap", "MAPMOVE", {}).run(*&gameEngine);
				return true;
			}
			if (type == "STARTCLOCK") {
				CLOCK.startClock(data["uniqueID"]);
				return true;
			}
			if (type == "ANIMATEIMAGEONMAP") {
				string uniqueID = data["whichImage"];
				string character = data["character"];
				string action = data["action"];
				string direction = data["direction"];

				Graphics::Image* image = NULL;

				if (uniqueID.find("PLAYER2") != -1) {
					if (uniqueID == "PLAYER2") {
						image = graphics.accessPlayerImage();
						character = SReplace(character, "PLAYER2", saveContainer.getNamesOfTwoPeopleForScene().second);
					}
					if (uniqueID == "Shadow PLAYER2") {
						image = graphics.accessPlayerShadowImage();
						character = SReplace(character, "PLAYER2", saveContainer.getNamesOfTwoPeopleForScene().second);
					}
				}
				
				if (uniqueID.find("PLAYER1") != -1) {
					if (uniqueID == "PLAYER1") {
						image = graphics.accessPlayerImage();
						character = SReplace(character, "PLAYER1", saveContainer.getPossibleCutsceneParticipants().front());
					}
					if (uniqueID == "Shadow PLAYER1") {
						image = graphics.accessPlayerShadowImage();
						character = SReplace(character, "PLAYER1", saveContainer.getPossibleCutsceneParticipants().front());
					}
				}
				else {
					image = graphics.accessImageViaUniqueID(uniqueID);
				}
				List<int> sources = imageLookup.animationFrames[character][action + "_" + direction];
				image->resetSources(*&graphics, sources);
				image->action = action;
				int fillInSpeed = explorer.getAnimationSpeeds()[action];
				if (fillInSpeed != 0) {
					image->animationSpeed = fillInSpeed;
				}
				image->direction = direction;
				Event("UpdateMap", "MAPMOVE", {}).run(*&gameEngine);
				return true;
			}
			if (type == "RUNPUZZLELOGIC") {
				bool playAudio = false;
				if (CLOCK.hasEnoughTimePassed("PuzzleAudio", 100)) {
					playAudio = true;
				}
				string whichPuzzle = data["whichPuzzle"];
				if (whichPuzzle == "WATERPUZZLE1") {
					if (playAudio) {
						Event("audio", "PLAYSFX", List<pair<string, string>>({
								pair<string, string>("audio","7261"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.0"),
							})).run(*&gameEngine);
					}
					Map<string, string> puzzleStatus = explorer.puzzleContainer.loadCurrentWaterPuzzle1Status();
					if (puzzleStatus["WaterPuzzleActivated"] == "OFF") {
						gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "WaterPuzzle1NoWater", saveContainer.getCurrentMainCharacter(), "EXPLORE");
						return false;
					}
					Explorer::mapObject whichObject = explorer.getObjectsThatAreClose().at(0);
					pair<Map<string, string>, string> results = explorer.puzzleContainer.runWaterPuzzle1Logic(whichObject.name);
					List<Explorer::mapObject> fontDefinitions = explorer.puzzleContainer.getWaterPuzzle1({ 50,35 });
					if (results.second == "NoMoreWater") {
						gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "WaterPuzzle1NoMoreSwitches", saveContainer.getCurrentMainCharacter(), "EXPLORE");
						return false;
					}
					if (results.second == "Broken") {
						gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "WaterPuzzle1Broken", saveContainer.getCurrentMainCharacter(), "EXPLORE");
						return false;
					}
					if (results.second == "ON" and playAudio) {
						Event("audio", "PLAYSFX", List<pair<string, string>>({
							pair<string, string>("audio","7262"),
							pair<string, string>("direct","1"),
							pair<string, string>("delay","0.0"),
							})).run(*&gameEngine);
					} 
					for (auto object : fontDefinitions.internalList) {
						string objectName = object.name;
						if (objectName.find("FONT") == -1) { 
							continue; }
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(objectName);
						string character = object.data["character"];
						string direction = object.data["direction"];
						string action = results.first[objectName];
						List<int> sources = imageLookup.getSequence(character, action + "_" + direction);
						if (sources.empty()) {
							throw exception("Can't have an image with no sources.");
						}
						theImage->resetSources(*&graphics, sources);
						for (int x = 0; x < explorer.currentMap.objects.size(); x++) {
							if (objectName == explorer.currentMap.objects.at(x).name) {
								explorer.currentMap.objects.at(x).imageSources = imageLookup.getSequenceAsString(character, action + "_" + direction);
							}
						}
					}
					if (results.first["FONT9"] == "ON") { // main puzzle finished
						gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "WaterPuzzleFinished1", saveContainer.getCurrentMainCharacter(), "EXPLORE");
						return false;
					}
					if (results.first["FONT15"] == "ON") { // main puzzle finished
						saveContainer.current.flags["WaterPuzzleSecretFinished"] = true;
					}
				}
				return true;
			}
			if (type == "CHANGEANIMATIONSPEED") {
				int speed = stoi(data["animationSpeed"]);
				List<string> styles = split(data["styles"], "_");
				for (auto uniqueID : split(data["uniqueID"], "_").internalList) {
					Graphics::Image* theImage = NULL;
					if (uniqueID.find("PLAYER1") != -1) {
						if (uniqueID == "PLAYER1") {
							theImage = graphics.accessPlayerImage();
						}
						if (uniqueID == "Shadow PLAYER1") {
							theImage = graphics.accessPlayerShadowImage();
						}
					}
					else {
						theImage = graphics.accessImageViaUniqueID(uniqueID);
					}
					if (theImage == NULL) {
						return true;
					}
					theImage->animationSpeed = stoi(data["animationSpeed"]);
					theImage->animationStyles = styles;
					if (data["styles"] != "FADEOUT") {
						theImage->frame = 0; // if just fade out, don't change frame just stay on frame and fade out
					}
					
				}
				return true;
			}
			if (type == "WAITFORANIMATION") {
				string uniqueID = data["uniqueID"];
				uniqueID = SReplace(uniqueID, "PLAYER1", saveContainer.getPossibleCutsceneParticipants().front());
				Graphics::Image* theImage = graphics.accessImageViaUniqueID(uniqueID);
				if (theImage == NULL) {
					return true;
				}
				return theImage->hasThisFinishedAnimating();
			}
			if (type == "STARTCUTSCENEDIRECTLY") {
				string cutsceneName = data["cutsceneName"];
				gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, cutsceneName, saveContainer.getPossibleCutsceneParticipants().front(), "EXPLORE");
				return false;
			}
			if (type == "FROMCUTSCENETOMENU") {
				string menuName = data["menuName"];
				gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure(menuName);
				return false;
			}
			if (type == "CHANGEOPACITY") {
				string whichImage = data["uniqueID"];
				string opacity = data["opacity"];

				string charImage = saveContainer.getPossibleCutsceneParticipants().front() + "_Explore";
				string shadowImage = saveContainer.getPossibleCutsceneParticipants().front() + "_Shadow";

				if (!graphics.doesThisImageAlreadyExist(charImage)) {
					// some cutscenes use the player as an invisible camera so we still want to access them even if they are "not in the cutscene"
					charImage = saveContainer.getCurrentMainCharacter() + "_Explore";
					shadowImage = saveContainer.getCurrentMainCharacter() + "_Shadow";
				}

				if (whichImage == "PLAYER1") {
					whichImage = charImage;
				}
				if (whichImage == "PLAYER1 Shadow") {
					whichImage = shadowImage;
				}

				graphics.accessImageViaUniqueID(whichImage)->opacity = stof(opacity);
				return true;
			}
			if (type == "TELEPORTOBJECT") {
				string whichObject = data["uniqueID"];
				pair<float, float> position = {stof(data["x"]), stof(data["y"])};
				for (int index = 0; index < explorer.currentMap.objects.size(); index++) {
					if (explorer.currentMap.objects.at(index).name == whichObject) {
						explorer.currentMap.objects.at(index).positionOnMap = position;
					}
				}
				return true;
			}
			if (type == "REMOVEIMAGES") {
				List<string> whichObjects = split(data["toRemove"], "$");
				for (auto imageName : whichObjects.internalList) {
					Event("Teardown", "TEARDOWNIMAGE", Map<string, string>({ 
						pair<string, string>("uniqueID", imageName),
						})).run(*&gameEngine);
					for (int index = 0; index < explorer.currentMap.objects.size(); index++) {
						if (explorer.currentMap.objects.at(index).name == imageName) {
							explorer.currentMap.objects.at(index).visible = false;
						}
					}
				}
				return true;
			}
			if (type == "HANDLESKILLANIMATION") {
				string target = data["target"];
				string caster = data["caster"];
				string skillName = data["skill"];
				Map<string, string> extras;
				for (auto s : split(data["extras"], "_").internalList) {
					extras[split(s, "%").at(0)] = split(s, "%").at(1);
				}
				return gameEngine.skillAnimationContainer.handleAnimation(*&gameEngine, graphics.accessImageViaUniqueID(caster), graphics.accessImageViaUniqueID(target), skillName, extras);
			}
			if (type == "SETUPBATTLE") {
				string team1source = data["team1"];
				string team1alliessource = data["team1allies"];
				string team2source = data["team2"];
				string team2alliessource = data["team2allies"];
				string backgroundImage = data["background"];
				string song = data["song"];
				string postBattle = data["postBattle"];
				data["opponentName"] = combat.definedTeams[team2source].first;
				data["language"] = gameEngine.language;

				List<Combat::Combatant> team1 = combat.getThisTeam(team1source, "TEAM1");
				List<Combat::Combatant> team1allies = combat.getThisTeam(team1alliessource, "TEAM1_ALLIES");
				List<Combat::Combatant> team2 = combat.getThisTeam(team2source, "TEAM2");
				List<Combat::Combatant> team2allies = combat.getThisTeam(team2alliessource, "TEAM2_ALLIES");

				Map<string, Map<string, pair<float, float>>> combatPositions = combat.getCombatantPositionLookup();
				combat.setUpBattle(team1, team1allies, team2, team2allies, data);

				Event("LoadImages", "RELOADALLCOMBATANTIMAGES", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("mode", "SETUP_HIDEBARS"),
					}))).run(*&gameEngine);
				Event("Background", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", backgroundImage),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", "0"),
						pair<string, string>("scale", "1.1"),
						pair<string, string>("uniqueID", "BattleBackground"),
							}))).run(*&gameEngine);
				Event("FilmGrain", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT")),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["ENVIRONMENT"])),
						pair<string, string>("scale", "1.1"),
						pair<string, string>("uniqueID", "FilmGrain"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "LOOP"),
						pair<string, string>("animation_speed", "1"),
					}))).run(*&gameEngine);
				CLOCK.startClock("CombatStartWait");
				CLOCK.startClock("PlayingTheDyingSound");
				return true;
			}
			if (type == "RELOADALLCOMBATANTIMAGES") {
				string mode = data["mode"];

				List<Combat::CombatantInstance *> team1 = combat.currentBattle->party1;
				List<Combat::CombatantInstance*> team1allies = combat.currentBattle->party1allies;
				List<Combat::CombatantInstance*> team2 = combat.currentBattle->party2;
				List<Combat::CombatantInstance*> team2allies = combat.currentBattle->party2allies;
				Map<string, Map<string, pair<float, float>>> combatPositions = combat.getCombatantPositionLookup();

				for (int x = 0; x < team1.size(); x++) {
					pair<float, float> pos = combatPositions["TEAM1_" + to_string(team1.size())][to_string(x+1)];
					Event("LoadImages", "LOADIMAGESFORCOMBATANT", Map<string, string>(List<pair<string, string>>({
							pair<string, string>("sources", team1.at(x)->c.data["Images"]["Back"]),
							pair<string, string>("x", to_string(pos.first)),
							pair<string, string>("y", to_string(pos.second)),
							pair<string, string>("scale", combat.layerScaleLookup["TEAM1"]),
							pair<string, string>("mode", mode),
							pair<string, string>("sizeOfTeam", to_string(team1.size())),
							pair<string, string>("uniqueID", team1.at(x)->c.uniqueCombatID),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["COMBATTEAM1"])),
						}))).run(*&gameEngine);
				}
				for (int x = 0; x < team2.size(); x++) {
					pair<float, float> pos = combatPositions["TEAM2_" + to_string(team2.size())][to_string(x + 1)];
					Event("LoadImages", "LOADIMAGESFORCOMBATANT", Map<string, string>(List<pair<string, string>>({
							pair<string, string>("sources", team2.at(x)->c.data["Images"]["Front"]),
							pair<string, string>("x", to_string(pos.first)),
							pair<string, string>("y", to_string(pos.second)),
							pair<string, string>("scale", combat.layerScaleLookup["TEAM2"]),
							pair<string, string>("mode", mode),
							pair<string, string>("sizeOfTeam", to_string(team2.size())),
							pair<string, string>("uniqueID", team2.at(x)->c.uniqueCombatID),
							pair<string, string>("layer",to_string(imageLookup.layerDefaults["COMBATTEAM2"])),
						}))).run(*&gameEngine);
				}
				for (int x = 0; x < team1allies.size(); x++) {
					pair<float, float> pos = combatPositions["TEAM1_ALLIES"][to_string(x + 1)];
					Event("LoadImages", "LOADIMAGESFORCOMBATANT", Map<string, string>(List<pair<string, string>>({
							pair<string, string>("sources", team1allies.at(x)->c.data["Images"]["Back"]),
							pair<string, string>("x", to_string(pos.first)),
							pair<string, string>("y", to_string(pos.second)),
							pair<string, string>("scale", combat.layerScaleLookup["TEAM1ALLIES"]),
							pair<string, string>("sizeOfTeam", to_string(team1allies.size())),
							pair<string, string>("uniqueID", team1allies.at(x)->c.uniqueCombatID),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["COMBATTEAM1ALLIES"])),
							pair<string, string>("mode", mode),
						}))).run(*&gameEngine);
				}
				for (int x = 0; x < team2allies.size(); x++) {
					pair<float, float> pos = combatPositions["TEAM2_ALLIES"][to_string(x + 1)];
					Event("LoadImages", "LOADIMAGESFORCOMBATANT", Map<string, string>(List<pair<string, string>>({
							pair<string, string>("sources", team2allies.at(x)->c.data["Images"]["Front"]),
							pair<string, string>("x", to_string(pos.first)),
							pair<string, string>("y", to_string(pos.second)),
							pair<string, string>("scale", combat.layerScaleLookup["TEAM2ALLIES"]),
							pair<string, string>("uniqueID", team2allies.at(x)->c.uniqueCombatID),
							pair<string, string>("sizeOfTeam", to_string(team2allies.size())),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["COMBATTEAM2ALLIES"])),
							pair<string, string>("mode", mode),
						}))).run(*&gameEngine);
				}
				return false;
			}
			if (type == "LOADIMAGESFORCOMBATANT") {
				string sources = data["sources"];
				string x = data["x"];
				string y = data["y"];
				string uniqueIDBase = data["uniqueID"];
				string baseLayer = data["layer"];
				string barback_life = uniqueIDBase + "_BARBACKLIFE";
				string barback_mana = uniqueIDBase + "_BARBACKMANA";
				string life = uniqueIDBase + "_LIFE";
				string mana = uniqueIDBase + "_MANA";
				string scale = data["scale"];
				string mode = data["mode"];

				if (data["sizeOfTeam"] == "1") {
					x = to_string(stoi(x) - 10);
				}

				Combat::CombatantInstance* actor = combat.currentBattle->getThisCombatant(uniqueIDBase);
				if (actor->c.isDead()) {
					return false;
				}
				float lifeProportion = actor->c.getLifeAsPercentage();
				float manaProportion = actor->c.getEnergyAsPercentage();

				if (mode.find("SETUP") != -1) {
					Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", sources),
					pair<string, string>("x", x),
					pair<string, string>("y", y),
					pair<string, string>("anchor", "CENTRE"),
					pair<string, string>("opacity", "1.0"),
					pair<string, string>("layer", baseLayer),
					pair<string, string>("scale", scale),
					pair<string, string>("uniqueID", uniqueIDBase),
						}))).run(*&gameEngine);
				}
				
				string opacity = "1.0";
				if (mode.find("HIDEBARS") != -1) {
					opacity = "0.0";
				}

				for (auto image : List<string>({barback_life,life,barback_mana, mana}).internalList) {
					// tear down more images when they have effects on them
					Event("Teardown", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string> ("uniqueID", image),
						}))).run(*&gameEngine);
				}
				Map<int, pair<float, float>> effectPositions = Menu::getEffectIconPositions(graphics.accessImageViaUniqueID(uniqueIDBase)->positionAsPercentage);
				int effectCount = 0;
				List <Combat::EffectObjectInstance*> effectObjectInstances = combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(uniqueIDBase);

				for (Combat::EffectObjectInstance * effect : effectObjectInstances.internalList) {
					string imageName = uniqueIDBase + "$" + effect->e.uniqueID + "$COMBATEFFECT";
					string borderName = uniqueIDBase + "$" + effect->e.uniqueID + "$COMBATBORDER";
					for (auto image : { imageName, borderName }) {
						Event("Teardown", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("uniqueID", image),
							}))).run(*&gameEngine);
					}
					int imageSource = combat.allEffectDefinitions[effect->e.uniqueID].imageRes;
					int borderSource = combat.allEffectDefinitions[effect->e.uniqueID].getBorder();
					
					Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", to_string(imageSource)),
						pair<string, string>("x", to_string(effectPositions[effectCount].first)),
						pair<string, string>("y", to_string(effectPositions[effectCount].second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", opacity),
						pair<string, string>("layer", to_string(stoi(baseLayer) + 1)),
						pair<string, string>("scale", "0.2"),
						pair<string, string>("uniqueID", imageName),
						}))).run(*&gameEngine);
					Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", to_string(borderSource)),
						pair<string, string>("x", to_string(effectPositions[effectCount].first)),
						pair<string, string>("y", to_string(effectPositions[effectCount].second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", opacity),
						pair<string, string>("layer", to_string(stoi(baseLayer) + 2)),
						pair<string, string>("scale", "0.2"),
						pair<string, string>("uniqueID", borderName),
						}))).run(*&gameEngine);

					if (opacity == "0.0") {
						graphics.tearDownSpecifiedText(imageName);
					}
					else {
						string duration = "";
						if (effect->e.infinite or effect->e.roundsLeft > 99) {
							duration = "$INFINITYSYMBOL$";
						}
						else {
							duration = to_string(effect->e.roundsLeft+1);
						}
						Event("DrawText", "DRAWTEXT", Map<string, string>({
							pair<string, string>("message",duration),
							pair<string, string>("format", "HighTowerText_12"),
							pair<string, string>("anchorStyle", "CENTRE"),
							pair<string, string>("x", to_string(effectPositions[effectCount].first)),
							pair<string, string>("y", to_string(effectPositions[effectCount].second)),
							pair<string, string>("w", "50"),
							pair<string, string>("h", "50"),
							pair<string, string>("colour", "WHITE"),
							pair<string, string>("shadowColour", "DARKBROWN"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("uniqueID", imageName),
							pair<string, string>("direct", "1"),
							})).run(*&gameEngine);
					}
					effectCount++;
				}

				Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("sources", to_string(BARBACK)),
				pair<string, string>("x", to_string(stof(x) - 6.25)),
				pair<string, string>("y", to_string(stof(y) + 10)),
				pair<string, string>("anchor", "BOTTOMCENTRE"),
				pair<string, string>("opacity", opacity),
				pair<string, string>("layer", to_string(stoi(baseLayer) + 1)),
				pair<string, string>("scale", scale),
				pair<string, string>("uniqueID", barback_life),
					}))).run(*&gameEngine);
				Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", to_string(LIFEBAR)),
					pair<string, string>("x", to_string(stof(x) - 6.25)),
					pair<string, string>("y", to_string(stof(y) + 10)),
					pair<string, string>("anchor", "BOTTOMCENTRE"),
					pair<string, string>("opacity", opacity),
					pair<string, string>("layer", to_string(stoi(baseLayer) + 2)),
					pair<string, string>("scale", scale),
					pair<string, string>("yStretch", to_string(lifeProportion)),
					pair<string, string>("uniqueID", life),
					}))).run(*&gameEngine);
				Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", to_string(BARBACK)),
					pair<string, string>("x", to_string(stof(x) -6)),
					pair<string, string>("y", to_string(stof(y) + 10)),
					pair<string, string>("anchor", "BOTTOMCENTRE"),
					pair<string, string>("opacity", opacity),
					pair<string, string>("layer", to_string(stoi(baseLayer) + 1)),
					pair<string, string>("scale", scale),
					pair<string, string>("uniqueID", barback_mana),
					}))).run(*&gameEngine);
				Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", to_string(MANABAR)),
					pair<string, string>("x", to_string(stof(x) -6)),
					pair<string, string>("y", to_string(stof(y) + 10)),
					pair<string, string>("anchor", "BOTTOMCENTRE"),
					pair<string, string>("opacity", opacity),
					pair<string, string>("layer", to_string(stoi(baseLayer) + 2)),
					pair<string, string>("scale", scale),
					pair<string, string>("yStretch", to_string(manaProportion)),
					pair<string, string>("uniqueID", mana),
					}))).run(*&gameEngine);

				return true;
			}
			if (type == "CHANGEOPACITYOFTHESELIFEBARS") {
				string who = data["who"];
				string opacity = data["opacity"];
				string barback_life = who + "_BARBACKLIFE";
				string barback_mana = who + "_BARBACKMANA";
				string life = who + "_LIFE";
				string mana = who + "_MANA";

				for (auto image : List<string>({ barback_life, barback_mana, life, mana }).internalList) {
					if (graphics.accessImageViaUniqueID(image) == NULL) {
						return false;
					} 
					graphics.accessImageViaUniqueID(image)->opacity = stof(opacity);
				}
				return false;
			}
			if (type == "LOADLIFEBARSHORIZONTAL") {
				string actor = data["actor"];
				string x = data["x"];
				string y = data["y"];
				string baseLayer = data["layer"];
				string scale = data["scale"];
				// do stuff to make it the right amount
				Combat::CombatantInstance* combatant = combat.currentBattle->getThisCombatant(actor);
				int lifeTotal = combatant->c.combatStats["LIFE"];
				int life = combatant->c.combatStats["CURRENTLIFE"];
				int energyTotal = combatant->c.combatStats["ENERGY"];
				int energy = combatant->c.combatStats["CURRENTENERGY"];

				string lifeMessage = "Life: " + to_string(life) + "/" + to_string(lifeTotal);
				string energyMessage = "Energy: " + to_string(energy) + "/" + to_string(energyTotal);

				float lifeProportion = combatant->c.getLifeAsPercentage();
				float manaProportion = combatant->c.getEnergyAsPercentage();

				float manaBarOffset = 2.5;

				Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", to_string(BARBACK_H)),
					pair<string, string>("x", x),
					pair<string, string>("y", y),
					pair<string, string>("anchor", "CENTREXSTRETCH"),
					pair<string, string>("opacity", "1.0"),
					pair<string, string>("layer", to_string(stoi(baseLayer) + 1)),
					pair<string, string>("scale", scale),
					pair<string, string>("uniqueID", "BARBACK_LIFE_H"),
					}))).run(*&gameEngine);
				Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", to_string(LIFEBAR_H)),
					pair<string, string>("x", x),
					pair<string, string>("y", y),
					pair<string, string>("anchor", "CENTREXSTRETCH"),
					pair<string, string>("opacity", "1.0"),
					pair<string, string>("layer", to_string(stoi(baseLayer) + 2)),
					pair<string, string>("scale", scale),
					pair<string, string>("uniqueID", "LIFE_H"),
					pair<string, string>("xStretch", to_string(lifeProportion)),
					}))).run(*&gameEngine);
				Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", to_string(BARBACK_H)),
					pair<string, string>("x", x),
					pair<string, string>("y", to_string(stof(y) + manaBarOffset)),
					pair<string, string>("anchor", "CENTREXSTRETCH"),
					pair<string, string>("opacity", "1.0"),
					pair<string, string>("layer", to_string(stoi(baseLayer) + 1)),
					pair<string, string>("scale", scale),
					pair<string, string>("uniqueID", "BARBACK_MANA_H"),
					}))).run(*&gameEngine);
				Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("sources", to_string(MANABAR_H)),
					pair<string, string>("x", x),
					pair<string, string>("y", to_string(stof(y) + manaBarOffset)),
					pair<string, string>("anchor", "CENTREXSTRETCH"),
					pair<string, string>("opacity", "1.0"),
					pair<string, string>("layer", to_string(stoi(baseLayer) + 2)),
					pair<string, string>("scale", scale),
					pair<string, string>("uniqueID", "MANA_H"),
					pair<string, string>("xStretch", to_string(manaProportion)),
					}))).run(*&gameEngine);
				Event("LifeMessage", "DRAWTEXT", List<pair<string, string>>({
								pair<string, string>("message", lifeMessage),
								pair<string, string>("direct", "1"),
								pair<string, string>("format", "Centaur_12"),
								pair<string, string>("anchorStyle", "CENTRE"),
								pair<string, string>("x", x),
								pair<string, string>("y", y),
								pair<string, string>("w", "50"),
								pair<string, string>("h",  "10"),
								pair<string, string>("colour", "WHITE"),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer",  to_string(stoi(baseLayer) + 2)),
								pair<string, string>("uniqueID", "LIFE_H"),
					})).run(*&gameEngine);
				Event("ManaMessage", "DRAWTEXT", List<pair<string, string>>({
								pair<string, string>("message", energyMessage),
								pair<string, string>("direct", "1"),
								pair<string, string>("format", "Centaur_12"),
								pair<string, string>("anchorStyle", "CENTRE"),
								pair<string, string>("x", x),
								pair<string, string>("y", to_string(stof(y) + manaBarOffset)),
								pair<string, string>("w", "50"),
								pair<string, string>("h",  "10"),
								pair<string, string>("colour", "WHITE"),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer",  to_string(stoi(baseLayer) + 2)),
								pair<string, string>("uniqueID", "MANA_H"),
					})).run(*&gameEngine);
			}
			if (type == "TEARDOWNLIFEBARSHORIZONTAL") {
				for (auto image : List<string>({ "BARBACK_LIFE_H", "LIFE_H", "BARBACK_MANA_H","MANA_H" }).internalList) {
					Event("Remove", "TEARDOWNIMAGE", List<pair<string, string>>({
							pair<string, string>("uniqueID", image),
						})).run(*&gameEngine);
					Event("Remove", "TEARDOWNTEXT", List<pair<string, string>>({
						pair<string, string>("uniqueID", image),
						})).run(*&gameEngine);
				}
				return false;
			}
			if (type == "UPDATECOMBATMESSAGES") {
				int verbosityLevel = stoi(gameEngine.stateFlags["combatVerbosity"]);
				int latestMessageVerbosity = combat.currentBattle->combatMessages.front().first;
				Graphics::Text* theText = graphics.accessTextViaUniqueID("combatText");
				if (latestMessageVerbosity <= verbosityLevel) {
					Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("message","$LATESTCOMBATSTRING$"),
					pair<string, string>("animateExisting","0"),
					pair<string, string>("direct","1"),
					pair<string, string>("format", "GoudyMedieval_20"),
					pair<string, string>("anchorStyle", "TOPLEFT"),
					pair<string, string>("x", "5"),
					pair<string, string>("y", "1"),
					pair<string, string>("w", "99"),
					pair<string, string>("h", "33"),
					pair<string, string>("typewriterSpeed", "0"),
					pair<string, string>("colour", "WHITE"),
					pair<string, string>("shadowColour", "BLACK"),
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["UI"] + 3)),
					pair<string, string>("uniqueID", "combatText"),
					pair<string, string>("animated", "TRUE"),
					pair<string, string>("styles", "TYPEWRITER"),
						}))).run(*&gameEngine);
				}
				if (theText != NULL and theText->fullMessage != combat.currentBattle->combatMessages.front().second) {
					theText->resetMessage(*&graphics, combat.currentBattle->combatMessages.front().second);
					theText->startTypewriter(*&graphics);
				}
				if (latestMessageVerbosity > verbosityLevel) {
					graphics.tearDownSpecifiedText("combatText");
				}
				return false;
			}
			if (type == "HANDLECOMBAT") {
				Event("Text", "UPDATECOMBATMESSAGES", {}).run(*&gameEngine);
				if (combat.currentBattle->combatMessages.size() < 2 and graphics.accessTextViaUniqueID("combatText")->howFarAlong() < 100) {
					CLOCK.startClock("CombatStartWait");
					return false;
				}
				if (!CLOCK.hasEnoughTimePassedDoNotResetClock("CombatStartWait", 1000)) {
					return false;
				}
				string currentActor = combat.currentBattle->currentRound.whoseTurnIsIt();
				string nextActor = combat.currentBattle->currentRound.whoseTurnIsNext();

				bool finishedRound = false;
				bool thereAreEffectsToRun = false;
								
				if (combat.currentBattle->executingSomething) {
					gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "HANDLECOMBATTICK", {}));
					combat.currentBattle->executingSomething = false;
					return false;
				}

				if (combat.currentBattle->playerIsInControl()) {
					finishedRound = Event("HandlePlayerInput", "HANDLECOMBATPLAYERINPUT", {}).run(*&gameEngine);
				}
				
				if (finishedRound) {
					Event("TickUp", "HANDLECOMBATTICK", {}).run(*&gameEngine);
				}
				
				return false;
			}
			if (type == "HANDLECOMBATTICK")  {
				Event("Teardown", "TEARDOWNPLAYERDESCRIPTIONCOMBAT", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("actor", combat.currentBattle->currentRound.whoseTurnIsIt()),
					}))).run(*&gameEngine);
				string combatStatus = combat.currentBattle->tick(*&combat);
				// add something to make battle finish if won or lost
				if (combatStatus == "PLAYERWIN") {
					combat.currentBattle->addCombatMessage(*&combat, "VICTORY", List<pair<string, string>>({
							pair<string, string>("language", gameEngine.language),
						}), 0);
					gameEngine.activeProcedure.eventList.clear();
					gameEngine.activeProcedure.eventList.push_front(Event("EndCombat", "ENDCOMBAT", {}));
					CLOCK.startClock("EndOfCombat");
					Event("Text", "UPDATECOMBATMESSAGES", {}).run(*&gameEngine);
					return false;
				}
				if (combatStatus == "INPROGRESS") {
					combat.currentBattle->announceCombatantTurn(*&combat, gameEngine.language);
					graphics.tearDownSpecifiedText("combatText");
					graphics.tearDownSpecifiedImage("whoseTurn");
					bool isPlayerInControl = combat.currentBattle->playerIsInControl();
					string currentActor = combat.currentBattle->currentRound.whoseTurnIsIt();
					if (isPlayerInControl) {
						string currentActor = combat.currentBattle->currentRound.whoseTurnIsIt();
						string who = combat.currentBattle->getThisCombatant(currentActor)->c.uniqueID;
						// add something here to say "if player is knocked out, skip
						pair<float, float> pos = graphics.accessImageViaUniqueID(currentActor)->positionAsPercentage;
						Event("LoadSkillBar", "LOADSKILLBARHERE", Map<string, string>({
								pair<string, string>("who", who),
								pair<string, string>("scale", "0.66"),
								pair<string, string>("x", "50"),
								pair<string, string>("y", "95.4"),
								pair<string, string>("full", "1"),
								pair<string, string>("combat", "1"),
							})).run(*&gameEngine);
						Event("LoadMenu", "LOADMENU", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("uniqueID", "COMBAT1"),
							}))).run(*&gameEngine);	
						Event("LoadLifeBars", "LOADLIFEBARSHORIZONTAL", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("actor", currentActor),
								pair<string, string>("x", "50"),
								pair<string, string>("y", "87"),
								pair<string, string>("scale", "0.66"),
								pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							}))).run(*&gameEngine);
						float position = pos.second - 30;
						if (who.find("Tianshun Song") != -1) {
							position += 15;
						}
						if (who.find("Angela Fleuret") != -1) {
							position += 10;
						}
						
						Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
							pair<string, string>("sources", imageLookup.getSequenceAsString("WHOSETURN", "FRONT_ACTION")),
							pair<string, string>("x", to_string(pos.first)),
							pair<string, string>("y", to_string(position)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
							pair<string, string>("scale", "1"),
							pair<string, string>("animated", "1"),
							pair<string, string>("animation_speed", "50"),
							pair<string, string>("styles", "LOOP"),
							pair<string, string>("uniqueID", "whoseTurn"),}))).run(*&gameEngine);
						Event("Lifebars", "RELOADALLCOMBATANTIMAGES", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("mode", ""),
							}))).run(*&gameEngine);
					}
					if (currentActor == "WORLD") {
						gameEngine.activeProcedure.eventList.clear();
						gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "HANDLECOMBAT", {}));
						gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "PRINTSKILLSTACKRESULTS", {}));
						gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "EXECUTESKILLINCOMBAT", List<pair<string, string>>({
								pair<string, string>("passive", "1"),
							})));
						gameEngine.activeProcedure.eventList.push_front(Event("Run Skill", "DETERMINESKILLEFFECTSTACK", List<pair<string, string>>({
								pair<string, string>("passive", "1"),
							})));
						combat.currentBattle->executingSomething = true;
						return false;
					}
					if (!isPlayerInControl) {
						Combat::CombatantInstance* actor = combat.currentBattle->getThisCombatant(currentActor);
						if (actor->c.isDead()) {
							for (Combat::EffectObjectInstance* effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(actor->c.uniqueCombatID).internalList) {
								if (effect->e.infinite) { continue; }
								combat.currentBattle->removeAnEffect(actor->c.uniqueCombatID, effect->e.uniqueID);
							}						
							if (actor->wasISummoned()) {
								combat.currentBattle->removeNewCombatantDuringBattle(*&combat, actor);
							}
							return false;
						}
						if (actor == NULL) {
							return false;
						}
						if (actor->c.currentlyCasting and !actor->c.finishedCasting()) {
							CLOCK.startClock("CombatStartWait");
							return false;
						}
						if (actor->c.finishedCasting()) {
							// run the skill they finished casting
							string target = actor->c.currentTarget;
							string buttonName = "STARTUSINGSKILLON_$" + actor->c.currentTarget + "$" + to_string(actor->c.indexOfSkillCurrentlyBeingCast);
							Event("RunButton", "HANDLEBUTTON", List<pair<string, string>>({
								pair<string, string>("uniqueID",buttonName),
								})).run(*&gameEngine);
							return false;
						}
						if (!actor->c.currentlyCasting) {
							string decision = actor->AIMakeDecision(*&combat);
							Event("RunButton", "HANDLEBUTTON", List<pair<string, string>>({
								pair<string, string>("uniqueID",decision),
								})).run(*&gameEngine);
							// make that actor do something
							CLOCK.startClock("CombatStartWait");
							return false;
						}
					}
					return true;
				}
				if (combatStatus == "PLAYERLOSE") {
					combat.currentBattle->addCombatMessage(*&combat, "DEFEAT", List<pair<string, string>>({
							pair<string, string>("language", gameEngine.language),
						}), 0);
					gameEngine.activeProcedure.eventList.clear();
					gameEngine.activeProcedure.eventList.push_front(Event("EndCombat", "ENDCOMBAT", {}));
					CLOCK.startClock("EndOfCombat");
					Event("Text", "UPDATECOMBATMESSAGES", {}).run(*&gameEngine);
					return false;
				}
			}
			if (type == "HANDLECOMBATPLAYERINPUT") {
				if (Args.get("mode") == "DEBUG") {
					//Event("Debug", "DEBUGUSERINPUT", {}).run(*&gameEngine);
					if (controller.hasThisBeenPressed(VK_F1)) {
						// make combat end instantly
					}
					if (controller.hasThisBeenPressed(VK_F2)) {
						// make text disappear and remove film grain
						combat.currentBattle->combatMessages.push_front({ 0, L"" });
						graphics.accessImageViaUniqueID("FilmGrain")->opacity = 0.0;
					}
				}
				Event("HandleSkillBarExplain", "HANDLESKILLEXPLAIN", Map<string, string>({
						pair<string, string>("x","5"),
						pair<string, string>("y","20"),
						pair<string, string>("scale","1.0"),
						pair<string, string>("full","1"),
						pair<string, string>("combat","1"),
					})).run(*&gameEngine);
				Event("HandleMenu", "HANDLEMENU", Map<string, string>({ List<pair<string,string>>({
					pair<string, string>("uniqueID", "COMBAT1"),
				}) })).run(*&gameEngine);

				return false;
			}
			if (type == "DETERMINESKILLEFFECTSTACK") {
				if (data["passive"] == "1") {
					combat.currentBattle->determineCurrentSkillEffectStackPassive(*&combat, gameEngine.language);
				}
				else if (data["active"] == "1") {
					combat.currentBattle->determineCurrentSkillEffectStack(*&combat, gameEngine.language);
				}
				return true;
			}
			if (type == "EXECUTESKILLINCOMBAT") {
				Event("Text", "UPDATECOMBATMESSAGES", {}).run(*&gameEngine);

				if (combat.currentBattle->currentEventStackObject.isAnimationFinished()) {
					if (combat.currentBattle->currentEventStackObject.ongoingReport.empty()) {
						if (data.hasKey("passive")) {
							return true;
						}
					}
					int howFar = graphics.accessTextViaUniqueID("combatText")->howFarAlong();
					if (graphics.accessTextViaUniqueID("combatText")->fullMessage == L"") {
						return true; // tried to use impossible skill such as summoning immediately with no spaces
					}
					if (howFar < 100) {
						CLOCK.startClock("Combat Wait");
						return false;
					}
					for (auto report : combat.currentBattle->currentEventStackObject.ongoingReport.internalList) {
						if (report.sData["success"] != "1") {
							graphics.tearDownSpecifiedText("combatText");
							gameEngine.activeProcedure.eventList.clear();
							gameEngine.activeProcedure.eventList.push_back(Event("Handle Combat", "PRINTSKILLSTACKRESULTS", {}));
							gameEngine.activeProcedure.eventList.push_back(Event("Handle Combat", "HANDLECOMBAT", {}));
							return false;
						}
					}
					if (!combat.currentBattle->currentEventStackObject.toPrint.empty() and combat.currentBattle->currentEventStackObject.counter >= combat.currentBattle->currentEventStackObject.ongoingReport.size()) {
						return true;
					}
					if (CLOCK.hasEnoughTimePassed("Combat Wait", 500)) {
						data["mostRecentMessage"] = WStringToString(graphics.accessTextViaUniqueID("combatText")->fullMessage);
						if (combat.currentBattle->currentEventStackObject.isAnimationFinished()) { return true; }
						return false;
					}
					return false;
				}

				string caster = combat.currentBattle->currentEventStackObject.getCurrentForAnimation().originalUser;
				if (combat.currentBattle->currentEventStackObject.getCurrentForAnimation().combatantsAffected.empty()) {
					combat.currentBattle->currentEventStackObject.animationTick();
					return false;
				}
				string target = combat.currentBattle->currentEventStackObject.getCurrentForAnimation().combatantsAffected.front();
				Map<string, string> extras = {};
				if (target != "WORLD") {
					// move down a bit if an opponent
					if (combat.currentBattle->getThisCombatant(target)->team.find("TEAM2") != -1) {
						extras["yOffset"] = "5";
					}
				}
				if (target == "WORLD") {
					target = "BattleBackground";
				}
				string animationName = combat.currentBattle->currentEventStackObject.getCurrentForAnimation().sourceName;
				Map<string, string> sData = combat.currentBattle->currentEventStackObject.getCurrentForAnimation().sData;

				// Tianshun is small so move Y axis down a bit
				if (caster.find("Tianshun Song") != -1) {
					extras["yOffset"] = "10";
				}
				
				
				if (sData.hasKey("SUMMONTHIS")) {
					extras["SUMMONTHIS"] = sData["SUMMONTHIS"];
					extras["SUMMONTHIS_UNIQUECOMBATID"] = target;
					extras["SUMMONTHIS_DIRECTION"] = sData["SUMMONTHIS_DIRECTION"];
					string teamName = sData["SUMMONTHIS_TEAMNAME"];
					string slot = sData["SUMMONTHIS_SLOT"];
					int layer = imageLookup.layerDefaults["COMBATTEAM1ALLIES"];
					if (extras["SUMMONTHIS_DIRECTION"] == "FRONT") {
						layer = imageLookup.layerDefaults["COMBATTEAM2ALLIES"];
					}
					extras["SUMMONTHIS_LAYER"] = to_string(layer);
					extras["SUMMONSEQUENCE"] = extras["SUMMONTHIS"];
					Graphics::Image * targetImage = graphics.accessImageViaUniqueID(target);
					if (targetImage == NULL) {
						Map<string, Map<string, pair<float, float>>> combatPositions = combat.getCombatantPositionLookup();
						pair<float, float> targetPosition = combatPositions[teamName][slot];
						string scale = combat.layerScaleLookup["TEAM2ALLIES"];
						if (layer == imageLookup.layerDefaults["COMBATTEAM1ALLIES"]) {
							scale = combat.layerScaleLookup["TEAM1ALLIES"];
						}
						extras["SUMMONTHIS_SCALE"] = scale;

						Event("LoadCombatantImageOnly", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString(sData["SUMMONTHIS"], "COMBAT_" + sData["SUMMONTHIS_DIRECTION"])),
						pair<string, string>("x", to_string(targetPosition.first)),
						pair<string, string>("y", to_string(targetPosition.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "0.0"),
						pair<string, string>("layer", to_string(layer)),
						pair<string, string>("scale", scale),
						pair<string, string>("uniqueID", target), }))).run(*&gameEngine);
					}
				}
				if (sData.hasKey("audioSource")) {
					extras["audioSource"] = sData["audioSource"];
				}
				bool result = gameEngine.skillAnimationContainer.handleAnimation(*&gameEngine, graphics.accessImageViaUniqueID(caster), graphics.accessImageViaUniqueID(target), animationName, extras);
				if (!result) {
					return false;
				}
				combat.currentBattle->currentEventStackObject.animationTick();
				for (Combat::CombatantInstance* actor : combat.currentBattle->getAllCombatants().internalList) {
					// if they're dead make them transparent. If they were summoned remove them from combat
					if (actor->c.isDead()) {
						Graphics::Image* image = graphics.accessImageViaUniqueID(actor->c.uniqueCombatID);
						if (!image->animationStyles.contains("FADEOUT") and CLOCK.hasEnoughTimePassed("PlayingTheDyingSound", 100)) {
							GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
									pair<string, string>("audio","6121"),
									pair<string, string>("direct","1"),
									pair<string, string>("delay","0.0"),
								})).run(*&gameEngine);
						}
						graphics.accessImageViaUniqueID(actor->c.uniqueCombatID)->animationStyles.internalList = { "FADEOUT","SINGLE" };
						graphics.accessImageViaUniqueID(actor->c.uniqueCombatID)->animated = true;
						graphics.accessImageViaUniqueID(actor->c.uniqueCombatID)->animationSpeed = 40;
					}
				}
				return false;
			}
			if (type == "PRINTSKILLSTACKRESULTS") {
				bool finished = true;
				for (int x = 0; x < combat.currentBattle->currentEventStackObject.toPrint.size(); x++) {
					Combat::Battle::EventStackObject::Result P = combat.currentBattle->currentEventStackObject.toPrint.at(x);
					int yOffset = 2 * x;
					pair<float, float> thisPosition = P.startingPosition;
					thisPosition.second -= 10;
					string currentUniqueID = P.uniqueID + "_" + to_string(x);
					thisPosition.second += yOffset;
					bool currentFinished = gameEngine.skillAnimationContainer.runDefaultTextAnimation(*&gameEngine, currentUniqueID, P.colour, P.message, thisPosition);
					if (!currentFinished) {
						finished = false;
					}
				}
				if (finished) {
					for (int x = 0; x < combat.currentBattle->currentEventStackObject.toPrint.size(); x++) {
						Combat::Battle::EventStackObject::Result P = combat.currentBattle->currentEventStackObject.toPrint.at(x);
						Event("SetUpPlayerReadoutText", "TEARDOWNTEXT", List<pair<string, string>>({
							pair<string, string>("uniqueID", P.uniqueID + "_" + to_string(x)),
							})).run(*&gameEngine);
					}
					return true;
				}
				return false;
			}
			if (type == "TEARDOWNPLAYERDESCRIPTIONCOMBAT") {
				string actor = data["actor"];
				for (auto image : List<string>({
						"BARBACK_LIFE_H", "LIFE_H", "BARBACK_LIFE_H", "MANA_H",
					}).internalList) {
					Event("Teardown", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("uniqueID", image),
						}
						))).run(*&gameEngine);
				}
			}
			if (type == "RESTORECOMBATANTOPACITY") {
				for (Combat::CombatantInstance* c : combat.currentBattle->getAllCombatants().internalList) {
					if (c->c.isDead()) { continue; }
					graphics.accessImageViaUniqueID(c->c.uniqueCombatID)->opacity = 1.0;
					Event("Lifebars", "CHANGEOPACITYOFTHESELIFEBARS", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("who", c->c.uniqueCombatID),
						pair<string, string>("opacity", "1.0"),
						}))).run(*&gameEngine);
				}
				return false;
			}
			if (type == "LOADCOMBATTARGETBUTTONS") {
				string whichSkillSlot = data["whichSkillSlot"];
				string currentActorName = combat.currentBattle->currentRound.whoseTurnIsIt();
				string who = combat.currentBattle->getThisCombatant(currentActorName)->c.uniqueID;
				Combat::CombatantInstance* currentActor = combat.currentBattle->getCurrentCombatant();
				pair<float, float> userPos = graphics.accessImageViaUniqueID(currentActorName)->positionAsPercentage;
				Combat::Skill skillInstance = currentActor->c.combatSkills[stoi(whichSkillSlot)];

				Event("ResetMenu", "TEARDOWNMENU", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("uniqueID", "COMBAT1"),
					}))).run(*&gameEngine);

				gameEngine.storedMenus["COMBAT1"].buttonReplace(Menu::getDefaultCombatMenuButtons());
				gameEngine.storedMenus["COMBAT1"].buttons["COMBAT1CANCEL"] = Menu::smallerButton("COMBAT1CANCEL", "GUI_COMBAT1CANCEL", {64.5, 82.5});
				gameEngine.storedMenus["COMBAT1"].buttons["COMBAT1EXPLAIN"] = Menu::TextBox("COMBAT1EXPLAIN", "GUI_COMBATEXPLAIN", "TINY", { 38.8, 82.5 });
				strings[gameEngine.language]["GUI"]["COMBATEXPLAIN"] = WSReplace(strings[gameEngine.language]["GUI"]["COMBATEXPLAINBASE"], L"$REPLACE$", strings[gameEngine.language]["Skill Names"][skillInstance.uniqueID]);

				Event("LoadMenu", "LOADMENU", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("uniqueID", "COMBAT1"),
					}))).run(*&gameEngine);

				string targetLogic = skillInstance.targetLogic;
				List<Combat::CombatantInstance*> validTargets = combat.currentBattle->getAllValidTargetsForThisSkill(*&combat, currentActor, skillInstance);
				for (Combat::CombatantInstance* c : validTargets.internalList) {
					Graphics::Image* image = graphics.accessImageViaUniqueID(c->c.uniqueCombatID);
					image->opacity = 1.0;
					pair<float, float> position = image->positionAsPercentage;
					string whereToGetString = "NPCNames_" + c->c.uniqueID;
					pair<float, float> whereToPutButton = position;
					// add things to get right string 
					if (targetLogic == "WORLD") {
						whereToGetString = "NPCNames_WORLD";
					}

					string currentButtonName = "STARTUSINGSKILLON$" + c->c.uniqueCombatID + "$" + whichSkillSlot;
					Menu::Button thisButton = Menu::smallerButton(currentButtonName, whereToGetString, whereToPutButton);
					gameEngine.storedMenus["COMBAT1"].buttons[currentButtonName] = thisButton;
				}
				// make sure dead ppl stay invisible
				for (Combat::CombatantInstance* c : combat.currentBattle->whoIsNotInThisList(validTargets).internalList) {
					if (c->c.isDead()) { continue; }
					graphics.accessImageViaUniqueID(c->c.uniqueCombatID)->opacity = 0.5;
					Event("Lifebars", "CHANGEOPACITYOFTHESELIFEBARS", Map<string, string>(List<pair<string, string>>({
							pair<string, string>("who", c->c.uniqueCombatID),
							pair<string, string>("opacity", "0.5"),
						}))).run(*&gameEngine);
				}

				Event("LoadMenu", "LOADMENU", Map<string, string>(List<pair<string, string>>({
								pair<string, string>("uniqueID", "COMBAT1"),
					}))).run(*&gameEngine);
				return false;
			}
			if (type == "ENDCOMBAT") {
				if (!CLOCK.hasEnoughTimePassedDoNotResetClock("EndOfCombat", 3000)) {
					return false;
				}
				saveContainer.save();
				bool resumeExploringSamePlace = combat.currentBattle->battleStatusCheck() == "PLAYERLOSE" or combat.currentBattle->data["postBattle"] == "RETURNTOEXPLORE";

				if (!CLOCK.hasEnoughTimePassedDoNotResetClock("EndOfCombat",3000)) {
					return false;
				}
				graphics.tearDownSpecifiedText("combatText");
				if (!combat.currentBattle->combatEnding) {
					if (!resumeExploringSamePlace) {
						Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", "4286"),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["LOADINGSCREEN"])),
						pair<string, string>("scale", "2.0"),
						pair<string, string>("uniqueID", "LoadingScreen"), }))).run(*&gameEngine);
					}
					for (Graphics::Image* image : combat.currentBattle->getAllImagesAssociatedWithThisBattle(*&combat).internalList) {
						graphics.bumpLayer(image, 100);
						image->animationStyles = { "SINGLE","FADEOUT" };
						image->animationSpeed = 40;
						image->animated = true;
					}
					combat.currentBattle->combatEnding = true;
					Event("SwapAudio", "STOPALLSONGS", {}).run(*&gameEngine);
					if (combat.currentBattle->data["song"] != "") {
						audio.fadeOutAndStopThis(stoi(combat.currentBattle->data["song"]), 3);
					}
					if (combat.currentBattle->battleStatusCheck() == "PLAYERWIN") {
						Event("LoadNewMap", "LOADMAP", Map<string, string>({
						pair<string, string>("targetMap",explorer.currentMap.name),
						pair<string, string>("putPlayerHere", "1"),
						pair<string, string>("x",to_string(combat.currentBattle->playerPositionBeforeBattle.first)),
						pair<string, string>("y",to_string(combat.currentBattle->playerPositionBeforeBattle.second)),
						pair<string, string>("direction", combat.currentBattle->playerDirectionBeforeBattle),
							})).run(*&gameEngine);
					}
					else {
						Event("LoadNewMap", "LOADMAP", Map<string, string>({
						pair<string, string>("targetMap","Tavern1"),
						pair<string, string>("putPlayerHere", "1"),
						pair<string, string>("x","48"),
						pair<string, string>("y","55"),
						pair<string, string>("direction", "STAND_FRONT"),
							})).run(*&gameEngine);
					}
					
				}
				bool finished = true;

				for (Graphics::Image* image : combat.currentBattle->getAllImagesAssociatedWithThisBattle(*&combat).internalList) {
					if (!image->hasThisFinishedAnimating()) {
						finished = false;
					}
				}
				if (finished) {
					string postCombat = combat.currentBattle->data["postBattle"];
					if (combat.currentBattle->battleStatusCheck() == "PLAYERLOSE") {
						if (combat.currentBattle->data.hasKey("SETFALSEIFLOSE")) {
							saveContainer.current.flags[combat.currentBattle->data["SETFALSEIFLOSE"]] = false;
						}
						if (!saveContainer.current.flags["IntroFinished"]) {
							if (saveContainer.current.flags["FailedTutorial"]) {
								postCombat = "Tutorial";
							}
							else {
								postCombat = "TownCutscene1";
							}
						}
						else {
							postCombat = "TavernTeleport";
						}
					}
					if (combat.currentBattle->battleStatusCheck() == "PLAYERWIN") {
						if (combat.currentBattle->data.hasKey("SETTRUEIFWIN")) {
							saveContainer.current.flags[combat.currentBattle->data["SETTRUEIFWIN"]] = true;
						}
					}
					combat.tearDownBattle();
					gameEngine.activeProcedure.eventList.clear();
					saveContainer.save();
					if (postCombat == "RETURNTOEXPLORE") {
						gameEngine.activeProcedure.eventList.push_front(Event("Explore", "EXPLORE", List<pair<string, string>>({
								pair<string, string>("audio", "1"),
							})));
						Event("", "PLAYALLSONGSFORTHISMAP", {}).run(*&gameEngine);
						return false;
					}
					// add other post combat things that aren't a cutscene
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, postCombat, saveContainer.getCurrentMainCharacter(), "EXPLORE");
					return false;
				}
				return false;
			}
			if (type == "ADDTOCHARACTERS") {
				List<string> parsedData = split(data["WHO"], "$");
				for (auto s : parsedData.internalList) {
					saveContainer.addToCharacterList(s);
				}
				return true;
			}
			if (type == "FORCEIMAGEFRAME") {
				string imageName = "Shadow PLAYER1";
				if (imageName == "Shadow PLAYER1") {
					imageName = saveContainer.getPossibleCutsceneParticipants().front() + "_Shadow";
				}
				graphics.accessImageViaUniqueID(imageName)->forceThisImageToGoToLastFrameAndStayThere();
				return true;
			}
			if (type == "SETFLAGANDSAVE") {
				string whichFlag = data["uniqueID"];
				bool status = data["status"] == "1";
				saveContainer.current.flags[whichFlag] = status;
				saveContainer.save();
				return true;
			}
			if (type == "MANIPOBJECT") {
				string objectName = data["objectName"];
				string action = data["action"];
				string condition1 = data["condition1"];
				string condition2 = data["condition2"];
				if (action == "ENABLEIF") {
					if (saveContainer.current.flags[condition1] == (condition2 == "1")) {
						explorer.getThisMapObject(objectName).canInteract = true;
						if (explorer.getThisMapObject(objectName).data.hasKey("trigger")) {
							explorer.getThisMapObject(objectName).data["trigger"] = "1";
						}
					}
				}
				return true;
			}
			if (type == "CHANGEIMAGELAYER") {
				string imageID = data["uniqueID"];
				int layer = stoi(data["layer"]);
				graphics.moveLayer(graphics.accessImageViaUniqueID(imageID), layer);
				return true;
			}
			if (type == "SETOBSTRUCTION") {
				string objectName = data["uniqueID"];
				bool isObstruction = data["isObstruction"] == "1";
				explorer.getThisMapObject(objectName).obstruction = isObstruction;
				return true;
			}
			return false;
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
	class SkillAnimationContainer {
	public:
		bool handleAnimation(GameEngine & gameEngine, Graphics::Image* caster, Graphics::Image* target, string procedureName, Map<string, string> extras) {
			if (procedureName == "Charge Bolt") {
				if (caster == target) {
					// don't do anything for the enchant self part of the skill
					Combat::CombatantInstance* actualTarget = combat.currentBattle->getThisCombatant(combat.currentBattle->getCurrentCombatant()->c.currentTarget);
					target = graphics.accessImageViaUniqueID(actualTarget->c.uniqueCombatID);
				}
			}
			pair<float, float> targetLocation = target->positionAsPercentage;
			pair<float, float> casterLocation = caster->positionAsPercentage;
			Map<string, bool> finished; // each bubble
			int casterLayer = graphics.whichLayerIsThisImageOn(caster);
			int targetLayer = graphics.whichLayerIsThisImageOn(target);
			int xOffset = 0;
			int yOffset = 0;
			string opacity = "1.0";
			if (extras.hasKey("xOffset")) {
				xOffset = stoi(extras["xOffset"]);
			}
			if (extras.hasKey("yOffset")) {
				yOffset = stoi(extras["yOffset"]);
			}
			bool willResurrect = false; // requires fading a combatant back in
			if (combat.skillDefinitions.hasKey(procedureName) and combat.skillDefinitions[procedureName].purposes.contains("RESURRECT")) {
				willResurrect = true;
			}
			if (extras["audioSource"] == "" and combat.skillDefinitions.hasKey(procedureName)) {
				extras["audioSource"] = to_string(combat.skillDefinitions[procedureName].audioSource);
			}
			if (procedureName == "SELFIMMOLATE") {
				return true;
			}
			if (procedureName == "DEFAULT_DISEASE") {
				procedureName = "DISEASED";
			}

			if (procedureName == "Suicide") {
				return true;
			}
			if (procedureName == "Septicemia") {
				procedureName = "DISEASED";
			}
			if (procedureName == "Blood Boil") {
				procedureName = "BURNING";
			}
			if (procedureName == "Thunderbolt" or procedureName == "Energy Bolt") {
				procedureName = "Charge Bolt";
			}
			if (procedureName == "Delay Blast 2") {
				extras["audioSource"] = "822";
			}
			char ending = procedureName.at(procedureName.size() - 1);
			if (procedureName.at(procedureName.size()-1) == 87) {
				string replacement;
				for (int x = 0; x < procedureName.size()-1; x++) {
					replacement += procedureName.at(x);
				}
				procedureName = replacement;
			}


			List<string> defaultAnimateOnTarget = list<string>({"Revitalise","Strength of Reason", "Laying of Hands", "Heal Wounds", "Serrated Strike", "Brilliant Spark", "Stone Strike", 
				"Stone Curse", "Atrophy", "Blade of Blood", "Vampiric Strike", "Exile", "Brain Drain","Blood Gift","Curse from Beyond the Grave","Viper Eyes", "Hypoxia", "Beggar's Blessing", "Botched Procedure", "Cestodarian Siphon", "Conciliatory Prayer", "Thoughtful Prayer", "Apostle of Patience","Shield of a Goddess", "Ivory Sanctuary", "Papalcy", "Incessant Devotion", "Ambrosia", "Blessed Light","Gift of Knowledge", "Paraclete's Invitation", "Castigate Cruor", "Entomb Spirit","Exalted Smash", "Erase Evil", "Absolution", "Adjudicate", "Stalked by Vengeance", "Rotation Blade", "Trickblade", "Debilitating Smash", "Clobber", "Cleave Armour", "Knee Crack", "Bulldoze","Knight Vision", "On My Target!", "Glass Sword", "Hack", "Bramble Cloak", "Shield of the Messenger", "Smuggler's Gambit", "Magebane Strike","Skewer", "Dragon Smash", "Weaponsmithing", "Winter Blast", "Sanctum Shroud", "Lacrymactory", "Mourning Edge", "Exemplar's Posture", "Bewrayment", "Avenger's Prayer", "Proscribe", "Conversion", "Fading Justice","Suppress","I Shall Take Care of This!","Song of Angels", "Lord's Authority", "Bailiff's Blade", "Fight the Pain!", "Fencer's Flash", "You're Worthless!", "Vapour Blade", "Light from the Other Side", "You're Revolting!", "Night Fracture", "Mug","Charm Collapse","Stalked by Shadows","Psychic Pithing", "Mind Maze", "Blinded Eye", "Black Djinn's Breath", "Wastrel's Comeuppance", "Petrifying Touch", "Rude Awakening", "Time Walk", "Deathdancer's Strike", "Natural Stab", "Platinum Lotus Strike", "Summer Strike", "Ring of Ash", "Shock Value", "Electrocute", "Ball Lightning", "Double-Edged Lightning", "Chain Lightning", "Blinding Flash", "Electric Loop","Mind Fry", "Valkyrie's Aura","Shadow Game", "Twilightning", "Storm Djinn's Grace", "Short Circuit", "Shocking Defeat", "Scowling Rift", "Fire Bolt", "Arcane Furnace", "Hellraiser's Haste", "Glittering Gaze", "Fireball", "Ensorcell", "Phoenix", "Stalked by Flames", "Delay Blast", "Delay Blast 2", "Exalted Explosion", "Starburst", "Odyllic Cleansing", "Brine", "Cryogenic Sleep", "Polar Prison", "Nacreous Aura", "Polar Vortex", "Mirror of Ice", "Earthen Shell", "Crown of Sands", "Crumble", "Crystalline Scythe", "Master of Wards", "Exalted Stab", "Backstab", "Black Mamba Strike", "Stormdragon Strike", "Paralytic Venom", "Blades of Punishment", "Spirit Shanks", "Brutalism", "Royal Slicers", "Parting Stab", "Shattered Moebius", "Magehunter Strike", "Death Chant", "Shroud of Intrigue", "Charge Bolt",
				"BURNING", "BLEEDING", "DISEASED", "POISONED"});
			List<string> defaultAnimateFullScreen = list<string>({"Light of Day", "Wishing Well", "Heatwave", "Pressure Front", "Prophesized Return", "Overrule", "Ice Age", "Global Warming", "Tempest", "Drought", "Rainstorm", "Healing Rain", "Excommunicative Assault", "Godly Repulsion","No One Said You Could Touch!", "Ice Storm", "Chaos Storm", "Underworld Dreams", "Rageflame", "Cataclysm","Thunderstorm", "Stormseeker", "Flame Wave", "Firespitter", "Vault of Destruction", "Tsunami", "Borealis Blast", "Sliprain", "Sandstorm", "Dust Torrent", "Rocky Soil", "Volcano", "Ward Against Magic", "Ward Against Weapons", "Ward Against Catastrophe", "Ward Against Cruelty", "Scatter Strike", });
			List<string> defaultAnimateOnEveryTarget = list<string>({"Order of the Wasp", "Great Gospel", "Remedy Ward", "Angelic Observatory", "Iridescent Breath", "Healing Winds", "Heal Wounds All", "Go On Without Me!", "Time Vortex", "Nacreous Aura 2", "Mass Burial", "Chant of Concentration", "DEFAULT_AFFECTION","Shadow Spike" });

			if (procedureName == "Don't Give Up!") {
				procedureName = "Revitalise";
			}
			if (procedureName == "Cataclysm") {
				opacity = "0.5";
			}
			if (procedureName == "Crazed Chop") {
				procedureName = "Hack";
			}
			if (procedureName == "Wildfire") {
				procedureName = "BURNING";
			}

			if (procedureName == "Contract from Below" or procedureName == "Pact with Darkness") {
				procedureName = "BLEEDING";
			}
			if (procedureName == "BURNING") {
				extras["audioSource"] = "6414";
			}
			if (procedureName == "DISEASED" or procedureName == "DISEASED2") {
				procedureName = "DISEASED";
				extras["audioSource"] = "7807";
			}
			if (procedureName == "BLEEDING") {
				extras["audioSource"] = "6193";
			}
			if (procedureName == "POISONED") {
				extras["audioSource"] = "7859";
			}
			if (procedureName == "Heal Wounds" or procedureName == "Heal Wounds All") {
				// skills that are not called Heal Wounds may kick off a Heal Wounds animation
				extras["audioSource"] = "6120";
			}

			if (willResurrect and !started) {
				CLOCK.startClock("RESURRECTWAIT");
			}
			if (CLOCK.hasEnoughTimePassed("RESURRECTWAIT", 200), willResurrect and started and !target->animationStyles.contains("FADEIN")) {
				target->animationStyles.internalList = {"SINGLE","FADEIN"};
				target->animated = true;
				target->frame = 0;
			}

			if (defaultAnimateOnTarget.contains(procedureName)) {
				if (!started) {
					float xLocation = targetLocation.first + xOffset;
					float yLocation = targetLocation.second + yOffset;

					string scale = "2.0";
					if (procedureName == "Charge Bolt") {
						scale = "6.0";
						yLocation = 50;
					}
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString(procedureName, "ACTION_1")),
						pair<string, string>("x", to_string(xLocation)),
						pair<string, string>("y", to_string(yLocation)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", scale),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "20"),
						pair<string, string>("uniqueID", "Default Skill Animation"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio",extras["audioSource"]),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.0"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				if (started) {
					Graphics::Image* theImage = graphics.accessImageViaUniqueID("Default Skill Animation");
					if (theImage->hasThisFinishedAnimating()) {
						graphics.tearDownSpecifiedImage("Default Skill Animation");
						started = false;
						if (target->animationStyles.contains("FADEIN")) {
							target->animationStyles.internalList = {};
							target->animated = false;
							target->frame = 0;
						}
						return true;
					}
				}
				return false;
			}
			if (defaultAnimateFullScreen.contains(procedureName)) {
				string fullScreenY = "50";
				// some of them look too low down when used by the players against the enemy
				List<string> makeTheseHigher = List<string>({"Excommunicative Assault", "Rotation Blade", "Reckless Swing", "Scatter Strike"});
				if (makeTheseHigher.contains(procedureName)) {
					fullScreenY = "30";
				}
				if (!started) {
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString(procedureName, "ACTION_1")),
						pair<string, string>("x", "50"),
						pair<string, string>("y", fullScreenY),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", opacity),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "4.1"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "30"),
						pair<string, string>("uniqueID", "Skill Animation"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio",to_string(combat.skillDefinitions[procedureName].audioSource)),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				if (started) {
					Graphics::Image* theImage = graphics.accessImageViaUniqueID("Skill Animation");
					if (theImage->hasThisFinishedAnimating()) {
						graphics.tearDownSpecifiedImage("Skill Animation");
						started = false;
						return true;
					}
				}
				return false;
			}
			if (defaultAnimateOnEveryTarget.contains(procedureName)) {
				Combat::CombatEvent example = combat.currentBattle->currentEventStackObject.getCurrentForAnimation();
				// all foes regardless
				if (procedureName == "Time Vortex" or procedureName == "Mass Burial" or procedureName == "Shadow Spike") {
					example.combatantsAffected.clear();
					for (Combat::CombatantInstance* target : combat.currentBattle->getCurrentCombatant()->getMyFoesThatAreAlive(*&combat).internalList) {
						example.combatantsAffected.push_back(target->c.uniqueCombatID);
					}
				} 
				if (procedureName == "Nacreous Aura 2") {
					extras["audioSource"] = "3502";
				}
				// all living allies regardless
				if (procedureName == "Heal Wounds All") {
					example.combatantsAffected.clear();
					for (Combat::CombatantInstance* target : combat.currentBattle->getCurrentCombatant()->getMyAlliesThatAreAlive(*&combat).internalList) {
						example.combatantsAffected.push_back(target->c.uniqueCombatID);
					}
				}
				if (!started) {
					for (auto who : example.combatantsAffected.internalList) {
						pair<float, float> location = graphics.accessImageViaUniqueID(who)->positionAsPercentage;
						Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString(procedureName, "ACTION_1")),
						pair<string, string>("x", to_string(location.first)),
						pair<string, string>("y", to_string(location.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "1.0"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "20"),
						pair<string, string>("uniqueID", who + "_" + procedureName),}))).run(*&gameEngine);
					}
					started = true;
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio",extras["audioSource"]),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.0"),
						})).run(*&gameEngine);
					return false;
				}
				if (started) {
					List<Graphics::Image*> toCheck;
					for (auto who : example.combatantsAffected.internalList) {
						toCheck.push_back(graphics.accessImageViaUniqueID(who + "_" + procedureName));
					}
					bool finished = false;
					for (Graphics::Image* current : toCheck.internalList) {
						if (current->hasThisFinishedAnimating()) { finished = true; }
					}
					if (finished) {
						for (Graphics::Image* current : toCheck.internalList) {
							graphics.tearDownSpecifiedImage(current->unique_ID);
						}
						started = false;
						return true;
					}
					return false;
				}
			}

			if (procedureName == "DEFAULT_WAIT") {
				return true;
			}
			if (procedureName == "DEFAULT_ATTACK") {
				if (!started) {
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("Default Attack", "ACTION_FRONT")),
						pair<string, string>("x", to_string(targetLocation.first)),
						pair<string, string>("y", to_string(targetLocation.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "1.0"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "20"),
						pair<string, string>("uniqueID", "Default Attack Skill Animation"),}))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio","5819"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.1"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				Graphics::Image* theImage = graphics.accessImageViaUniqueID("Default Attack Skill Animation");
				if (theImage != NULL  and theImage->hasThisFinishedAnimating()) {
					graphics.tearDownSpecifiedImage("Default Attack Skill Animation");
					return false;
				}
				if (theImage == NULL) {
					started = false;
					return true;
				}
				return false;
			}
			if (procedureName == "Doublestrike") {
				if (!started) {
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("Doublestrike", "ACTION_1")),
						pair<string, string>("x", to_string(targetLocation.first)),
						pair<string, string>("y", to_string(targetLocation.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "2.0"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "50"),
						pair<string, string>("uniqueID", "Doublestrike Skill Animation"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio","1053"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.1"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				Graphics::Image* theImage = graphics.accessImageViaUniqueID("Doublestrike Skill Animation");
				if (theImage != NULL and theImage->hasThisFinishedAnimating()) {
					graphics.tearDownSpecifiedImage("Doublestrike Skill Animation");
					return false;
				}
				if (theImage == NULL) {
					started = false;
					return true;
				}
				return false;
			}
			if (List<string>({"Life Drain", "DEFAULT_LEECHSKILL", "Aura Drain", "Steal Enchantment"}).contains(procedureName)) {
				int numberOfBubbles = 50;
				string audio = "1060";
				if (procedureName == "Aura Drain") {
					audio = "7669";
				}
				if (procedureName == "Steal Enchantment") {
					audio = "13193";
				}
				
				if (!started) {
					pair<float, float> destination = { casterLocation.first, casterLocation.second - 5 };
					if (extras.hasKey("xOffset")) {
						destination.first += stof(extras["xOffset"]);
					}
					if (extras.hasKey("yOffset")) {
						destination.second += stof(extras["yOffset"]);
					}
					bezierPlots.clear();
					for (int x = 0; x < numberOfBubbles; x++) {
						string imageName = "LIFEBUBBLE" + to_string(x);
						List<string> randomScales = List<string>({"0.1","0.11","0.12","0.13","0.14","0.15","0.16","0.17","0.18","0.19","0.2"});
						List<int> randomTimes = List<int>({50,60,70,80,90,100,110,120,130,});
						Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString(procedureName, "ACTION_1")),
						pair<string, string>("x", to_string(targetLocation.first)),
						pair<string, string>("y", to_string(targetLocation.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", RANDOM.getRandom(randomScales)),
						pair<string, string>("animated", "1"),
						pair<string, string>("animation_speed", "10"),
						pair<string, string>("uniqueID", imageName),}))).run(*&gameEngine);
						List<pair<float, float>> basePoints = List<pair<float, float>>({
							targetLocation, graphics.randomVariation(graphics.getEquidistantPoint(targetLocation, destination),25), destination
							});
						bezierPlots[imageName] = graphics.plot(basePoints, 0.1);
						timers[imageName] = RANDOM.getRandom(randomTimes);
						timers[imageName + "START"] = RANDOM.getRandom(randomTimes) + 50;
						CLOCK.startClock(imageName + "START");
					}
					started = true;
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
							pair<string, string>("audio",audio),
							pair<string, string>("direct","1"),
							pair<string, string>("delay","0.3"),
						})).run(*&gameEngine);
					return false;
				}
				for (int x = 0; x < numberOfBubbles; x++) {
					string imageName = "LIFEBUBBLE" + to_string(x);
					finished[imageName] = bezierPlots[imageName].size() == 0;
					if (!CLOCK.hasEnoughTimePassedDoNotResetClock(imageName + "START", timers[imageName + "START"])) { continue; }
					if (finished[imageName]) {
						graphics.accessImageViaUniqueID(imageName)->animationStyles.push_back("FADEOUT");
					}
					if (!finished[imageName] and CLOCK.hasEnoughTimePassed(imageName, timers[imageName])) {
						graphics.accessImageViaUniqueID(imageName)->positionAsPercentage = bezierPlots[imageName].pop_front();
					}
				}
				if (!finished.getValues().contains(false)) {
					started = false;
					for (int x = 0; x < numberOfBubbles; x++) {
						string imageName = "LIFEBUBBLE" + to_string(x);
						graphics.tearDownSpecifiedImage(imageName);
					}
					return true;
				}
				return false;
			}
			if (List<string>({ "Snowblind", "Ice Beam", "Hail", "Jerod's Runestone"}).contains(procedureName)) {
				int numberOfBubbles = 33;
				int varSize = 33;
				float speedAsPercentage = 0.03;
				int audio = 3275;

				pair<float, float> source = caster->positionAsPercentage;
				pair<float, float> destination = target->positionAsPercentage;
				if (source.first < destination.first) {destination.first += 7;}
				if (source.first > destination.first) {destination.first -= 7;}
				if (source.second < destination.second) {destination.second += 5;}
				if (source.second > destination.second) {destination.second -= 5;}
				source.second -= 15;

				if (procedureName == "Ice Beam") {
					audio = ICEBEAM_WAV;
					varSize = 2;
				}

				if (procedureName == "Hail") {
					audio = ICEBEAM_WAV;
					varSize = 10;
				}

				if (procedureName == "Jerod's Runestone") {
					varSize = 0;
					numberOfBubbles = 1;
					audio = JEROD_WAV;
				}

				if (!started) {
					straightPlots.clear();
					for (int x = 0; x < numberOfBubbles; x++) {
						pair<float, float> currentDestination = destination;
						currentDestination.second += RANDOM.getRandom(varSize * -1, varSize);
						string imageName = "LIFEBUBBLE" + to_string(x);
						List<string> randomScales = List<string>({ "2.5","2.6","2.4","2.3","2.2","2.1","2.16","2.7","2.8","2.9","3" });
						List<int> randomTimes = List<int>({ 50,100,200,300,400,500,600,800,700, });
						Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
							pair<string, string>("sources", imageLookup.getSequenceAsString(procedureName, "ACTION_1")),
							pair<string, string>("x", to_string(source.first)),
							pair<string, string>("y", to_string(source.second)),
							pair<string, string>("anchor", "CENTRE"),
							pair<string, string>("opacity", "0"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
							pair<string, string>("scale", RANDOM.getRandom(randomScales)),
							pair<string, string>("animated", "1"),
							pair<string, string>("styles", "LOOP"),
							pair<string, string>("animation_speed", "10"),
							pair<string, string>("uniqueID", imageName), }))).run(*&gameEngine);
						straightPlots[imageName] = currentDestination;
						timers[imageName] = RANDOM.getRandom(randomTimes);
						timers[imageName + "START"] = RANDOM.getRandom(randomTimes) + 50;
						CLOCK.startClock(imageName + "START");
					}
					started = true;
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
							pair<string, string>("audio",to_string(audio)),
							pair<string, string>("direct","1"),
							pair<string, string>("delay","0.3"),
						})).run(*&gameEngine);
					return false;
				}
				bool allFinished = true;
				for (int x = 0; x < numberOfBubbles; x++) {
					string imageName = "LIFEBUBBLE" + to_string(x);
					if (!CLOCK.hasEnoughTimePassedDoNotResetClock(imageName + "START", timers[imageName])) {
						allFinished = false;
						continue;
					}
					Graphics::Image * current = graphics.accessImageViaUniqueID(imageName);
					if (explorer.areThesePointsThisClose(current->positionAsPercentage, straightPlots[imageName], false, false, 10)) {
						current->animationStyles.addToBackIfNotAlreadyInList("FADEOUT");
					}
					else {
						current->opacity = 1.0;
						current->animationStyles = List<string>({"LOOP"});
					}
					if (explorer.areThesePointsThisClose(current->positionAsPercentage, straightPlots[imageName], false, false, 5)) {
						current->opacity = 0;
					}
					else {
						current->positionAsPercentage = explorer.moveLHSCloserToRHSAsPercentage(source, current->positionAsPercentage, straightPlots[imageName], false, false, speedAsPercentage);
						allFinished = false;
					}

				}
				if (!allFinished) { return false; }
				for (int x = 0; x < numberOfBubbles; x++) {
					string imageName = "LIFEBUBBLE" + to_string(x);
					graphics.tearDownSpecifiedImage(imageName);
				}
				started = false;
				return true;
			}
			if (procedureName == "Fine Strike") {
				if (!started) {
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("FineStrike1", "ACTION_1")),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "2.1"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "20"),
						pair<string, string>("uniqueID", "Fine Strike Skill Animation"), }))).run(*&gameEngine);
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("FineStrike2", "ACTION_1")),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "2.1"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "20"),
						pair<string, string>("uniqueID", "Fine Strike Skill Animation 2"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio","1062"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.5"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				Graphics::Image* theImage = graphics.accessImageViaUniqueID("Fine Strike Skill Animation");
				Graphics::Image* theImage2 = graphics.accessImageViaUniqueID("Fine Strike Skill Animation 2");

				if (theImage->animationStyles.contains("FADEOUT") and theImage->hasThisFinishedAnimating()) {
					graphics.tearDownSpecifiedImage("Fine Strike Skill Animation");
					graphics.tearDownSpecifiedImage("Fine Strike Skill Animation 2");
					started = false;
					return true;
				}

				for (auto image : { theImage, theImage2 }) {
					if (image->hasThisFinishedAnimating() and !image->animationStyles.contains("FADEOUT")) {
						image->animationStyles.push_back("FADEOUT");
					}
					return false;
				}
			}
			if (procedureName == "Gentleman's Riposte") {
				if (!started) {
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("Gentleman's Riposte", "ACTION_1")),
						pair<string, string>("x", to_string(casterLocation.first)),
						pair<string, string>("y", to_string(casterLocation.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "1.0"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "20"),
						pair<string, string>("uniqueID", "Riposte Skill Animation"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio","5868"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.5"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				if (started) {
					Graphics::Image* theImage = graphics.accessImageViaUniqueID("Riposte Skill Animation");
					if (theImage->hasThisFinishedAnimating()) { 
						graphics.tearDownSpecifiedImage("Riposte Skill Animation");
						started = false;
						return true; }
				}
				return false;
			}
			if (extras.hasKey("SUMMONSEQUENCE")) {
				if (!started) {
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString(extras["SUMMONSEQUENCE"], "ACTION_" + extras["SUMMONTHIS_DIRECTION"])),
						pair<string, string>("x", to_string(targetLocation.first)),
						pair<string, string>("y", to_string(targetLocation.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", extras["SUMMONTHIS_LAYER"]),
						pair<string, string>("scale",  extras["SUMMONTHIS_SCALE"]),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "40"),
						pair<string, string>("uniqueID", "Animate Skill Animation"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio", to_string(combat.skillDefinitions[procedureName].audioSource)),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.2"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				if (started) {
					Graphics::Image* theImage = graphics.accessImageViaUniqueID("Animate Skill Animation");
					if (theImage->hasThisFinishedAnimating()) {
						graphics.tearDownSpecifiedImage("Animate Skill Animation");
						started = false;
						Graphics::Image* theImage2 = graphics.accessImageViaUniqueID(extras["SUMMONTHIS_UNIQUECOMBATID"]);
						theImage2->opacity = 1.0;
						return true;
					}
				}
				return false;
			}
			if (procedureName == "FAILEDSKILL") {
				if (!started) {
					CLOCK.startClock("FailedSkillAnimation");
					started = true;
				}

				if (started and CLOCK.hasEnoughTimePassed("FailedSkillAnimation", 1500)) {
					started = false;
					return true;
				}
				return false;
			}
			if (procedureName == "Heavenstrike") {
				if (!started) {
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("Heavenstrike1", "ACTION_1")),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(targetLayer-1)),
						pair<string, string>("scale", "2.1"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "40"),
						pair<string, string>("uniqueID", "Heavenstrike1"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio","1056"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.0"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				Graphics::Image* theImage = graphics.accessImageViaUniqueID("Heavenstrike1");
				Graphics::Image* theImage2 = graphics.accessImageViaUniqueID("Heavenstrike2");
				if (theImage->hasThisFinishedAnimating() and theImage2 == NULL) {
					theImage->resetSources(*&graphics, imageLookup.getSequence("Heavenstrike1", "ACTION_2"));
					theImage->frame = 0;
					theImage->animationStyles.internalList = {"SINGLE"};
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("Heavenstrike2", "ACTION_1")),
						pair<string, string>("x", to_string(targetLocation.first)),
						pair<string, string>("y", to_string(targetLocation.second - 10)),
						pair<string, string>("anchor", "BOTTOMMIDDLE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "1.6"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "120"),
						pair<string, string>("uniqueID", "Heavenstrike2"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio","1057"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.0"),
						})).run(*&gameEngine);
				}
				if (theImage2 == NULL) {
					return false;
				}
				if (theImage2->hasThisFinishedAnimating()) {
					theImage2->opacity = 0.0;
				}
				
				if (theImage2->hasThisFinishedAnimating() and theImage->hasThisFinishedAnimating()) {
					graphics.tearDownSpecifiedImage("Heavenstrike1");
					graphics.tearDownSpecifiedImage("Heavenstrike2");
					started = false;
					return true;
				}
				return false;
			}
			if (procedureName == "Plasma Pulse") {
				if (!started) {
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("PlasmaPulse1", "ACTION_1")),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "2.1"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "20"),
						pair<string, string>("uniqueID", "PlasmaPulse1"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio","1058"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.0"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				Graphics::Image* theImage = graphics.accessImageViaUniqueID("PlasmaPulse1");
				Graphics::Image* theImage2 = graphics.accessImageViaUniqueID("PlasmaPulse2");
				Graphics::Image* theImage3 = graphics.accessImageViaUniqueID("PlasmaPulse3");
				Graphics::Image* theImage4 = graphics.accessImageViaUniqueID("PlasmaPulse4");
				int layer = graphics.whichLayerIsThisImageOn(caster);
				if (theImage != NULL and theImage->hasThisFinishedAnimating()) {
					graphics.tearDownSpecifiedImage("PlasmaPulse1");
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("PlasmaPulse1", "ACTION_2")),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "2.1"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "20"),
						pair<string, string>("uniqueID", "PlasmaPulse2"), }))).run(*&gameEngine);
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("PlasmaPulse4", "ACTION_1")),
						pair<string, string>("x", to_string(casterLocation.first)),
						pair<string, string>("y", to_string(casterLocation.second - 15)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(layer-1)),
						pair<string, string>("scale", "1.0"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "LOOP"),
						pair<string, string>("animation_speed", "40"),
						pair<string, string>("uniqueID", "PlasmaPulse4"), }))).run(*&gameEngine);
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("PlasmaPulse5", "ACTION_1")),
						pair<string, string>("x", to_string(targetLocation.first)),
						pair<string, string>("y", to_string(targetLocation.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "2.1"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "LOOP"),
						pair<string, string>("animation_speed", "40"),
						pair<string, string>("uniqueID", "PlasmaPulse5"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio","1059"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0.0"),
						})).run(*&gameEngine);
				}
				if (theImage2 != NULL and theImage2->hasThisFinishedAnimating()) {
					started = false;
					for (auto image : { "PlasmaPulse2", "PlasmaPulse3", "PlasmaPulse4", "PlasmaPulse5" }) {
						graphics.tearDownSpecifiedImage(image);
					}
					return true;
				}
				if (theImage3 != NULL and theImage3->hasThisFinishedAnimating()) {
					theImage3->opacity = 0.0;
				}


				
			}
			if (procedureName == "Basalt Bastion") {
				if (!started) {
					pair<float, float> sizeOfTarget = target->getSizeAsPercentage(*&graphics);
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("Basalt Bastion", "ACTION_1")),
						pair<string, string>("x", to_string(targetLocation.first)),
						pair<string, string>("y", to_string(targetLocation.second + (sizeOfTarget.second/4))),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "1.0"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "55"),
						pair<string, string>("uniqueID", "Default Skill Animation"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio","7540"),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				Graphics::Image* theImage = graphics.accessImageViaUniqueID("Default Skill Animation");
				if (theImage != NULL and theImage->hasThisFinishedAnimating()) {
					graphics.tearDownSpecifiedImage("Default Skill Animation");
					return false;
				}
				if (theImage == NULL) {
					started = false;
					return true;
				}
				return false;
			}
			if (procedureName == "Reckless Swing") {
				if (!started) {
					Event("LoadSkillAnimation", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString(procedureName, "ACTION_1")),
						pair<string, string>("x", "10"),
						pair<string, string>("y", to_string(targetLocation.second)),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["SKILLS"])),
						pair<string, string>("scale", "2.0"),
						pair<string, string>("animated", "1"),
						pair<string, string>("styles", "SINGLE"),
						pair<string, string>("animation_speed", "30"),
						pair<string, string>("uniqueID", "Skill Animation"), }))).run(*&gameEngine);
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio",to_string(combat.skillDefinitions[procedureName].audioSource)),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0"),
						})).run(*&gameEngine);
					started = true;
					return false;
				}
				if (started) {
					Graphics::Image* theImage = graphics.accessImageViaUniqueID("Skill Animation");
					if (CLOCK.hasEnoughTimePassed("Reckless Swing", 5)) {
						theImage->positionAsPercentage.first += 1;
					}
					if (theImage->hasThisFinishedAnimating()) {
						graphics.tearDownSpecifiedImage("Skill Animation");
						started = false;
						return true;
					}
				}
				return false;
			}
			if (List<string>({ "Earthquake", "Earthquake", "Uneasy Earth"}).contains(procedureName)) {
				if (!started) {
					Map<string, pair<float, float>> positions;
					for (Combat::CombatantInstance* actor : combat.currentBattle->getAllCombatants().internalList) {
						positions[actor->c.uniqueCombatID] = graphics.accessImageViaUniqueID(actor->c.uniqueCombatID)->positionAsPercentage;
						quakeCoordinates[actor->c.uniqueCombatID].push_back(positions[actor->c.uniqueCombatID]);
					}
					int quakes = 150;
					for (int x = 0; x < quakes; x++) {
						int currentShake = RANDOM.getRandom(-10, 10);
						for (string actor : positions.getKeys().internalList) {
							quakeCoordinates[actor].push_front({ positions[actor].first + currentShake, positions[actor].second });
						}
					}
					GameEngine::Event("PlayAudio", "PLAYSFX", Map<string, string>({
								pair<string, string>("audio",to_string(combat.skillDefinitions[procedureName].audioSource)),
								pair<string, string>("direct","1"),
								pair<string, string>("delay","0"),
						})).run(*&gameEngine);
					started = true;
				} 
				bool finished = true;
				if (CLOCK.hasEnoughTimePassed("earthquakeTimer", 1)) {
					for (string actor : quakeCoordinates.getKeys().internalList) {
						if (!quakeCoordinates[actor].empty()) {
							finished = false;
							graphics.accessImageViaUniqueID(actor)->positionAsPercentage = quakeCoordinates[actor].front();
							quakeCoordinates[actor].pop_front();
						}
					}
				}
				if (finished) {
					started = false;
					return true;
				}
				return false;
			}

			return false;
		}
		bool runDefaultTextAnimation(GameEngine & gameEngine, string textName, string colour, string message, pair<float, float> start) {
			Graphics::Text* textPopUp = graphics.accessTextViaUniqueID(textName);
			pair<float, float> destination = { start.first, start.second - 10 };
			if (textPopUp == NULL) {
				CLOCK.startClock(textName);
				CLOCK.startClock(textName + "MOVE");
				Event("SetUpPlayerReadoutText", "DRAWTEXT", List<pair<string, string>>({
								pair<string, string>("message", message),
								pair<string, string>("direct", "1"),
								pair<string, string>("format", "Centaur_20"),
								pair<string, string>("anchorStyle", "CENTRE"),
								pair<string, string>("x", to_string(start.first)),
								pair<string, string>("y", to_string(start.second)),
								pair<string, string>("w", "50"),
								pair<string, string>("h",  "50"),
								pair<string, string>("colour", colour),
								pair<string, string>("shadowColour", "DARKBROWN"),
								pair<string, string>("layer",  to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("uniqueID", textName),
					})).run(*&gameEngine);
				return false;
			}
			if (CLOCK.hasEnoughTimePassed(textName + "MOVE", 80)) {
				graphics.accessTextViaUniqueID(textName)->positionAsPercentage.second -= 1;
			}
			if (textPopUp->positionAsPercentage == destination) {
				return true;
			}
			
			return false;
		}

		bool started = false;
		Map<string, List<pair<float, float>>> bezierPlots;
		Map<string, pair<float, float>> straightPlots;
		Map<string, int> timers;
		Map <string, List<pair<float, float>>> quakeCoordinates;
	};
	Map<string, Procedure> storedProcedures = List<pair<string, Procedure>>({
		pair<string, Procedure>({"BOOTMENU", 
			Procedure("Boot Menu", List<Event>({
			Event("Play This Song", "PLAYTHISSONG", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("uniqueID", "8354"),
				}))),
			Event("Load Background", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", imageLookup.getSequenceAsString("BOOTMENU2","ACTION_1")),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "50"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("scale", "2.0"),
				pair<string, string>("opacity", "1.0"),
				pair<string, string>("layer", "1"),
				pair<string, string>("styles", "LOOP"),
				pair<string, string>("uniqueID", "bootMenu2"),
				pair<string, string>("animated", "1"),
				pair<string, string>("animation_speed", "100"),
			}))),
			Event("Wait", "WAIT", Map<string, string>({
				pair<string, string>("clockID", "BOOTMENU"),
				pair<string, string>("waitDuration", "3000"),
			})),
			Event("Load Background", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", imageLookup.getSequenceAsString("BOOTMENU1","ACTION_1")),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "50"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "0.0"),
				pair<string, string>("layer", "0"),
				pair<string, string>("styles", "FADEIN$LOOP"),
				pair<string, string>("uniqueID", "bootMenu1"),
				pair<string, string>("animated", "1"),
				pair<string, string>("animation_speed", "120"),
			}))),
			Event("Wait", "WAIT", Map<string, string>({
				pair<string, string>("clockID", "BOOTMENU"),
				pair<string, string>("waitDuration", "500"),
			})),
			Event("Load Image", "LOADIMAGE", Map<string, string>(List<pair<string,string>>({
				pair<string, string>("sources", imageLookup.getSequenceAsString("LOGO","ACTION_1")),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "30"),
				pair<string, string>("anchor", "CENTRE"),
				pair<string, string>("opacity", "0.0"),
				pair<string, string>("layer", "2"),
				pair<string, string>("styles", "FADEIN$LOOP"),
				pair<string, string>("uniqueID", "Logo"),
				pair<string, string>("animated", "1"),
				pair<string, string>("animation_speed", "120"),
			}))),
			Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("message","$LANGUAGE$_GUI_VERSION"),
				pair<string, string>("animateExisting","0"),
				pair<string, string>("format", "Centaur_25"),
				pair<string, string>("anchorStyle", "TOPLEFT"),
				pair<string, string>("x", "88"),
				pair<string, string>("y", "95"),
				pair<string, string>("w", "100"),
				pair<string, string>("h", "10"),
				pair<string, string>("colour", "ELITESKILLYELLOW"),
				pair<string, string>("shadowColour", "DARKBROWN"),
				pair<string, string>("layer", "4"),
				pair<string, string>("uniqueID", "version"),
				pair<string, string>("animated", "TRUE"),
				pair<string, string>("styles", "TYPEWRITER"),
				pair<string, string>("typewriterSpeed", "25"),
				pair<string, string>("nowait", "1"),
					}))),
			Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("message","$LANGUAGE$_GUI_CREDIT"),
				pair<string, string>("animateExisting","0"),
				pair<string, string>("format", "Centaur_25"),
				pair<string, string>("anchorStyle", "TOPLEFT"),
				pair<string, string>("x", "2"),
				pair<string, string>("y", "95"),
				pair<string, string>("w", "100"),
				pair<string, string>("h", "10"),
				pair<string, string>("colour", "ELITESKILLYELLOW"),
				pair<string, string>("shadowColour", "DARKBROWN"),
				pair<string, string>("layer", "4"),
				pair<string, string>("uniqueID", "credits"),
				pair<string, string>("animated", "TRUE"),
				pair<string, string>("styles", "TYPEWRITER"),
				pair<string, string>("typewriterSpeed", "25"),
				pair<string, string>("nowait", "1"),
					}))),
			Event("LoadAMenu", "LOADMENU", Map<string, string>({
				pair<string, string>("uniqueID", "MAINMENU"),
			})),
			Event("HandleMenu", "HANDLEMENU", Map<string, string>({
				pair<string, string>("uniqueID", "MAINMENU"),
			})),
				}))}),
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
				pair<string, string>("message","ENG_NAMES_Angela"),
				pair<string, string>("format", "HighTowerText_40"),
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
				pair<string, string>("message","ENG_DEBUG_TestString"),
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
			Event("Debug Loading", "DEBUGLOAD", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("uniqueID", "Olyver Sumner")
				}))),
			Event("Load Map", "MANAGEAUDIOSWAP", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("targetMap", "RoadToBénouville"),
			}))),
			Event("Load Map", "LOADMAP", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("targetMap", "RoadToBénouville"),
			}))),
			Event("Debug Exploring", "EXPLORE", Map<string,string>(List<pair<string,string>>({}))),
			})) }),
		pair<string, Procedure>({ "Test", Procedure("Cutscene", List<Event>({
			Event("1", "DIALOGUE", {Map<string, string>(List<pair<string,string>>({
				pair<string, string>({"cutscene", "DeadHorse"}),
				pair<string, string>({"speaker", "PLAYER"}),
				}))}),
			Event("2", "DIALOGUE", {Map<string, string>(List<pair<string,string>>({
				pair<string, string>({"cutscene", "DeadHorse"}),
				pair<string, string>({"speaker", "PLAYER"}),
				}))}),
				Event("Line2", "TEARDOWNDIALOGUE",{}),
				Event("Explore", "EXPLORE",{}),
}))}),
		});
	Map<string, Menu> storedMenus = List<pair<string, Menu>>({
		pair<string, Menu>(
			"MAINMENU", Menu("MAINMENU", List<Menu::Button>({
				Menu::standardButton("NewGame", "GUI_NEWGAME", {30,75}),
				Menu::smallButton("Credits", "GUI_CREDITS", {50,97}),
				Menu::standardButton("LoadGame", "GUI_LOADGAME", {70,75}),
				}), {})),
		pair<string, Menu>(
			"NEWGAME", Menu("NEWGAME", List<Menu::Button>({
				Menu::standardButton("NewGameToMain", "GUI_CANCEL", {20,93}),
				Menu::standardButton("StartGame", "GUI_STARTGAME", {80,93}),
				}), Map<string, string>({
						pair<string, string>("SELECTCHARACTERTOEDIT", "1"),
						pair<string, string>("CHARACTERTOEDITOFFSETX", "18"),
						pair<string, string>("CHARACTERTOEDITOFFSETY", "35"),
						pair<string, string>("CHARACTERTOEDITSCALE", "0.8"),
						pair<string, string>("TYPEWRITERTEXT_1", "startMenu1"),
						pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " NewGameToMain"),
					}))),
		pair<string, Menu>(
			"CREDITS", Menu("CREDITS", List<Menu::Button>({
				Menu::standardButton("CreditsToMain", "GUI_RETURN", {80,93}),
				}), Map<string, string>({
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " CreditsToMain"),
					}))),
		pair<string, Menu>(
			"LOADGAME", Menu("LOADGAME", List<Menu::Button>({
				}), Map<string, string>({
					}))),
		pair<string, Menu>(
			"DEBUGMENU", Menu("DEBUGMENU", List<Menu::Button>({
				Menu::smallButton("DebugButton_StartAFight", "GUI_STARTAFIGHT", {10,5}),
				Menu::smallButton("DebugButton_BackToExplore", "GUI_BACKTOEXPLORE", {10,10}),
				Menu::smallButton("DebugButton_ShowFPS", "GUI_SHOWFPS", {10,15}),
				Menu::smallButton("DebugButton_FastMove", "GUI_FASTMOVE", {10,20}),
				Menu::smallButton("DebugButton_Amnesia", "GUI_AMNESIA", {10,25}),
				Menu::smallButton("DebugButton_UserInput", "GUI_DEBUGUSERINPUT", {10,30}),

				Menu::smallButton("DebugButton_RunCutscene", "GUI_STARTCUTSCENE", {25,5}),

				Menu::smallButton("DebugButton_TeleportToChapel", "GUI_TELEPORTTOCHAPEL", {40,5}),
				Menu::smallButton("DebugButton_TeleportToChapelR1", "GUI_TELEPORTTOCHAPELR1", {40,10}),
				Menu::smallButton("DebugButton_TeleportToChapelR2", "GUI_TELEPORTTOCHAPELR2", {40,15}),
				Menu::smallButton("DebugButton_TeleportToChapelR3", "GUI_TELEPORTTOCHAPELR3", {40,20}),
				Menu::smallButton("DebugButton_TeleportToTown", "GUI_TELEPORTTOTOWN", {40,25}),
				Menu::smallButton("DebugButton_TeleportToEstateSecret", "GUI_TELEPORTTOESTATESECRET", {40,30}),
				Menu::smallButton("DebugButton_TeleportToChapelCrackInWall", "GUI_TELEPORTTOESTATECRACKINWALL", {40,35}),
				Menu::smallButton("DebugButton_TeleportToChapelLeft1", "GUI_TELEPORTTOCHAPELLEFT1", {40,40}),
				Menu::smallButton("DebugButton_TeleportToChapelR2ByStairs", "GUI_TELEPORTTOCHAPELR2BYSTAIRS", {40,45}),
				Menu::smallButton("DebugButton_TeleportToMerchant", "GUI_TELEPORTTOMERCHANT", {40,50}),
				Menu::smallButton("DebugButton_Player2Cutscene", "GUI_DEBUGPLAYER2", {40,55}),
				Menu::smallButton("DebugButton_TeleportToSpigotPuzzle", "GUI_TELEPORTTOSPIGOTPUZZLE", {40,60}),
				Menu::smallButton("DebugButton_BloodWallDefeated", "GUI_BLOODWALLDEFEATED", {40,65}),
				Menu::smallButton("DebugButton_OudinDefeated", "GUI_OUDINDEFEATED", {40,70}),
				Menu::smallButton("DebugButton_DreamSequence1", "GUI_DREAM1", {40,75}),
				Menu::smallButton("DebugButton_TeleportToSpain", "GUI_TELEPORTTOSPAIN", {40,80}),
				Menu::smallButton("DebugButton_RiderDefeatedCutscene", "GUI_RIDERDEFEATED", {40,85}),
				Menu::smallButton("DebugButton_BloodWallIntro", "GUI_BLOODWALLINTRO", {40,90}),

				Menu::smallButton("DebugButton_AddAllPlayable", "GUI_ADDALLPLAYABLE", {55,5}),
				Menu::smallButton("DebugButton_ToggleFlag_IntroFinished", "GUI_IntroFinishedFlag", {70,5}),
				Menu::smallButton("DebugButton_ToggleFlag_OudinDefeated", "GUI_OudinDefeatedFlag", {70,10}),
				Menu::smallButton("DebugButton_VariousChapelR2Flags", "GUI_ChapelRightWing2Cutscenes", {70,15}),
				Menu::smallButton("DebugButton_VariousChapelL1Flags", "GUI_ChapelLeftSpigotsDone", {70,20}),
				Menu::smallButton("DebugButton_ChapelFinished", "GUI_ChapelFinished", {70,25}),
				Menu::smallButton("DebugButton_WaterRoomDebug", "GUI_WaterRoomDebug", {70,30}),
				Menu::smallButton("DebugButton_LookingForMichelet", "GUI_LookingForMichelet", {70,35}),
				Menu::smallButton("DebugButton_FoundMichelet", "GUI_FoundMichelet", {70,40}),

				}), Map<string, string>({
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " DebugButton_BackToExplore"),
				}))),
		pair<string, Menu>(
			"HELPMENU", Menu("HELPMENU", List<Menu::Button>({
				Menu::ClickableTextBox("HELPBUTTONCOMBATBASICS", "GUI_HELPBUTTONCOMBATBASICS", "VERY_SMALL", {20,20}),
				Menu::ClickableTextBox("HELPBUTTONPARTYMANAGEMENTBASICS", "GUI_HELPBUTTONPARTYMANAGEMENTBASICS", "VERY_SMALL", {20,30}),
				Menu::ClickableTextBox("HELPBUTTONATTRIBUTEBASICS2", "GUI_HELPBUTTONATTRIBUTEBASICS2", "VERY_SMALL", {20,40}),
				Menu::ClickableTextBox("HELPBUTTONEXPLOREBASICS", "GUI_HELPBUTTONEXPLOREBASICS", "VERY_SMALL", {20,50}),
				Menu::ClickableTextBox("HELPBUTTONEXPLOREMOREBASICS", "GUI_HELPBUTTONEXPLOREMOREBASICS", "VERY_SMALL", {20,60}),
				Menu::ClickableTextBox("HELPBUTTONBUILDBASICS", "GUI_HELPBUTTONBUILDBASICS", "VERY_SMALL", {20,70}),
				Menu::smallButton("HelpMenu_BackToPartyManage", "GUI_RETURN", {10,90}),
			}),Map<string, string>({
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " HelpMenu_BackToPartyManage"),
				}))),
		pair<string, Menu>(
			"EXPLOREPAUSE", Menu("EXPLOREPAUSE", List<Menu::Button>({
				Menu::standardButton("RETURNTOEXPLORE", "GUI_RESUMEEXPLOREBUTTON", {35, 14}),
				Menu::standardButton("PARTYMANAGEMENT", "GUI_MANAGEPARTYBUTTON", {35, 25}),
				Menu::standardButton("PARTYMANAGEHELP", "GUI_PARTYMANAGEHELP", {35, 36}),
				Menu::standardButton("GOTOCODEXBUTTON", "GUI_GOTOCODEXBUTTON", {35, 47}),
				Menu::standardButton("AUDIOSETTINGS", "GUI_AUDIOOPTIONS", {65, 14}),
				Menu::standardButton("QUITTOMAIN", "GUI_QUITTOMAINBUTTON", {65, 25}),
				Menu::standardButton("QUITTODESKTOP", "GUI_QUITBUTTON", {65, 36}),
				}),Map<string, string>({
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " RETURNTOEXPLORE")}))),
		pair<string, Menu>(
			"AUDIOSETTINGS", Menu("AUDIOSETTINGS", List<Menu::Button>({
				Menu::standardButton("FROMAUDIOTOPAUSE", "GUI_FROMAUDIOTOPAUSEBUTTON", {50, 75}),
				}),Map<string, string>({
					pair<string, string>("AUDIOMANAGEMENT", "1"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMAUDIOTOPAUSE")}))),
		pair<string, Menu>(
			"QUITCONFIRM", Menu("QUITCONFIRM", List<Menu::Button>({
				Menu::TextBox("TEXTBOX1", "GUI_QUITTEXT", "SMALL", {50, 20}),
				Menu::standardButton("QUITNO", "GUI_CANCELBUTTON", {50, 45}),
				Menu::standardButton("QUITYES", "GUI_QUITBUTTON", {50, 60}),
				}),Map<string, string>({
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " QUITNO")}))),
		pair<string, Menu>(
			"QUITTOMAINCONFIRM", Menu("QUITTOMAINCONFIRM", List<Menu::Button>({
				Menu::TextBox("TEXTBOX1", "GUI_QUITTEXT", "SMALL", {50, 20}),
				Menu::standardButton("QUITMAINNO", "GUI_CANCELBUTTON", {50, 45}),
				Menu::standardButton("QUITMAINYES", "GUI_QUITTOMAINBUTTON", {50, 60}),
				}),Map<string, string>({
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " QUITMAINNO")}))),
		pair<string, Menu>(
			"PARTYREFORM", Menu("PARTYREFORM", List<Menu::Button>({
				Menu::TextBox("TEXTBOX1", "GUI_PARTYLABEL", "VERY_SMALL", {40, 10}),
				Menu::TextBox("TEXTBOX2", "GUI_RESERVELABEL", "VERY_SMALL", {75, 10}),
				Menu::standardButton("FROMPARTYREFORMTOPARTYMANAGEMENT", "GUI_FROMAUDIOTOPAUSEBUTTON", {12, 90}),
				}),Map<string, string>({
					pair<string, string>("PARTYREFORM", "1"),
					pair<string, string>("CHARACTERTOEDITSCALE", "0.25"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMPARTYREFORMTOPARTYMANAGEMENT")}))),
		pair<string, Menu>(
			"PARTYMANAGEMENT", Menu("PARTYMANAGEMENT", List<Menu::Button>({
				Menu::standardButton("FROMPARTYMANAGEMENTTOSKILLMANAGE", "GUI_FROMPARTYMANAGEMENTTOSKILLMANAGE", {12, 49.8}),
				Menu::standardButton("FROMPARTYMANAGEMENTTOEQUIPMENT", "GUI_FROMPARTYMANAGEMENTTOEQUIPMENT", {12, 60}),
				Menu::standardButton("FROMPARTYMANAGEMENTTOPARTYREFORM", "GUI_FROMPARTYTOREFORM", {12, 70}),
				Menu::standardButton("FROMPAUSETOTOME", "GUI_FROMPAUSETOTOME", {12, 80}),
				Menu::Button("equipmentToggle","GUI_NOTEXT","GUI_NOTEXT", "equipmentToggle",{SHOWEQUIPMENT},BUTTON_CLICK_WAV, BUTTON_HOVER_WAV,{25,60}, true,true,false, true, Map<string, string>({
					pair<string, string>("hasHoverText", "1"),
					pair<string, string>("hoverTextName", "equipmentToggleText"),
					pair<string, string>("hoverTextContent", "equipmentToggleText"),
					pair<string, string>("hoverTextContentX", "15"),
					pair<string, string>("hoverTextContentY", "55"),
					pair<string, string>("hoverAnchorStyle", "TOPLEFT"),
					pair<string, string>("hoverTextFormat", "Centaur_12"),
					})),
				Menu::standardButton("FROMPARTYTOEXPLOREPAUSE", "GUI_FROMPARTYTOPAUSE", {12, 90}),
				}),Map<string, string>({
					pair<string, string>("PARTYSHUFFLE", "1"),
					pair<string, string>("SKILLEXPLAIN", "1"),
					pair<string, string>("ATTPOINTMANAGE", "1"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMPARTYTOEXPLOREPAUSE")}))),
		pair<string, Menu>(
			"SKILLMANAGEMENT", Menu("SKILLMANAGEMENT", List<Menu::Button>({
				Menu::TextBox("TEXTBOX1", "GUI_SELECTCHARACTER", "VERY_SMALL", {15, 7}),
				Menu::standardButton("FROMSKILLMANAGETOPARTYMANAGE", "GUI_FROMPARTYTOPAUSE", {12, 90}),
				Menu::smallButton("SKILLTREE1", "GUI_SKILLTREESELECTIONEMPTY", {45, 8}),
				Menu::smallButton("SKILLTREE2", "GUI_SKILLTREESELECTIONEMPTY", {75, 8}),
			}),Map<string, string>({
					pair<string, string>("SKILLEXPLAIN", "2"),
					pair<string, string>("SELECTCHARACTERTOEDIT", "1"),
					pair<string, string>("CHARACTERTOEDITOFFSETX", "5"),
					pair<string, string>("CHARACTERTOEDITOFFSETY", Menu::skillGridYLoc()),
					pair<string, string>("CHARACTERTOEDITSCALE", "0.25"),
					pair<string, string>("MANAGESKILLS", "1"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMSKILLMANAGETOPARTYMANAGE")}))),
		pair<string, Menu>(
			"EQUIPMENTMANAGEMENT", Menu("EQUIPMENTMANAGEMENT", Menu::getDefaultButtonsForEquipmentSelect(),Map<string, string>({
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
					pair<string, string>("SELECTCHARACTERTOEDIT", "1"),
					pair<string, string>("CHARACTERTOEDITOFFSETX", "5"),
					pair<string, string>("CHARACTERTOEDITOFFSETY", Menu::skillGridYLoc()),
					pair<string, string>("CHARACTERTOEDITSCALE", "0.25"),
					pair<string, string>("MANAGEEQUIPMENT", "1"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMEQUIPMANAGETOPARTYMANAGE")}))),
		pair<string, Menu>(
			"SKILLTREEEDIT1", Menu("SKILLTREEEDIT1", List<Menu::Button>({}),Map<string, string>({
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["DROPDOWNMENU"])),
					pair<string, string>("SKILLTREEEDIT", "1"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " CANCELSKILLTREECHOICE")}))),
		pair<string, Menu>(
			"SKILLTREEEDIT2", Menu("SKILLTREEEDIT2", List<Menu::Button>({}),Map<string, string>({
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["DROPDOWNMENU"])),
					pair<string, string>("SKILLTREEEDIT", "2"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " CANCELSKILLTREECHOICE")}))),
		pair<string, Menu>(
			"EQUIPMENTEDIT", Menu("EQUIPMENTEDIT", List<Menu::Button>({
					Menu::standardButton("CANCELEQUIPCHOICE", "GUI_CANCELSKILLTREECHOICE", {45, 7}),
					Menu::standardButton("Weapon", "GUI_Weapon", {45, 17}),
					Menu::standardButton("Armour", "GUI_Armour", {45, 27}),
					Menu::standardButton("Accessory", "GUI_Accessory", {45, 37}),
				}),Map<string, string>({
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["DROPDOWNMENU"])),
					pair<string, string>("EQUIPMENTCHOICE", "1"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " CANCELEQUIPCHOICE")}))),
		pair<string, Menu>(
			"MERCHANT", Menu("MERCHANT", Menu::getDefaultButtonsForMerchant(),Map<string, string>({
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["DROPDOWNMENU"])),
					pair<string, string>("MERCHANT", "Buy"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " MERCHANTTOEXPLORE")}))),
		pair<string, Menu>(
			"MERCHANTCONFIRM", Menu("MERCHANTCONFIRM", Menu::getDefaultButtonsForConfirmTrade(),Map<string, string>({
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["DROPDOWNMENU"])),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " TradeConfirmNo")}))),
		pair<string, Menu>(
			"TRADEDENY", Menu("TRADEDENY", Menu::getDefaultButtonsForDenyTrade("GUI_CAN'TAFFORD"),Map<string, string>({
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["DROPDOWNMENU"])),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " TradeConfirmNo")}))),
		pair<string, Menu>(
			"TRADEDENYFULL", Menu("TRADEDENY", Menu::getDefaultButtonsForDenyTrade("GUI_NOSPACE"), Map<string, string>({
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["DROPDOWNMENU"])),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " TradeConfirmNo") }))),
		pair<string, Menu>(
			"YOUFOUNDANITEM", Menu("YOUFOUNDANITEM", Menu::getDefaultButtonsForFindingItems(), Map<string, string>({
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["DROPDOWNMENU"])),
							pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " AcceptItem"),
							pair<string, string>("BUTTONMAP2", to_string(VK_RETURN) + " AcceptItem"),
							pair<string, string>("BUTTONMAP3", to_string(VK_SPACE) + " AcceptItem")
}))),
		pair<string, Menu>(
			"TUTORIALCONFIRM", Menu("TUTORIALCONFIRM", List<Menu::Button>({
				Menu::TextBox("TEXTBOX1", "GUI_TUTTEXTBOX", "SMALL", {50, 20}),
				Menu::standardButton("TUTNO", "GUI_TUTNO", {50, 45}),
				Menu::standardButton("TUTYES", "GUI_TUTYES", {50, 60}),
				}), Map<string, string>({}))),
		pair<string, Menu>(
			"COMBAT1", Menu("COMBAT1", {}, Map<string, string>({
				pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " COMBAT1CANCEL"),
				pair<string, string>("BUTTONMAP2", to_string(0x51) + " COMBAT2QUIT"),
			}))),
		pair<string, Menu>(
			"COMBATQUIT", Menu("COMBATQUIT", List<Menu::Button>({
					Menu::TextBox("TEXTBOX1", "GUI_QUITCOMBAT", "VERY_SMALL", {50, 30}),
					Menu::smallButton("QUITCOMBAT", "GUI_Yes", {33, 50}),
					Menu::smallButton("COMBATQUITCANCEL", "GUI_No", {66, 50}),
			}), Map<string, string>({
				pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " COMBATQUITCANCEL"),
			}))),
		pair<string, Menu>(
			"COMBATRESULT", Menu("COMBATRESULT", List<Menu::Button>({
			Menu::TextBox("TEXTBOX1", "GUI_POSTBATTLE", "LARGE", {50, 10}),
			}), Map<string, string>({}))),
		pair<string, Menu>(
			"CODEX1", Menu("CODEX1", {}, Map<string, string>({
				pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMCODEXTOPAUSE"),
			}))),
		pair<string, Menu>(
			"TOME", Menu("TOME", Menu::getDefaultMenuButtonsForTomes(), Map<string, string>({
				pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMTOMETOPAUSE"),
			}))),
			pair<string, Menu>(
			"SPIGOTPUZZLE", Menu("SPIGOTPUZZLE", Menu::getButtonsForSpigotPuzzle(), Map<string, string>({
				pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMSPIGOTTOEXPLORE"),
			}))),

	});
	Procedure makeDynamicCutsceneProcedure(string language, string cutsceneName, string player, string postProcedure) {
		cutsceneName = SReplace(cutsceneName, "PLAYER", saveContainer.getPossibleCutsceneParticipants().front());
		return Procedure("Cutscene", List<Event>(convertDynamicStringsToDialogue(language, cutsceneName, player) + 
			List<Event>(Event("PostCutscene", "TEARDOWNDIALOGUE", {})) + List<Event>(Event("PostCutscene", postProcedure, {}))
		));
	}
	Procedure makeAreaTransitionProcedure(string fromMap, string toMap, pair<float, float> playerPosition, string playerDirection, Map<string,string> walkableData) {
		List<Event> events;
		if (walkableData.hasKey("audio")) {
			events.push_back(Event("Play AT Audio", "PLAYSFX", Map<string, string>({
				pair<string, string>("audio",walkableData["audio"]),
				})));
		}
		events.push_back(Event("ShowLoadingScreen", "SHOWMAPLOADINGSCREEN", Map<string, string>({
			pair<string, string>("targetMap",toMap),
			})));
		events.push_back(Event("TearDownOtherImages", "UNLOADIMAGESFORMAPCHANGE", {}));
		events.push_back(Event("WaitASec", "WAIT", Map<string, string>({
			pair<string, string>("clockID","AreaTransitionWait"),
			pair<string, string>("waitDuration","50"),
			})));
		events.push_back(Event("SwapAudio", "MANAGEAUDIOSWAP", {
			pair<string, string>("targetMap",toMap),
			}));
		events.push_back(Event("LoadNewMap", "LOADMAP", Map<string, string>({
			pair<string, string>("targetMap",toMap),
			pair<string, string>("putPlayerHere", "1"),
			pair<string, string>("x",to_string(playerPosition.first)),
			pair<string, string>("y",to_string(playerPosition.second)),
			pair<string, string>("direction", walkableData["direction"]),
			})));
		events.push_back(Event("FadeOut", "ANIMATEIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", "LoadingScreen"),
				pair<string, string>("styles", "FADEOUT"),
				pair<string, string>("speed", "100"),
				pair<string, string>("wait", "TRUE"), }))));
		events.push_back(Event("TeardownImage", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", "LoadingScreen"), }))));
		if (walkableData["don'tAddExploreAtTheEnd"] != "1") {
			events.push_back(Event("Explore", "EXPLORE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("force","1"),
				pair<string, string>("noaudioyet","1"),
				pair<string, string>("audio","1"), }))));
		}
		return Procedure("AreaTransition", events);
	}
	Procedure makeMerchantLoadProcedure(string whichMerchant) {
		Procedure result = makeLoadMenuProcedure("MERCHANT");
		for (auto & event : result.eventList.internalList) {
			event.data["whichMerchant"] = whichMerchant;
		}
		return result;
	}
	Procedure makeLoadMenuProcedure(string whichMenu) {
		List<Event> events;
		events.push_back(Event("LoadAMenu", "LOADMENU", Map<string, string>({
			pair<string, string>("uniqueID", whichMenu),
			})));
		events.push_back(Event("HandleMenu", "HANDLEMENU", Map<string, string>({
			pair<string, string>("uniqueID", whichMenu),
			})));
		if (storedMenus[whichMenu].data.getKeys().contains("layer")) {
			events.at(0).data["layer"] = storedMenus[whichMenu].data["layer"];
		}
		return Procedure("MenuProcedure", events);
	}
	Procedure makeOpenChestProcedure(Map<string, string> data) {
		List<Event> events;
		events.push_back(Event("PlayOpenChestSound", "PLAYSFX", Map<string, string>({
			pair<string, string>("audio", "OPENCHEST")
			})));
		events.push_back(Event("HandleChest", "HANDLECHESTANIMATION", data));
		events.push_back(Event("PlayItemSound", "PLAYSFX", Map<string, string>({
			pair<string, string>("audio", to_string(ITEMFIND_WAV)),
			pair<string, string>("direct", "1")
			})));
		events.push_back(Event("LoadAMenu", "LOADMENU", Map<string, string>({
			pair<string, string>("uniqueID", "YOUFOUNDANITEM"),
			pair<string, string>("contents", data["contents"]),
			})));
		events.push_back(Event("HandleMenu", "HANDLEMENU", Map<string, string>({
			pair<string, string>("uniqueID", "YOUFOUNDANITEM"),
			pair<string, string>("objectID", data["imageID"]),
			pair<string, string>("contents", data["contents"]),
			})));
		return Procedure("HandleChest", events);
	}
	Procedure makeCombatProcedure(Map<string, string> data) {
		List<Event> events;
		events.push_back(Event("PlayItemSound", "PLAYSFX", Map<string, string>({
			pair<string, string>("audio", to_string(COMBATSOUNDSTART_1)),
			pair<string, string>("direct", "1")
			})));
		events.push_back(Event("ShowLoadingScreen", "SHOWMAPLOADINGSCREEN", Map<string, string>({
				pair<string, string>("direct",to_string(COMBATLOADINGSCREEN_1)),
				pair<string, string>("speed", "50"),
			})));
		events.push_back(Event("TearDownOtherImages", "UNLOADIMAGESFORMAPCHANGE", {}));
		events.push_back(Event("WaitASec", "WAIT", Map<string, string>({
			pair<string, string>("clockID","AreaTransitionWait"),
			pair<string, string>("waitDuration","50"),
			})));
		events.push_back(Event("SwapAudio", "STOPALLSONGS", {}));
		if (data["song"] != "") {
			events.push_back(Event("Play This Song", "PLAYTHISSONG", Map<string, string>(List<pair<string, string>>({
					pair<string, string>("uniqueID", data["song"]),
				}))));
		}
		events.push_back(Event("StartCombat", "SETUPBATTLE", data));
		events.push_back(Event("FadeOut", "ANIMATEIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", "LoadingScreen"),
				pair<string, string>("styles", "FADEOUT"),
				pair<string, string>("speed", "100"),
				pair<string, string>("wait", "TRUE"), }))));
		events.push_back(Event("TeardownImage", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", "LoadingScreen"), }))));
		events.push_back(Event("WaitASec", "WAIT", Map<string, string>({
			pair<string, string>("clockID","BattleStartWait"),
			pair<string, string>("waitDuration","100"),
			})));
		events.push_back(Event("HandleCombatLoop", "HANDLECOMBAT", {}));
		return Procedure("StartCombat ", events);
	}
	List<Event> convertDynamicStringsToDialogue(string language, string cutsceneName, string player) {
		// dynamic as in, the line changes depending on who the player is
		// for example, player interacting with something on the map
		// other cutscenes have all hardcoded speakers
		List<Event> results;
		List<string> acceptedLines;
		Map<string, wstring> lines; lines.internalMap = strings[language][cutsceneName];
		Map<float, List<string>> sortedKeys;
		float currentLine = 1;
		for (auto s : lines.getKeys().internalList) {
			float number_only = stof(split(s, " ").at(0));
			sortedKeys[number_only].push_back(s);
		}

		string mainCharacter = saveContainer.getPossibleCutsceneParticipants().front();
		string player2 = saveContainer.getNamesOfTwoPeopleForScene().second;
		for (auto const sortedKey : sortedKeys.getKeys().internalList) {
			List<string> keys = sortedKeys[sortedKey];
			for (auto key : keys.internalList) {
				wstring val = lines[key];
				string thisLine = split(key, " ").at(0);
				if (acceptedLines.contains(thisLine)) { continue; }
				string speaker = "";
				for (int x = key.find(" ") + 1; x < key.size(); x++) {
					speaker += key.at(x);
				}
				if (speaker.find("$PLAYER2==") != -1) {
					string whichPlayer2 = split(speaker, "==").at(1);
					whichPlayer2 = SReplace(whichPlayer2, "$ASYNC$", "");
					if (whichPlayer2 == player2) {
						results.push_back(Event(thisLine, "DIALOGUE", { Map<string, string>(List<pair<string,string>>({
							pair<string, string>({"cutscene", cutsceneName}),
							pair<string, string>({"speaker", speaker}),
					})) }));
						acceptedLines.push_back(thisLine);
						continue;
					}
				}
				if (speaker.find("$DIRECT$") != -1) {
					// use the speaker name directly
					results.push_back(Event(thisLine, "DIALOGUE", { Map<string, string>(List<pair<string,string>>({
					pair<string, string>({"cutscene", cutsceneName}),
					pair<string, string>({"speaker", speaker}),
					})) }));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$CAMERAMOVE$") {
					// instruction to move the camera
					List<string> data = split(WStringToString(val), " ");
					results.push_back(Event(thisLine, "MOVECAMERA", { Map<string, string>(List<pair<string,string>>({
					pair<string, string>({"x", data.at(0)}),
					pair<string, string>({"y", data.at(1)}),
					pair<string, string>({"speed", data.at(2)}),
					})) }));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$STOPDIALOGUE$") {
					results.push_back(Event("PostCutscene", "TEARDOWNDIALOGUE", {}));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$LOADOBJECT$") {
					List<string> parsed_data = split(WStringToString(val), "_");
					results.push_back(Event("LoadObject", "LOADOBJECT", Map<string, string>({
						pair<string, string>("name", parsed_data.at(0)),
						pair<string, string>("x", parsed_data.at(1)),
						pair<string, string>("y", parsed_data.at(2)),
						pair<string, string>("layer", parsed_data.at(3)),
						pair<string, string>("animationSpeed", parsed_data.at(4)),
						pair<string, string>("sources_1", parsed_data.at(5)),
						pair<string, string>("sources_2", parsed_data.at(6)),
						pair<string, string>("sources_3", parsed_data.at(7)),
						pair<string, string>("scale", parsed_data.at(8)),
						pair<string, string>("opacity", parsed_data.at(9)),
						pair<string, string>("followPlayer", parsed_data.at(10)),
						pair<string, string>("anchor", parsed_data.at(11)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$TELEPORTOBJECT$") {
					List<string> parsed_data = split(WStringToString(val), "_");
					results.push_back(Event("Teleport", "TELEPORTOBJECT", Map<string, string>({
						pair<string, string>("uniqueID", parsed_data.at(0)),
						pair<string, string>("x", parsed_data.at(1)),
						pair<string, string>("y", parsed_data.at(2)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$ANIMATEOBJECT$") {
					List<string> parsed_data = split(WStringToString(val), "_");
					results.push_back(Event("Animate Image", "ANIMATEIMAGEONMAP", Map<string, string>({
						pair<string, string>("whichImage", parsed_data.at(0)),
						pair<string, string>("character", parsed_data.at(1)),
						pair<string, string>("action", parsed_data.at(2)),
						pair<string, string>("direction", parsed_data.at(3)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$CHANGEANIMATIONSPEED$") {
					List<string> parsed_data = split(WStringToString(val), "$");
					results.push_back(Event("ChangeSpeed", "CHANGEANIMATIONSPEED", Map<string, string>({
						pair<string, string>("uniqueID", parsed_data.at(0)),
						pair<string, string>("animationSpeed", parsed_data.at(1)),
						pair<string, string>("styles", parsed_data.at(2)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$WAITFORANIMATION$") {
					results.push_back(Event("ChangeSpeed", "WAITFORANIMATION", Map<string, string>({
						pair<string, string>("uniqueID", WStringToString(val)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$MOVEOBJECTS$") {
					List<string> parsed_data = split(WStringToString(val), "$");
					results.push_back(Event("LoadObject", "MOVEOBJECTS", Map<string, string>({
						pair<string, string>("whichObjects", parsed_data.at(0)),
						pair<string, string>("targetPositions", parsed_data.at(1)),
						pair<string, string>("speed", parsed_data.at(2)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$MOVEPLAYERCUTSCENE$") {
					List<string> parsed_data = split(WStringToString(val), "$");
					results.push_back(Event("MovePlayer", "MOVEPLAYERCUTSCENE", Map<string, string>({
						pair<string, string>("targetX", parsed_data.at(0)),
						pair<string, string>("targetY", parsed_data.at(1)),
						pair<string, string>("audio", parsed_data.at(2)),
						pair<string, string>("speed", parsed_data.at(3)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$WAIT$") {
					List<string> parsed_data = split(WStringToString(val), "_");
					results.push_back(Event("Wait", "STARTCLOCK", Map<string, string>({
						pair<string, string>("uniqueID", parsed_data.at(0)),
						})));
					results.push_back(Event("Wait", "WAIT", Map<string, string>({
						pair<string, string>("clockID", parsed_data.at(0)),
						pair<string, string>("waitDuration", parsed_data.at(1)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$PLAYSFX$") {
					List<string> parsedData = split(WStringToString(val), "_");
					results.push_back(Event("PlayAudio", "PLAYSFX", Map<string, string>({
						pair<string, string>("audio", parsedData.at(0)),
						pair<string, string>("delay", parsedData.at(1)),
						pair<string, string>("direct", parsedData.at(2)),
						})));
					acceptedLines.push_back(thisLine);
					continue; 
				}
				if (speaker == "$PLAYSONG$") {
					List<string> parsedData = WStringToString(val);
					results.push_back(Event("PlayAudio", "PLAYTHISSONG", Map<string, string>({
						pair<string, string>("uniqueID", parsedData.at(0)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$STOPSONG$") {
					List<string> parsedData = WStringToString(val);
					results.push_back(Event("PlayAudio", "STOPTHISSONG", Map<string, string>({
						pair<string, string>("uniqueID", parsedData.at(0)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$FROMCUTSCENETOMENU$") {
					results.push_back(Event("LoadMenu", "FROMCUTSCENETOMENU", Map<string, string>({
						pair<string, string>("menuName", WStringToString(val)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$STARTCUTSCENEDIRECTLY$") {
					string whichCutscene = SReplace(WStringToString(val), "PLAYER",saveContainer.getPossibleCutsceneParticipants().front());
					results.push_back(Event("StartCutscene", "STARTCUTSCENEDIRECTLY", Map<string, string>({
						pair<string, string>("cutsceneName", whichCutscene),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$CHANGEOPACITY$") {
					List<string> parsed_data = split(WStringToString(val), "$");
					results.push_back(Event("LoadObject", "CHANGEOPACITY", Map<string, string>({
						pair<string, string>("uniqueID", parsed_data.at(0)),
						pair<string, string>("opacity", parsed_data.at(1)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$REMOVEIMAGES$") {
					results.push_back(Event("LoadObject", "REMOVEIMAGES", Map<string, string>({
						pair<string, string>("toRemove",WStringToString(val)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$HANDLESKILLANIMATION$") {
					List<string> parsed_data = split(WStringToString(val), "$");
					results.push_back(Event("LoadObject", "HANDLESKILLANIMATION", Map<string, string>({
						pair<string, string>("caster", parsed_data.at(0)),
						pair<string, string>("target", parsed_data.at(1)),
						pair<string, string>("skill", parsed_data.at(2)),
						pair<string, string>("extras", parsed_data.at(3)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$AREATRANSITION$") {
					List<string> parsed_data = split(WStringToString(val), "$");
					string currentMap = explorer.currentMap.name;
					string targetMap = parsed_data.at(0);
					List<string> futurePlayerPositionAsString = split(parsed_data.at(1), ",");
					pair<float, float> futurePlayerPositionAsFloat = {stof(futurePlayerPositionAsString.at(0)), stof(futurePlayerPositionAsString.at(1))};
					string futurePlayerDirection = parsed_data.at(2);
					Map<string, string> walkableDataToMoveOn = stringMapCompose(parsed_data.at(3), ",");
					walkableDataToMoveOn["don'tAddExploreAtTheEnd"] = "1";
					acceptedLines.push_back(thisLine);
					Procedure P = makeAreaTransitionProcedure(currentMap, targetMap, futurePlayerPositionAsFloat, futurePlayerDirection, walkableDataToMoveOn);
					for (auto e : P.eventList.internalList) {
						results.push_back(e);
					}
					continue;
				}
				if (speaker == "$CHANGECAMERAOPERTATION$") {
					string parsedData = WStringToString(val);
					results.push_back(Event("LoadObject", "CHANGECAMERAOPERTATION", Map<string, string>({
						pair<string, string>("followPlayer", parsedData),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$TELEPORTCAMERA$") {
					List<string> parsed_data = split(WStringToString(val), ",");
					results.push_back(Event("LoadObject", "TELEPORTCAMERA", Map<string, string>({
						pair<string, string>("x", parsed_data.at(0)),
						pair<string, string>("y", parsed_data.at(1)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$ADDTOCHARACTERS$") {
					results.push_back(Event("AddToCharacters", "ADDTOCHARACTERS", Map<string, string>({
							pair<string, string>("WHO", WStringToString(val)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$FORCETHISIMAGETOGOTOLASTFRAME$") {
					results.push_back(Event("ChangeFrame", "FORCEIMAGEFRAME", Map<string, string>({
						pair<string, string>("uniqueID",WStringToString(val)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$STARTCOMBAT$") {
					Map<string, string> data = stringMapCompose(WStringToString(val), ",");
					for (auto key : data.getKeys().internalList) {
						if (data[key] == "N/A") {
							data[key] = "";
						}
					}
					results.push_back(Event("StartCombat", "STARTCOMBAT", data));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$MANIPULATEOBJECT$") {
					List<string> parsedData = split(WStringToString(val), "$");
					Map<string, string> data;
					data["objectName"] = parsedData.at(0);
					data["action"] = parsedData.at(1);
					data["condition1"] = parsedData.at(2);
					data["condition2"] = parsedData.at(3);
					results.push_back(Event("Manip", "MANIPOBJECT", data));
					continue;
				}
				if (speaker == "$SETFLAG$") {
					List<string> parsedData = split(WStringToString(val), "=");
					Map<string, string> data;
					data["uniqueID"] = parsedData.at(0);
					data["status"] = parsedData.at(1);
					results.push_back(Event("SetFlag", "SETFLAGANDSAVE", data));
					continue;
				}
				if (speaker == "$OPENMENU$") {
					string menuName = WStringToString(val);
					results.push_back(Event("Wait", "LOADMENU", Map<string, string>({
						pair<string, string>("uniqueID", menuName),
						})));
					results.push_back(Event("Wait", "HANDLEMENU", Map<string, string>({
						pair<string, string>("uniqueID", menuName),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "TEARDOWNMAPPOPUPTEXT") {
					results.push_back(Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", explorer.mapPopupTextID})));
					acceptedLines.push_back(thisLine);
					continue;
				}

				if (speaker == "$RUNPUZZLELOGIC$") {
					Map<string, string> data;
					data["whichPuzzle"] = WStringToString(val);
					results.push_back(Event("Puzzle", "RUNPUZZLELOGIC", data));
					continue;
				}
				if (speaker == "$CHANGEIMAGELAYER$") {
					List<string> parsedData = split(WStringToString(val), "_");
					Map<string, string> data;
					data["uniqueID"] = parsedData.at(0);
					data["layer"] = parsedData.at(1);
					results.push_back(Event("ChangeImageLayer", "CHANGEIMAGELAYER", data));
					continue;
				}
				if (speaker == "$SETOBSTRUCTION$") {
					List<string> parsedData = split(WStringToString(val), "_");
					Map<string, string> data;
					data["uniqueID"] = parsedData.at(0);
					data["isObstruction"] = parsedData.at(1);
					results.push_back(Event("ChangeImageLayer", "SETOBSTRUCTION", data));
					continue;
				}
				if (speaker != "PLAYER" and speaker != mainCharacter and speaker != "PLAYER$ASYNC$" and speaker != "EMPTY$ASYNC$" and speaker != "PLAYER2") {
					// Line belongs to a different playable character
					continue;
				}
				// either the speaker is the main character or SPEAKER (line shared across all playable characters)
				results.push_back(Event(thisLine, "DIALOGUE", { Map<string, string>(List<pair<string,string>>({
					pair<string, string>({"cutscene", cutsceneName}),
					pair<string, string>({"speaker", speaker}),
					})) }));
				acceptedLines.push_back(thisLine);
			}
		}
		return results;
	}
	GameEngine() {}
	void setup() {
		string mode = Args.get("mode");
		if (mode == "DEBUG") {
			explorer.keepMapFullyOnScreen = true;
			activeProcedure = storedProcedures["DEBUG3"];
		}
		else if (mode == "NORMAL") {
			activeProcedure = storedProcedures["BOOTMENU"];
		}
		else {
			activeProcedure = storedProcedures["BOOTMENU"];
		}
		CLOCK.startClock("FPS");
		stateFlags["QUIT"] = "0";
		stateFlags["includeEquipmentInStatView"] = "1";
		stateFlags["combatVerbosity"] = "0";
	}
	bool quit() {
		return stateFlags["QUIT"] == "1";
	}
	void run() {
		if (stateFlags["SHOWFPS"] == "1") {
			Event("ShowFPS", "SHOWFPS", {}).run(*this);
		}
		activeProcedure.run(*this);
	}
	void changeLanguage(string lang) {
		language = lang;
		combat.language = lang;
	}

	Procedure activeProcedure;
	Map<string, string> stateFlags;
	string language = "ENG";
	SkillAnimationContainer skillAnimationContainer;
};
GameEngine game;