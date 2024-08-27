#pragma once

#include "Graphics.h"

class Menu {
public:
	Menu() {}
	class Button {
	public:
		Button() {}
		Button(string _uniqueID, string _textID, string _buttonContent, string _imageID, List<int> _sources, int _audioClick, int _audioHover, pair<float, float> _position, bool _visible, bool _clickable) {
			uniqueID = _uniqueID;
			textID = _textID;
			imageID = _imageID;
			sources = _sources;
			audioClick = _audioClick;
			audioHover = _audioHover;
			visible = _visible;
			position = _position;
			buttonContent = _buttonContent;
			clickable = _clickable;
		}
		string getSources() {
			string result = "";
			for (auto x : sources.internalList) {
				result += to_string(x);
				result += " ";
			}
			return result;
		}
		string uniqueID;
		string textID;
		string imageID;
		string buttonContent;
		List<int> sources;
		pair<float, float> position;
		string anchorStyle = "CENTRE";
		int audioClick;
		int audioHover;
		bool visible;
		bool clickable;
		float width = 100;
		float height = 0;
	};
	Menu(string uniqueID, List<Button> _buttons, Map<string, string> _data) {
			buttons = _buttons;
			data = _data;
		}
	Map<int, string> getKeyboardShortcutsForThisMenu() {
		Map<int, string> result;
		for (auto & node : data.getKeys().internalList) {
			if (node.find("BUTTONMAP") != -1) {
				int whichKey = stoi(split(data[node], " ").at(0));
				string whichButton = split(data[node], " ").at(1);
				result[whichKey] = whichButton;
			}
		}
		return result;
	}
	List<Button> getClickableButtons() {
		List<Button> results;
		for (auto button : buttons.internalList) {
			if (button.clickable and button.visible) {
				results.push_back(button);
			}
		}
		return results;
	}

	static Button standardButton(string baseID, string buttonMessage, pair<float, float> position) {
		Button result;
		result.uniqueID = baseID;
		result.textID = baseID + "_TEXT";
		result.imageID = baseID + "_IMAGE";
		result.sources = { BUTTON1_DEFAULT, BUTTON1_HOVERED, BUTTON1_PRESSED };
		result.audioClick = BUTTON_CLICK_WAV;
		result.audioHover = BUTTON_HOVER_WAV;
		result.buttonContent = buttonMessage;
		result.position = position;
		result.visible = true;
		result.clickable = true;
		result.anchorStyle = "CENTRE";
		return result;
	}
	static Button plusButton(string baseID) {
		Button result;
		result.uniqueID = baseID;
		result.textID = "";
		result.imageID = baseID;
		result.sources = { PLUSBUTTON_DEFAULT, PLUSBUTTON_HOVERED, PLUSBUTTON_PRESSED };
		result.audioClick = PLUS_WAV;
		result.audioHover = BUTTON_HOVER_WAV;
		result.buttonContent = "";
		result.visible = true;
		result.clickable = true;
		result.anchorStyle = "CENTRE";
		return result;
	}
	static Button minusButton(string baseID) {
		Button result;
		result.uniqueID = baseID;
		result.textID = "";
		result.imageID = baseID;
		result.sources = { MINUSBUTTON_DEFAULT, MINUSBUTTON_HOVERED, MINUSBUTTON_PRESSED };
		result.audioClick = MINUS_WAV;
		result.audioHover = BUTTON_HOVER_WAV;
		result.buttonContent = "";
		result.visible = true;
		result.clickable = true;
		result.anchorStyle = "CENTRE";
		return result;
	}
	static Button TextBox(string baseID, string buttonMessage, string size, pair<float, float> position) {
		Button result;
		result.uniqueID = baseID;
		result.textID = baseID + "_TEXT";
		result.imageID = baseID + "_IMAGE";
		result.visible = true;
		result.clickable = false;
		result.buttonContent = buttonMessage;
		result.position = position;
		result.anchorStyle = "CENTRE";
		if (size == "SMALL") {
			result.sources = List<int>({ TEXTBOX_SMALL });
			result.width = 30;
			result.height = 30;
		}
		return result;
	}
	
	static const List<pair<float, float>> getPlayerCardPositions() {
		return List<pair<float, float>>({pair<float, float>(35,25),pair<float, float>(53,25),pair<float, float>(71,25),pair<float, float>(89,25)});
	}

	string uniqueID;
	List<Button> buttons;
	Map<string, string> data;
};

class Combat {
public:
	Combat() {
		influenceLookups["STRENGTH"] = 0.05;
		influenceLookups["INTELLIGENCE"] = 0.05;
		influenceLookups["VITALITY"] = 0.08;
		influenceLookups["PIETY"] = 0.15;
		influenceLookups["LUCK"] = 0.1;
		influenceLookups["SPEED"] = 0.1;
		influenceLookups["AGILITY"] = 0.15;
		skillDefinitions["DEFAULT_ATTACK"] = Skill("DEFAULT_ATTACK", "Attack", SKILLICON_ATTACK, 0, 0, 0, list<string>({"PHYSICAL","ATTACK"}), Map<string, PowerValue>({
			pair<string, PowerValue>("DAMAGE1",PowerValue("DAMAGE1",10,0,999,true,list<string>({"Strength"}))) }), list<string>({ "DEALDAMAGE","DEALPHYSICALDAMAGE" }), list<CombatAnimationNode>({ CombatAnimationNode("DEFAULT_ATTACK","DEFAULT_ATTACK", {}) }));
		AttributesInOrder = {"VITALITY","PIETY","STRENGTH", "INTELLIGENCE", "AGILITY","LUCK"};
		statsInOrder = {"LIFE","ENERGY","ENERGYREGEN", "SPEED"};
		defaultAttInvestments["Angela Fleuret"].internalMap = {
			{"STRENGTH" , 2},
			{"INTELLIGENCE" , 3},
			{"VITALITY", 2},
			{"PIETY", 12},
			{"AGILITY", 0},
			{"LUCK", 1}
		};
		defaultAttInvestments["Tianshun Song"].internalMap = {
			{"STRENGTH" , 0},
			{"INTELLIGENCE" , 8},
			{"VITALITY", 0},
			{"PIETY", 8},
			{"AGILITY", 4},
			{"LUCK", 0}
		};
		defaultAttInvestments["Olyver Sumner"].internalMap = {
			{"STRENGTH" , 1},
			{"INTELLIGENCE" , 9},
			{"VITALITY", 0},
			{"PIETY", 5},
			{"AGILITY", 3},
			{"LUCK", 2}
		};
		defaultAttInvestments["Hernando Pizarro"].internalMap = {
			{"STRENGTH" , 10},
			{"INTELLIGENCE" , 0},
			{"VITALITY", 5},
			{"PIETY", 0},
			{"AGILITY", 3},
			{"LUCK", 2}
		};
		defaultAttInvestments["Gihat al-Din Jaqmaq"].internalMap = {
			{"STRENGTH" , 5},
			{"INTELLIGENCE" , 0},
			{"VITALITY", 2},
			{"PIETY", 4},
			{"AGILITY", 5},
			{"LUCK", 4}
		};
	}
	class CombatAnimationNode {
	public:
		CombatAnimationNode(string _name, string _procedure, List<int> _sources) {
			name = _name;
			procedure = _procedure;
			sources = _sources;
		}
		bool handleAnimation() {
			// do stuff
			return true;
		}
		string name;
		string procedure;
		List<int> sources;
	};
	class Effect {
	public:
		Effect() {}
		Effect(string _name, int _imageSource, bool _positive, List<string> _tags, int _remainingDuration) {
			name = _name;
			imageSource = _imageSource;
			positive = _positive;
			tags = _tags;
			remainingDuration = _remainingDuration;
		}
		string name;
		int imageSource;
		bool positive;
		List<string> tags;
		int remainingDuration;
	};
	class PowerValue {
	public:
		PowerValue() {}
		PowerValue(string _name, float _baseValue, float _min, float _max, bool _goesUpwards, List<string> _influenceTags) {
			name = _name;
			baseValue = _baseValue;
			min = _min;
			max = _max;
			goesUpwards = _goesUpwards;
			influenceTags = _influenceTags;
		}
		int getResultantValue(Combat & combat, Map<string, int> influences) {
			float result = baseValue;
			float multiplier = 1.0;
			if (!goesUpwards) {
				multiplier*=-1;
			}
			for (auto tag : influences.getKeys().internalList) {
				if (influenceTags.contains(tag)) {
					multiplier += (influences[tag] * combat.influenceLookups[tag]);
				}
			}
			result *= multiplier;
			return round(result);
		}
		wstring getResultantValueAsString(Combat & combat, Map<string, int> influences) {
			int result = getResultantValue(*&combat, influences);
			return to_wstring(result);
		}
		string name;
		float baseValue;
		float min;
		float max;
		bool goesUpwards = true; // some skills may have a number that goes down as power goes up, like casting faster or cheaper
		List<string> influenceTags;
	};
	class Skill {
	public:
		Skill() {}
		Skill(string _uniqueID, string _displayName, int _imageSource, int _baseCost, int _baseCastingTime, int _baseRecharge, List<string> _skillTypeTags, Map<string, PowerValue> _powerValues,
			List<string> _purposes, List<CombatAnimationNode> _combatAnimationNodes) {
			uniqueID = _uniqueID;
			displayName = _displayName;
			baseCost = _baseCost;
			baseCastingTime = _baseCastingTime;
			baseRecharge = _baseRecharge;
			skillTypeTags = _skillTypeTags;
			powerValues = _powerValues;
			purposes = _purposes;
			imageSource = _imageSource;
			combatAnimationNodes = _combatAnimationNodes;
		}

		wstring getName(string language) {
			return strings[language]["Skill Names"][uniqueID];
		}
		wstring getBaseDescription(string language) {
			return strings[language]["Skill Descriptions"][uniqueID];
		}
		string uniqueID;
		string displayName;
		int imageSource;
		int baseCost;
		int baseCastingTime;
		int baseRecharge;
		List<string> skillTypeTags;
		Map<string, PowerValue> powerValues;
		List<string> purposes; // used by the AI to decide which skill to use
		List<CombatAnimationNode> combatAnimationNodes;
	};
	class SkillBar {
	public:
		SkillBar() {}
		const int limit = 6;
		List<Skill> skills; // some enemies can have illegal / impossible skillbars like more than 4 skills 
	};
	class Combatant {
	public:
		Combatant() {}
		Combatant(string _uniqueID, string _displayName, Map<string, int> _intData, Map<string, Map<string, string>> _data) {
			defaultStats["LIFE"] = PowerValue("LIFE", 100, -999, 9999, true, list<string>({ "VITALITY", "LIFEBOOST" }));
			defaultStats["ENERGY"] = PowerValue("ENERGY", 30, -999, 9999, true, list<string>({ "PIETY", "ENERGYBOOST" }));
			defaultStats["ENERGYREGEN"] = PowerValue("ENERGYREGEN", 3, 0, 9999, true, list<string>({ "PIETY", "ENERGYREGENBOOST" }));
			defaultStats["SPEED"] = PowerValue("SPEED", 10, 0, 9999, true, list<string>({ "AGILITY", "SPEEDBOOST" }));
			uniqueID = _uniqueID;
			displayName = _displayName;
			data = _data;
			intData = _intData;
		}
		wstring getPrintout(string language, Combat & combat) {
			wstring result = L"";
			result += StringToWString(displayName) + L"\n\n\n";

			result += strings[language]["Skill Tree Names"][data["equippedSkillTreeNames"]["1"]] + L", ";
			result += strings[language]["Skill Tree Names"][data["equippedSkillTreeNames"]["2"]];
			result += L"\n";

			for (auto attribute : combat.AttributesInOrder.internalList) {
				wstring toAdd = strings[language]["Attribute Names"][attribute];
				wstring gap = getGap(toAdd);
				result += toAdd + L":" + gap + L"②" + to_wstring(intData[attribute]) + L" \n";
			}
			
			result += L"\n";

			for (auto stat : combat.statsInOrder.internalList) {
				wstring toAdd = strings[language]["Stat Names"][stat];
				wstring gap = getGap(toAdd);
				result += toAdd + L":" + gap + L"②" + defaultStats[stat].getResultantValueAsString(*&combat, intData) + L" \n";
			}
			result += strings[language]["GUI"]["PHYSICALPOWER"] + to_wstring(getPhysicalPowerAsPercentage(*&combat)) + L"% \n";
			result += strings[language]["GUI"]["MAGICALPOWER"] + to_wstring(getMagicalPowerAsPercentage(*&combat)) + L"% \n";
			return result;
		}
		wstring getGap(wstring toAdd) { 
			wstring gap = L"\t";
			if (toAdd.size() < 10) {
				gap += gap;
			}
			return gap;
		}
		wstring getPointsRemainingPrintout(string language, Combat & combat) {
			wstring result = L"";
			int pointsLeft = saveContainer.getRemainingPoints(uniqueID);
			if (pointsLeft > 1) {
				result = L"②" + to_wstring(pointsLeft) + strings[language]["GUI"]["XPOINTSLEFT"];
			}
			if (pointsLeft == 1) {
				result = L"②" + to_wstring(pointsLeft) + strings[language]["GUI"]["1POINTLEFT"];
			}
			return result;
		}
		int getPhysicalPowerAsPercentage(Combat & combat) {
			// for printout
			float result = PowerValue("PHYSICAL", 100, 0, 9999, true, list<string>({ "STRENGTH"})).getResultantValue(*&combat, intData);
			return round(result);
		}
		int getMagicalPowerAsPercentage(Combat& combat) {
			// for printout
			float result = PowerValue("MAGICAL", 100, 0, 9999, true, list<string>({ "INTELLIGENCE" })).getResultantValue(*&combat, intData);
			return round(result);
		}
		
		string uniqueID;
		string displayName;
		List<Skill> knownSkills;
		SkillBar currentSkills;
		Map<string, Map<string, string>> data;
		Map<string, int> intData;
		Map<string, PowerValue> defaultStats;
		Map<string, PowerValue> combatStats; // stats that change during combat
	};

	Combatant loadPartyMemberAsCombatant(string name) {
		Map<string, string> equippedSkillNames; equippedSkillNames.internalMap = saveContainer.current.equippedSkills[name];
		Map<string, string> equippedSkillTreeNames; equippedSkillTreeNames.internalMap = saveContainer.current.equippedSkillTrees[name];
		Map<string, string> equipment; equipment.internalMap = saveContainer.current.equippedItems[name];
		Map<string, int> attributeInvestments; attributeInvestments.internalMap = saveContainer.current.attributeInvestments[name];
		Map<string, Map<string, string>> data;
		data["equippedSkillNames"] = equippedSkillNames;
		data["equipment"] = equipment;
		data["equippedSkillTreeNames"] = equippedSkillTreeNames;
		return Combatant(name, name, attributeInvestments, data);
	}

	Map<string, Skill> skillDefinitions;
	Map<string, float> influenceLookups;
	List<string> AttributesInOrder;
	List<string> statsInOrder;
	Map<string, Map<string, int>> defaultAttInvestments;
};
Combat combat;

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
				pair<float, float> position = { stof(data["x"]), stof(data["y"]) };
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
			if (type == "MANAGEAUDIOSWAP") {
				string previousMap = explorer.currentMap.name;
				string targetMap = data["targetMap"];
				bool unloadAudio = data.hasKey("unloadAudio");
				
				List<string> existingSongs = explorer.currentMap.getSongNames();
				List<string> upcomingSongs = explorer.maps[targetMap].getSongNames();
				for (auto song : existingSongs.internalList) {
					string songName = split(song, " ").at(0);
					if (!upcomingSongs.contains(songName)) {
						audio.fadeOutAndStopThis(stoi(songName), 3);
						if (unloadAudio) {
							audio.unloadThisAudio(stoi(songName));
						}
					}
				}

				for (auto song : upcomingSongs.internalList) {
					string songName = split(song, " ").at(0);
					string volumeName = split(song, " ").at(1);
					float volume = audio.volumes[volumeName];
					if (!existingSongs.contains(songName)) {
						if (!audio.isThisAudioLoaded(stoi(songName))) {
							audio.loadAudio(stoi(songName));
						}
						audio.playSound(stoi(songName), volume, true, true);
					}
				}
				return true;
			}
			if (type == "SHOWMAPLOADINGSCREEN") {
				string mapName = data["targetMap"];
				string uniqueID = "LoadingScreen";
				string loadingScreenSource = explorer.maps[mapName].data["LoadingScreenImage"];
				if (!graphics.doesThisImageAlreadyExist(uniqueID)) {
					Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", loadingScreenSource),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "0.0"),
						pair<string, string>("layer",to_string(imageLookup.layerDefaults["LOADINGSCREEN"])),
						pair<string, string>("scale", "1.0"),
						pair<string, string>("uniqueID", uniqueID), }))).run(*&gameEngine);
					return false;
				}
				if (!CLOCK.hasEnoughTimePassed("loadingscreenmapfade", 10)) {
					return false;
				}
				Graphics::Image* theImage = graphics.accessImageViaUniqueID(uniqueID);
				theImage->opacity = TChange(theImage->opacity,0.1f,0.0f,1.0f);
				if (theImage->opacity == 1.0f) {
					return true;
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
				if (data.hasKey("direct")) {
					audio.loadAudio(stoi(data["audio"]));
					audio.playSound(stoi(data["audio"]), audio.volumes["SFXVolume"], false, false);
				}
				else {
					audio.playRandomSFXFromThisCollection(data["audio"], audio.volumes["SFXVolume"]);
				}
				
				return true;
			}
			if (type == "LOADMAP") {
				string mapName = data["targetMap"];
				string explorerName = saveContainer.getCurrentMainCharacter();
				explorer.loadMap(mapName);
				if (data.hasKey("putPlayerHere")) {
					explorer.playerOnMap.position = { stoi(data["x"]), stoi(data["y"]) };
				}
				string direction = "STAND_FRONT";
				if (data.hasKey("direction")) {
					direction = data["direction"];
				}
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
					pair<string, string>("sources", imageLookup.getSequenceAsString(explorerName, direction)),
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
					pair<string, string>("sources", imageLookup.getSequenceAsString("Shadow " + explorerName, direction)),
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
					string opacity = x.opacity;
					if (!x.visible) {
						opacity = "0.0";
					}
					Event("Load" + x.name, "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", x.imageSources),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", opacity),
						pair<string, string>("layer", to_string(x.layer)),
						pair<string, string>("animated", x.animated),
						pair<string, string>("animation_speed", to_string(x.animationSpeed)),
						pair<string, string>("styles", "LOOP"),
						pair<string, string>("uniqueID", x.name), }))).run(*&gameEngine);
				}
				Event("", "MAPMOVE", {}).run(*&gameEngine);
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
				if (graphics.doesThisTextAlreadyExist(explorer.mapPopupTextID) and data.getKeys().contains("copy")) {
					int layer = imageLookup.layerDefaults["TEXTONMAP"];
					graphics.TextMap[layer][explorer.mapPopupTextID].positionAsPercentage = imagePositions[data["copy"] + "_text"];
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
				List<string> mp = split(data["message"], "_");
				wstring message = wstring(data["message"].begin(), data["message"].end());
				if (data["direct"] != "1") {
					message = strings[mp.at(0)][mp.at(1)][mp.at(2)];
				}
				if (data["getStringFromCombatant"] == "1") {
					message = combat.loadPartyMemberAsCombatant(data["message"]).getPrintout(gameEngine.language,*&combat);
				}
				if (data["getStringFromCombatant"] == "2") {
					message = combat.loadPartyMemberAsCombatant(data["message"]).getPointsRemainingPrintout(gameEngine.language, *&combat);
				}
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
				if (!graphics.doesThisTextAlreadyExist(uniqueID)) {
					graphics.addText(Graphics::Text(message, format, position, anchorStyle, size, colour, shadowColour, uniqueID), layer);
					if (animated) {
						if (styles.contains("TYPEWRITER")) {
							graphics.TextMap[layer].internalMap[uniqueID].startTypewriter();
						}
					}
				}
				else {
					if (!animated) {
						if (graphics.TextMap[layer][uniqueID].message != message) {
							graphics.TextMap[layer][uniqueID].message = message;
						}
					}
					if (animated and styles.contains("TYPEWRITER")) {
						if (graphics.TextMap[layer][uniqueID].fullMessage != message) {
							graphics.TextMap[layer][uniqueID].message = message;
							graphics.TextMap[layer][uniqueID].startTypewriter();
						}
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
									audio.playRandomSFXFromThisCollection("PARCHMENT", audio.volumes["SFXVolume"]/2);
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
				int layer = imageLookup.layerDefaults["DEBUGUSERINPUT"];
				if (!graphics.doesThisTextAlreadyExist(uniqueID)) {
					graphics.addText(Graphics::Text(L"", "Centaur_25", { 0,0 }, "TOPLEFT", { 100,100 }, graphics.Colours["WHITE"], graphics.Colours["BLACK"], uniqueID), layer);
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
				if (!graphics.doesThisTextAlreadyExist(uniqueID)) {
					graphics.addText(Graphics::Text(L"", "Centaur_25", { 10,90 }, "TOPLEFT", { 100,100 }, graphics.Colours["BLACK"], graphics.Colours["BLACK"], uniqueID), layer);
				}
				string clickAndDragMessage = "Images Hovered Over: ";
				graphics.TextMap[layer].internalMap[uniqueID].message = StringToWString(clickAndDragMessage);
				Event("", "DEBUGUSERINPUT", {}).run(*&gameEngine);
			}
			if (type == "CLICKANDDRAG") {				
				List<string> draggable = split(data["objects"], ",");
				bool YLocked = data["YLock"] == "1";
				float XMin = 0;
				float XMax = 100;
				if (data.hasKey("XMin")) {
					XMin = stof(data["XMin"]);
				}
				if (data.hasKey("XMax")) {
					XMax = stof(data["XMax"]);
				}
				if (controller.mouseInstructionsInOrder.contains("LButtonDown")) {
					for (Graphics::Image* image : graphics.beingDragged.internalList) {
						graphics.bumpLayer(image, -1);
					}
					graphics.beingDragged.clear();
					graphics.recentlyFinishedBeingDragged.clear();
					pair<float, float> click = controller.mouseClickPosition;
					for (auto const& ID : draggable.internalList) {
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(ID);
						if (theImage == NULL) { continue; }
						if (theImage->hasThisBeenClickedOn(*&graphics, click)) {
							graphics.beingDragged.push_back(theImage);
							}
						}
					for (Graphics::Image * image : graphics.beingDragged.internalList) {
						graphics.bumpLayer(image, 1);
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
				if (controller.mouseInstructionsInOrder.contains("LButtonUp")) {
					// call upon drag and drop logic depending on the event
					for (Graphics::Image* image : graphics.beingDragged.internalList) {
						graphics.bumpLayer(image, -1);
					}
					for (Graphics::Image * image : graphics.beingDragged.internalList) {
						graphics.recentlyFinishedBeingDragged.push_back(image->unique_ID);
					}
					graphics.recentlyFinishedBeingDragged;
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
				string theObjects;
				List<string> party = saveContainer.current.party;
				for (int x = 0; x < min(party.size(), saveContainer.partyLimit); x++) {
					theObjects += party.at(x) + "_CARD,";
				}
				Event("ClickAndDrag", "CLICKANDDRAG", Map<string, string>({
					pair<string, string>("objects", theObjects),
					})).run(*&gameEngine);
				if (!graphics.recentlyFinishedBeingDragged.empty()) {
					List<Graphics::Image*> images;
					for (int x = 0; x < min(party.size(), saveContainer.partyLimit); x++) {
						if (!graphics.recentlyFinishedBeingDragged.contains(party.at(x) + "_CARD")) {
							images.push_back(graphics.accessImageViaUniqueID(party.at(x) + "_CARD"));
						}
					}
					for (Graphics::Image * image : images.internalList) {
						if (image->hasThisBeenClickedOn(*&graphics, controller.mouseUnclickPosition)) {
							string lhs = split(graphics.recentlyFinishedBeingDragged.front(), "_").at(0);
							string rhs = split(image->unique_ID, "_").at(0);
							string previousLeader = saveContainer.getCurrentMainCharacter();
							saveContainer.rearrangeTheseCharactersInParty(lhs, rhs);
							string nextLeader = saveContainer.getCurrentMainCharacter();
							Event("PrintoutPlayer", "SWAPEXPLORERS", List<pair<string, string>>({
							pair<string, string>("previous", previousLeader),
							pair<string, string>("next", nextLeader),
								})).run(*&gameEngine);
						}
					}
					List<pair<float, float>> positions = Menu::getPlayerCardPositions();
					List<string> party = saveContainer.current.party;
					for (int x = 0; x < min(party.size(), saveContainer.partyLimit); x++) {
						string who = party.at(x);
						graphics.accessImageViaUniqueID(who + "_CARD")->positionAsPercentage = positions.at(x);
						Event("TearDown", "TEARDOWNPLAYERREADOUT", pair<string, string>("who", who)).run(*&gameEngine);
						Event("PrintoutPlayer", "SETUPPLAYERREADOUT", List<pair<string, string>>({
							pair<string, string>("who", party.at(x)),
							pair<string, string>("x", to_string(positions.at(x).first - 8)),
							pair<string, string>("y", to_string(positions.at(x).second + 25)),
							})).run(*&gameEngine);
					}
				}
				return true;
			}
			if (type == "DEBUGWALKING") {
				string textUniqueID = "DEBUGWALKING";
				int layer = 10;
				if (!graphics.doesThisTextAlreadyExist(textUniqueID)) {
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
				//saveContainer.current.party = {data["uniqueID"]};
				return true;
			}
			if (type == "EXPLORE") {
				//Event("", "DEBUGUSERINPUT", {}).run(*&gameEngine);
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
				bool stopExploringChangeArea = false;
				bool popUpTextNeedsToBeDrawn = false;
				bool mapPopUpTextExists = false;
				string whichCutscene = "";
				string currentMap = explorer.currentMap.name;
				string targetMap = "";
				pair<float, float> futurePlayerPosition = {0,0};
				string futurePlayerDirection = "FRONT";
				Map<string, string> walkableDataToMoveOn; // send this to function that deals with next step
				if (moving and CLOCK.hasEnoughTimePassed("EXPLORE",exploreAnimationSpeeds["MOVE"])) {
					explorer.tryToMovePlayer(newDirection);
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
				// user has walked in range of an interactible object
				bool userInput = controller.haveOneOfTheseBeenPressed(VK_SPACE) and CLOCK.hasEnoughTimePassed("DialogueEnded", 1000);
				for (auto walkable : explorer.getObjectsInRange().internalList) {
					if (walkable.data.getKeys().contains("message")) {
						popUpTextNeedsToBeDrawn = true;
					}
					if (walkable.data.getKeys().contains("cutscene") and userInput) {
						whichCutscene = walkable.data["cutscene"];
						stopExploringStartCutscene = true;
						popUpTextNeedsToBeDrawn = false;
					}
					if (walkable.data.getKeys().contains("areaTransition") and userInput) {
						currentMap = explorer.currentMap.name;
						targetMap = walkable.data["areaTransition"];
						futurePlayerPosition = { stof(walkable.data["playerPosX"]), stof(walkable.data["playerPosY"]) };
						futurePlayerDirection = "BACK";
						walkableDataToMoveOn = walkable.data;
						stopExploringChangeArea = true;
					}
				}

				if (!popUpTextNeedsToBeDrawn and mapPopUpTextExists) {
					Event("TearDownPopUpText", "TEARDOWNTEXT", Map<string, string>(pair<string, string>{"uniqueID", explorer.mapPopupTextID})).run(*&gameEngine);
				}
				
				if (stopExploringStartCutscene) {
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
			if (type == "DIALOGUE") {
				string cutscene = data["cutscene"];
				string speakerID = data["speaker"];
				string line = name;
				string direct = data["direct"]; // 1 = use string here 0 = get string from Strings.h
				if (speakerID == "PLAYER") {
					speakerID = saveContainer.getCurrentMainCharacter();
				}
				if (direct != "1") {
					line = gameEngine.language + "_" + cutscene + "_" + line + " " + speakerID;
				}
				string speakerImageID = imageLookup.getSequenceAsString(speakerID, "SPEAKER");
				if (!graphics.doesThisImageAlreadyExist("TEXTBOX")) {
					Event("Load TextBox", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("sources", to_string(TEXTBOX)),
				pair<string, string>("x", "50"),
				pair<string, string>("y", "86"),
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
					pair<string, string>("message",speakerID),
					pair<string, string>("direct", "1"),
					pair<string, string>("format", "LightText_40"),
					pair<string, string>("anchorStyle", "TOPLEFT"),
					pair<string, string>("x", "18"),
					pair<string, string>("y", "72"),
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
				pair<string, string>("layer", to_string(imageLookup.layerDefaults["UI"] + 3)),
				pair<string, string>("uniqueID", "speakerDialogueText"),
				pair<string, string>("animated", "TRUE"),
				pair<string, string>("styles", "TYPEWRITER,PARCHMENT"),
					}))).run(*&gameEngine);
				return (finishedWriting and controller.haveOneOfTheseBeenPressed({ VK_SPACE }));
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
			if (type == "LOADMENU") {
				controller.mouseClickPosition = { -1,-1 };
				CLOCK.startClock("MENUINPUTDELAY");
				string menuName = data["uniqueID"];
				Menu menu= gameEngine.storedMenus[menuName];
				for (auto button : menu.buttons.internalList) {
					if (!button.visible) { continue; }
					if (button.sources.size() > 0) { // no sources = no images needed
						Event("LoadThisButtonImage", "LOADIMAGE", Map<string, string>({
							pair<string, string>("sources", button.getSources()),
							pair<string, string>("x", to_string(button.position.first)),
							pair<string, string>("y", to_string(button.position.second)),
							pair<string, string>("anchor", button.anchorStyle),
							pair<string, string>("opacity", "1.0"),
							pair<string, string>("layer",to_string(imageLookup.layerDefaults["BUTTONS"])),
							pair<string, string>("scale", "1.0"),
							pair<string, string>("uniqueID", button.imageID),
							})).run(*&gameEngine);
					}
					Event("LoadThisButtonText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",gameEngine.language + "_" + button.buttonContent),
						pair<string, string>("format", "Centaur_25"),
						pair<string, string>("anchorStyle", button.anchorStyle),
						pair<string, string>("x", to_string(button.position.first)),
						pair<string, string>("y", to_string(button.position.second)),
						pair<string, string>("w", to_string(button.width)),
						pair<string, string>("h",  to_string(button.height)),
						pair<string, string>("colour", "WHITE"),
						pair<string, string>("shadowColour", "DARKBROWN"),
						pair<string, string>("layer",  to_string(imageLookup.layerDefaults["BUTTONS"])),
						pair<string, string>("uniqueID", button.textID),
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
							pair<string, string>("y", to_string(audioManagementStartPos.second-1.5)),
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
				return true;
			}
			if (type == "SETUPPLAYERREADOUT") {
				string who = data["who"];
				pair<float, float> Where = { stof(data["x"]), stof(data["y"]) };
				Event("SetUpPlayerReadoutText", "DRAWTEXT", List<pair<string, string>>({
						pair<string, string>("message", who),
						pair<string, string>("getStringFromCombatant", "1"),
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
						pair<string, string>("getStringFromCombatant", "2"),
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
				Event("TearDown", "TEARDOWNTEXT", pair<string, string>("uniqueID", who + "_PRINTOUTTEXT")).run(*&gameEngine);
				Event("TearDown", "TEARDOWNTEXT", pair<string, string>("uniqueID", who + "_POINTSLEFTTEXT")).run(*&gameEngine);
				for (auto attribute : combat.AttributesInOrder.internalList) {
					Event("TeardownImage", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", who + "_" + attribute + "_PLUSBUTTONIMAGE"),}))).run(*&gameEngine);
					Event("TeardownImage", "TEARDOWNIMAGE", Map<string, string>(List<pair<string, string>>({
				pair<string, string>("uniqueID", who + "_" + attribute + "_MINUSBUTTONIMAGE"), }))).run(*&gameEngine);
				}
				return true;
			}
			if (type == "HANDLEMENU") {
				//Event("", "DEBUGUSERINPUT", {}).run(*&gameEngine);
				string whichMenu = data["uniqueID"];
				List<Menu::Button> clickables = gameEngine.storedMenus[whichMenu].getClickableButtons();
				if (gameEngine.storedMenus[whichMenu].data.getKeys().contains("PARTYSHUFFLE")) {
					Event("HandleRearrange", "HANDLEPARTYREARRANGECLICKANDDRAG", {}).run(*&gameEngine);
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
				bool anythingHovered = false;
				for (auto clickable : clickables.internalList) {
					if (graphics.doesThisImageAlreadyExist(clickable.imageID)) {
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(clickable.imageID);
						bool hoveredOver = theImage->hasThisBeenClickedOn(*&graphics, controller.mouseMovePosition);
						bool clickedOn = theImage->hasThisBeenClickedOn(*&graphics, controller.mouseClickPosition);
						if (hoveredOver) {
							anythingHovered = true;
						}
						if (hoveredOver and clickedOn) {
							theImage->frame = 2;
							Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
									pair<string,string>("audio",to_string(clickable.audioClick)),
									pair<string,string>("direct","1"),
								})).run(*&gameEngine);
							Event("ButtonLogic", "HANDLEBUTTON", pair<string, string>("uniqueID", clickable.uniqueID)).run(*&gameEngine);
							controller.mouseClickPosition = { -1,-1 };
						}
						if (hoveredOver and !clickedOn) {
							theImage->frame = 1;
							string hoveredOverItem = clickable.imageID;
							if (hoveredOverItem != controller.latestMenuItemHovered) {
								Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
									pair<string,string>("audio",to_string(clickable.audioHover)),
									pair<string,string>("direct","1"),
									})).run(*&gameEngine);
								controller.latestMenuItemHovered = clickable.imageID;
							}
						}
						if (!hoveredOver) {
							theImage->frame = 0;
						}
					}
				}
				if (!anythingHovered) {
					controller.latestMenuItemHovered = "";
				}
				if (CLOCK.hasEnoughTimePassed("MENUINPUTDELAY", 100) and not controller.menuItemCooldown) {
					for (auto const& [key, value] : gameEngine.storedMenus[whichMenu].getKeyboardShortcutsForThisMenu().internalMap) {
						if (controller.hasThisBeenPressed(key)) {
							controller.menuItemCooldown = true;
							Event("ButtonLogic", "HANDLEBUTTON", pair<string, string>("uniqueID", value)).run(*&gameEngine);
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
			if (type == "TEARDOWNMENU") {
				graphics.beingDragged.clear();
				string menuName = data["uniqueID"];
				Menu menu = gameEngine.storedMenus[menuName];
				for (auto button : menu.buttons.internalList) {
					if (!button.visible or button.imageID == "") { continue; }
					Event("TearDown", "TEARDOWNIMAGE", pair<string,string>("uniqueID", button.imageID)).run(*&gameEngine);
					Event("TearDown", "TEARDOWNTEXT", pair<string, string>("uniqueID", button.textID)).run(*&gameEngine);
				}
				if (menu.data.hasKey("AUDIOMANAGEMENT")) {
					for (auto node : audio.volumes.getKeys().internalList) {
						Event("TearDown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", node + "_IMAGE")).run(*&gameEngine);
						Event("TearDown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", node + "_KNOB_IMAGE")).run(*&gameEngine);
						Event("TearDown", "TEARDOWNTEXT", pair<string, string>("uniqueID", node + "_TEXT")).run(*&gameEngine);
					}
				}
				if (menu.data.hasKey("PARTYSHUFFLE")) {
					List<string> party = saveContainer.current.party;
					for (auto const & member : party.internalList) {
						Event("TearDown", "TEARDOWNPLAYERREADOUT", pair<string, string>("who", member)).run(*&gameEngine);
						Event("TearDown", "TEARDOWNIMAGE", pair<string, string>("uniqueID", member + "_CARD")).run(*&gameEngine);
					}
				}
				return true;
			}
			if (type == "HANDLEBUTTON") {
				string buttonLogic = data["uniqueID"];
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
					wstring message1 = combat.loadPartyMemberAsCombatant(who).getPrintout(gameEngine.language, *&combat);
					wstring message2 = combat.loadPartyMemberAsCombatant(who).getPointsRemainingPrintout(gameEngine.language, *&combat);
					graphics.TextMap[imageLookup.layerDefaults["BUTTONS"]][who + "_PRINTOUTTEXT"].message = message1;
					graphics.TextMap[imageLookup.layerDefaults["BUTTONS"]][who + "_POINTSLEFTTEXT"].message = message2;
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
				if (buttonLogic == "QUITYES") {
					gameEngine.stateFlags["QUIT"] = "1";
					return true;
				}
				if (buttonLogic == "QUITNO") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "QUITCONFIRM")).run(*&gameEngine);
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EXPLOREPAUSE");
					return true;
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
					gameEngine.activeProcedure = gameEngine.makeLoadMenuProcedure("EXPLOREPAUSE");
					return true;
				}
			}
			if (type == "LOADSKILLBARHERE") {
				string who = data["who"];
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
				pair<string, string>("message","ENG_NAMES_Angela"),
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
				pair<string, string>("targetMap", "House1Inside1"),
			}))),
			Event("Load Map", "LOADMAP", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("targetMap", "House1Inside1"),
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
			"DEBUGMENU", Menu("DEBUGMENU", List<Menu::Button>({Menu::standardButton("DEBUGBUTTON1", "Example Button", {50,50}),}), {})),
		pair<string, Menu>(
			"EXPLOREPAUSE", Menu("EXPLOREPAUSE", List<Menu::Button>({
				Menu::standardButton("PARTYMANAGEMENT", "GUI_MANAGEPARTYBUTTON", {50, 25}),
				Menu::standardButton("RETURNTOEXPLORE", "GUI_RESUMEEXPLOREBUTTON", {50, 36}),
				Menu::standardButton("AUDIOSETTINGS", "GUI_AUDIOOPTIONS", {50, 47}),
				Menu::standardButton("QUITTODESKTOP", "GUI_QUITBUTTON", {50, 58}),
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
				Menu::standardButton("QUITNO", "GUI_CANCELBUTTON", {50, 50}),
				Menu::standardButton("QUITYES", "GUI_QUITBUTTON", {50, 60}),
				}),Map<string, string>({
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " QUITNO")}))),
		pair<string, Menu>(
			"PARTYMANAGEMENT", Menu("PARTYMANAGEMENT", List<Menu::Button>({
				Menu::standardButton("PARTYMANAGEHELP", "GUI_PARTYMANAGEHELP", {12, 80}),
				Menu::standardButton("FROMPARTYTOEXPLOREPAUSE", "GUI_FROMPARTYTOPAUSE", {12, 90}),
				}),Map<string, string>({
					pair<string, string>("PARTYSHUFFLE", "1"),
					pair<string, string>("ATTPOINTMANAGE", "1"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMPARTYTOEXPLOREPAUSE")}))),
	});
	Procedure makeDynamicCutsceneProcedure(string language, string cutsceneName, string player, string postProcedure) {
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
		events.push_back(Event("Explore", "EXPLORE", {}));
		return Procedure("AreaTransition", events);
	}
	Procedure makeLoadMenuProcedure(string whichMenu) {
		List<Event> events;
		events.push_back(Event("LoadAMenu", "LOADMENU", Map<string, string>({
			pair<string, string>("uniqueID", whichMenu),
			})));
		events.push_back(Event("HandleMenu", "HANDLEMENU", Map<string, string>({
			pair<string, string>("uniqueID", whichMenu),
			})));
		return Procedure("MenuProcedure", events);
	}
	List<Event> convertDynamicStringsToDialogue(string language, string cutsceneName, string player) {
		// dynamic as in, the line changes depending on who the player is
		// for example, player interacting with something on the map
		// other cutscenes have all hardcoded speakers
		List<Event> results;
		List<string> acceptedLines;
		Map<string, wstring> lines; lines.internalMap = strings[language][cutsceneName];
		for (auto const [key, val] : strings[language][cutsceneName]) {
			string thisLine = split(key, " ").at(0);
			if (acceptedLines.contains(thisLine)) { continue; }
			string thisSpeaker = split(key, " ").at(1);
			if (thisSpeaker.find("+") != -1) {
				// add something where the line itself must change according to the player
			}
			string speaker = thisSpeaker;
			if (lines.getKeys().contains(thisLine + " " + player)) {
				speaker = "PLAYER";
			}
			results.push_back(Event(thisLine, "DIALOGUE", { Map<string, string>(List<pair<string,string>>({
				pair<string, string>({"cutscene", cutsceneName}),
				pair<string, string>({"speaker", "PLAYER"}),
				})) }));
			acceptedLines.push_back(thisLine);
		}
		return results;
	}
	GameEngine() {}
	void setup() {
		string mode = Args.get("mode");
		if (mode == "DEBUG") {
			activeProcedure = storedProcedures["DEBUG3"];
		}
		else {
			throw runtime_error("Not Implemented yet");
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
	string language = "ENG";
};
GameEngine game;