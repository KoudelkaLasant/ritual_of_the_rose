#pragma once

#include "Graphics.h"

class Menu {
public:
	Menu() {}
	class Button {
	public:
		Button() {
			layer = imageLookup.layerDefaults["BUTTONS"];
		}
		Button(string _uniqueID, string _textID, string _buttonContent, string _imageID, List<int> _sources, int _audioClick, int _audioHover, pair<float, float> _position, bool _visible, bool _clickable, Map<string, string> _extras) {
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
			extras = _extras;
			layer = imageLookup.layerDefaults["BUTTONS"];
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
		Map<string, string> extras;
		string anchorStyle = "CENTRE";
		int audioClick;
		int audioHover;
		bool visible;
		bool clickable;
		float width = 100;
		float height = 0;
		int layer = 0;
	};
	Menu(string _uniqueID, List<Button> _buttons, Map<string, string> _data) {
		uniqueID = _uniqueID;
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
	List<Button> getButtonsToLoad() {
		List<Button> result = buttons;
		return result;
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
	static Button smallButton(string baseID, string buttonMessage, pair<float, float> position) {
		Button result;
		result.uniqueID = baseID;
		result.textID = baseID + "_TEXT";
		result.imageID = baseID + "_IMAGE";
		result.sources = { SMALLBUTTON, SMALLBUTTON_HOVERED, SMALLBUTTON_PRESSED };
		result.audioClick = BUTTON_CLICK_WAV;
		result.audioHover = BUTTON_HOVER_WAV;
		result.buttonContent = buttonMessage;
		result.position = position;
		result.visible = true;
		result.clickable = true;
		result.anchorStyle = "CENTRE";
		result.extras["format"] = "Centaur_13";
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
		if (size == "VERY_SMALL") {
			result.sources = List<int>({ BUTTON1_DEFAULT });
			result.width = 20;
			result.height = 10;
		}
		return result;
	}
	static Button MerchantTalkingBox(string baseID) {
		Button result = Menu::TextBox(baseID, "", "", {0,0});
		result.sources = List<int>({ TEXT_BOX_NARROW });
		result.width = 100;
		result.height = 20;
		result.extras["textOffsetX"] = "2";
		result.extras["textOffsetY"] = "2";
		result.extras["textOffsetX2"] = "4";
		result.extras["TYPEWRITER"] = "1";
		result.extras["direct"] = "1";
		result.extras["animated"] = "TRUE";
		result.extras["colour"] = "BLACK";
		result.anchorStyle = "TOPLEFT";
		result.extras["layer"] = to_string(imageLookup.layerDefaults["BUTTONS"] - 1);
		return result;
	}
	static Button MoneyPrintout(string baseID, pair<float, float> position) {
		Button result = TextBox(baseID, "", "", position);
		result.sources = List<int>({ MONEY });
		result.extras["textOffsetX"] = "4";
		result.extras["textOffsetY"] = "0";
		result.extras["textOffsetX2"] = "0";
		result.extras["direct"] = "1";
		result.extras["getMessageFromMoney"] = "1";
		return result;
	}
	static const List<pair<float, float>> getPlayerCardPositions() {
		// for party rearrange
		return List<pair<float, float>>({pair<float, float>(35,25),pair<float, float>(53,25),pair<float, float>(71,25),pair<float, float>(89,25)});
	}
	static const Map<int, List<pair<float, float>>> getPlayerCardReformGridPositions(float xOffset, float yOffset) {
		Map<int, List<pair<float, float>>> results;

		List<pair<float, float>> firstRow({pair<float, float>(xOffset,yOffset),pair<float, float>(5 + xOffset,yOffset),pair<float, float>(10 + xOffset,yOffset),
			pair<float, float>(15 + xOffset,yOffset), pair<float, float>(20 + xOffset,yOffset) });
		for (int x = 1; x < 6; x++) {
			List<pair<float, float>> currentRow = firstRow;
			for (int y = 0; y < currentRow.size(); y++) {
				currentRow.at(y).second *= x;
			}
			results[x-1] = currentRow;
		}
		return results;
	}
	static const Map<string, string> getSkillGridPositions() {
		Map<string, string> result = Map<string, string>({pair<string, string>("1","35"),pair<string, string>("2","65"),});
		return result;
	}
	static const Map<string, RECT> getSkillBarEditPositions() {
		Map<string, RECT> result;
		float top = 560;
		float bottom = 660;
		//result["WHOLESKILLBAR"] = graphics.makeRect(top, 398, bottom,1098);
		result["SKILLSLOT1"] = graphics.makeRect(top, 498, bottom, 598);
		result["SKILLSLOT2"] = graphics.makeRect(top, 598, bottom, 698);
		result["SKILLSLOT3"] = graphics.makeRect(top, 698, bottom, 798);
		result["SKILLSLOT4"] = graphics.makeRect(top, 798, bottom, 898);
		result["SKILLSLOT5"] = graphics.makeRect(top, 898, bottom, 998);

		return result;
	}
	static const Map<string, string> getLocAndScaleOfSkillBarEdit() {
		Map<string, string> result;
		result["scale"] = "1.0";
		result["x"] = "60";
		result["y"] = "85";
		return result;
	}
	static const Map<string, RECT> getPartyShufflePositions() {
		Map<string, RECT> result;
		float top = 10;
		float bottom = 350;
		float left = 325;
		float gap = 200;
		result["PARTYSLOT1"] = graphics.makeRect(top, left, bottom, left+gap);
		result["PARTYSLOT2"] = graphics.makeRect(top, (left + gap+1), bottom, left+gap*2);
		result["PARTYSLOT3"] = graphics.makeRect(top, left + (gap*2) + 1, bottom, left +(gap*3));
		result["PARTYSLOT4"] = graphics.makeRect(top, left + (gap*3)+1, bottom, left+(gap*4));

		return result;
	}
	static const Map<int, pair<float, float>> getItemGridPositions() {
		Map<int, pair<float, float>> results;
		int rowLength = 4;
		int columnLength = 10;
		pair<float, float> topLeftCorner = { 40,35 };
		pair<float, float> gap = {15,5};
		int cell = 0;
		for (int x = 0; x < columnLength; x++) {
			for (int y = 0; y < rowLength; y++) {
				results[cell] = { topLeftCorner.first + (gap.first * y), topLeftCorner.second + (gap.second * x) };
				cell += 1;
			}
		}
		return results;
	}
	static const Map<int, pair<float, float>> getMerchantGridPositions() {
		Map<int, pair<float, float>> results;
		int rowLength = 6;
		int columnLength = 10;
		pair<float, float> topLeftCorner = { 10,30 };
		pair<float, float> gap = { 15,5 };
		int cell = 0;
		for (int x = 0; x < columnLength; x++) {
			for (int y = 0; y < rowLength; y++) {
				results[cell] = { topLeftCorner.first + (gap.first * y), topLeftCorner.second + (gap.second * x) };
				cell += 1;
			}
		}
		return results;
	}
	static const List<Button> getDefaultButtonsForEquipmentSelect() {
		List<Button> result = List<Button>({ 
			Menu::TextBox("TEXTBOX1", "GUI_SELECTCHARACTER", "VERY_SMALL", { 15, 7 }),
			Menu::standardButton("EQUIPMENTTYPE", "GUI_Weapon", { 45, 7 }),
			Menu::standardButton("FROMEQUIPMANAGETOPARTYMANAGE", "GUI_FROMPARTYTOPAUSE", { 12, 90 }) });
		return result;
	}
	static const List<Button> getDefaultButtonsForMerchant() {
		List<Button> result = List<Button>({
			Menu::standardButton("MERCHANTTOEXPLORE", "GUI_MERCHANTTOEXPLORE", {12, 90}),
			Menu::standardButton("MERCHANTBUY", "GUI_MERCHANTBUY", {25, 20}),
			Menu::standardButton("MERCHANTSELL", "GUI_MERCHANTSELL", {50, 20}),
			Menu::standardButton("MERCHANTBUYBACK", "GUI_MERCHANTBUYBACK", {75, 20}),
			Menu::MerchantTalkingBox("MERCHANTDIALOGUE"),
			Menu::MoneyPrintout("MONEYSTATUS", {8,22}),
			});
		return result;
	}
	static const List<Button> getDefaultButtonsForConfirmTrade() {
		List<Button> result = List<Button>({
			Menu::TextBox("TradeConfirm", "", "SMALL", {50,50}),
			Menu::standardButton("TradeConfirmYes", "GUI_Yes", {65, 70}),
			Menu::standardButton("TradeConfirmNo", "GUI_No", {35, 70}),
			});
		result.at(0).extras["direct"] = "1";
		return result;
	}
	static const List<Button> getDefaultButtonsForDenyTrade(string reason) {
		List<Button> result = List<Button>({
			Menu::TextBox("TradeDeny", reason, "SMALL", {50,50}),
			Menu::standardButton("TradeConfirmNo", "GUI_RETURN", {50, 70}),
			});
		return result;
	}
	static const List<Button> getDefaultButtonsForFindingItems() {
		List<Button> result = List<Button>({
			Menu::TextBox("YouFoundTextBox", "GUI_You found 1", "SMALL", {50,50}),
			Menu::standardButton("AcceptItem", "GUI_RETURN", {50, 70}),
			});
		return result;
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
		influenceLookups["VITALITY"] = 0.09;
		influenceLookups["PIETY"] = 0.15;
		influenceLookups["LUCK"] = 0.1;
		influenceLookups["SPEED"] = 0.1;
		influenceLookups["AGILITY"] = 0.15;
		// skill tags are used by more than 2 skill trees so give lower bonus
		influenceLookups["ELEMENTALBOOST"] = 0.04;

		// skill tags are shared with 2 skill trees so give slightly lower bonus
		influenceLookups["HOLYBOOST"] = 0.05;
		influenceLookups["UNHOLYBOOST"] = 0.05;

		// less skill trees use this tag so give slightly higher bonus
		influenceLookups["SHADOWBOOST"] = 0.06;
		influenceLookups["FIREBOOST"] = 0.06;
		influenceLookups["WATERBOOST"] = 0.06;
		influenceLookups["WAYFARINGBOOST"] = 0.06;
		influenceLookups["WEATHERBOOST"] = 0.06;
		influenceLookups["BLOODBOOST"] = 0.06;
		influenceLookups["Dual Weapon MasteryBOOST"] = 0.06;
		influenceLookups["1H Weapon MasteryBOOST"] = 0.06;

		// armour boosts
		influenceLookups["PHYSICALARMOUR"] = 0.05;
		influenceLookups["ELECTRICARMOUR"] = 0.05;

		AttributesInOrder = {"VITALITY","PIETY","STRENGTH", "INTELLIGENCE", "AGILITY","LUCK"};
		statsInOrder = {"LIFE","ENERGY","ENERGYREGEN", "SPEED"};
		defaultAttInvestments["Angela Fleuret"] = {
			{"STRENGTH" , 2},
			{"INTELLIGENCE" , 3},
			{"VITALITY", 2},
			{"PIETY", 12},
			{"AGILITY", 0},
			{"LUCK", 1}
		};
		defaultAttInvestments["Tianshun Song"] = {
			{"STRENGTH" , 0},
			{"INTELLIGENCE" , 8},
			{"VITALITY", 0},
			{"PIETY", 8},
			{"AGILITY", 4},
			{"LUCK", 0}
		};
		defaultAttInvestments["Olyver Sumner"] = {
			{"STRENGTH" , 1},
			{"INTELLIGENCE" , 9},
			{"VITALITY", 0},
			{"PIETY", 5},
			{"AGILITY", 3},
			{"LUCK", 2}
		};
		defaultAttInvestments["Hernando Pizarro"] = {
			{"STRENGTH" , 10},
			{"INTELLIGENCE" , 0},
			{"VITALITY", 5},
			{"PIETY", 0},
			{"AGILITY", 3},
			{"LUCK", 2}
		};
		defaultAttInvestments["Gihat al-Din Jaqmaq"] = {
			{"STRENGTH" , 5},
			{"INTELLIGENCE" , 0},
			{"VITALITY", 2},
			{"PIETY", 4},
			{"AGILITY", 5},
			{"LUCK", 4}
		};
		defaultSkillTreeChoices["Angela Fleuret"] = {
			{"1", "Cleromancy"},
			{"2", "Hagiomancy"},
		};
		defaultSkillTreeChoices["Tianshun Song"] = {
			{"1", "Sangromancy"},
			{"2", "Necromancy"},
		};
		defaultSkillTreeChoices["Olyver Sumner"] = {
			{"1", "Meteomancy"},
			{"2", "Electromancy"},
		};
		defaultSkillTreeChoices["Gihat al-Din Jaqmaq"] = {
			{"1", "Dual Weapon Mastery"},
			{"2", "Umbromancy"},
		};
		defaultSkillTreeChoices["Hernando Pizarro"] = {
			{"1", "1H Weapon Mastery"},
			{"2", "Wayfaring"},
		};
		defaultSkillChoices["Angela Fleuret"] = {
			{"1", "Heal Wounds"},
			{"5", "Heavenstrike"}
		};
		defaultSkillChoices["Tianshun Song"] = {
			{"1", "Life Drain"},
			{"5", "Animate Skeleton Warrior"}
		};
		defaultSkillChoices["Olyver Sumner"] = {
			{"1", "Rainstorm"},
			{"5", "Plasma Pulse"}
		};
		defaultSkillChoices["Gihat al-Din Jaqmaq"] = {
			{"1", "Doublestrike"},
			{"5", "Shadow Spike"},
		};
		defaultSkillChoices["Hernando Pizarro"] = {
			{"1", "Fine Strike"},
			{"5", "Gentleman's Riposte"},
		};
		defaultEquipment["Angela Fleuret"] = {
			{"Weapon", "Withered Secespita"},
			{"Armour" , "Vatican Vestiments"},
			{"Accessory", "Cross of St Jeanne-Marie"}
		};
		defaultEquipment["Tianshun Song"] = {
			{"Weapon", "Tibetan Tie Bian"},
			{"Armour" , "Ming Theatre Costume"},
			{"Accessory", "Lunyu Page Fragment"}
		};
		defaultEquipment["Olyver Sumner"] = {
			{"Weapon", "Roger Bacon's Quill"},
			{"Armour" , "Insulating Gloves"},
			{"Accessory", "Theoricae Novae Planetarum"}
		};
		defaultEquipment["Gihat al-Din Jaqmaq"] = {
			{"Weapon", "Suero's Blade"},
			{"Armour" , "Martin's Cloak"},
			{"Accessory", "Matteo Carreri's Locket"}
		};
		defaultEquipment["Hernando Pizarro"] = {
			{"Weapon", "Blades of House JaqMaq"},
			{"Armour" , "Alhambran Tunic"},
			{"Accessory", "Mask of House JaqMaq"}
		};
		

		defineAllSkills();
		defineAllEquipment();
	}
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
		int getResultantValue(Combat & combat, Map<string, int> percentInfluences, Map<string, int> flatInfluences) {
			float result = baseValue;
			float multiplier = 1.0;
			if (!goesUpwards) {
				multiplier*=-1;
			}
			for (auto tag : percentInfluences.getKeys().internalList) {
				string tagName = split(tag, "_").at(0);
				if (influenceTags.contains(tagName)) {
					multiplier += (percentInfluences[tag] * combat.influenceLookups[tagName]);
				}
			}
			result *= multiplier;
			for (auto tag : flatInfluences.getKeys().internalList) {
				string tagName = split(tag, "_").at(0);
				if (influenceTags.contains(tagName)) {
					result += flatInfluences[tag];
				}	
			}
			if (baseValue > 0 and !goesUpwards) {
				result *= -1;
			}
			result = round(result);
			if (result < min) {
				result = min;
			}
			if (result > max) {
				result = max;
			}
			return result;
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
		Skill(string _uniqueID, string _displayName, string _skillTree, int _imageSource, int _baseCost, int _baseCastingTime, int _baseRecharge, List<string> _skillLogicNames, List<string> _skillTypeTags, Map<string, PowerValue> _powerValues, List<string> _purposes) {
			uniqueID = _uniqueID;
			displayName = _displayName;
			baseCost = _baseCost;
			baseCastingTime = _baseCastingTime;
			baseRecharge = _baseRecharge;
			skillTypeTags = _skillTypeTags;
			powerValues = _powerValues;
			purposes = _purposes;
			imageSource = _imageSource;
			skillTree = _skillTree;
			skillLogicNames = _skillLogicNames;
		}

		bool isElite() {
			return skillTypeTags.contains("ELITE");
		}
		int getBorderSource() {
			int border = NORMALSKILLBORDER;
			if (isElite()) {
				border = ELITESKILLBORDER;
			}
			return border;
		}
		wstring getName(string language) {
			return strings[language]["Skill Names"][uniqueID];
		}
		wstring getBaseDescription(string language) {
			return strings[language]["Skill Descriptions"][uniqueID];
		}
		wstring getStartActionString(string language, string who) {
			wstring result = strings[language]["Skill Actions"]["Default_Started"];
			if (skillTypeTags.contains("SPELL")) {
				result = strings[language]["Skill Actions"]["Spell_Started"];
			}
			if (skillTypeTags.contains("ATTACK")) {
				result = strings[language]["Skill Actions"]["Attack_Started"];
			}
			result = WSReplace(result, L"$WHO$", StringToWString(who));
			result = WSReplace(result, L"$SKILLNAME$", StringToWString(displayName));
			return result;
		}
		string uniqueID;
		string displayName;
		string skillTree;
		int imageSource;
		int baseCost;
		int baseCastingTime;
		int baseRecharge;
		List<string> skillLogicNames;
		List<string> skillTypeTags;
		Map<string, PowerValue> powerValues;
		List<string> purposes; // used by the AI to decide which skill to use
	};
	class Equipment {
	public:
		class Effect {
		public:
			Effect() {}
			Effect(string _tag, float _influence, bool _goesUp, bool _isFlat) {
				tag = _tag;
				influence = _influence;
				goesUp = _goesUp;
				isFlat = _isFlat;
			}
			
			wstring getPrintout(string language, Combat& combat) {
				wstring result;
				if (tag.find("BOOST") != -1) {
					result = strings[language]["Item Effect Strings"]["XBOOST"];
					string typeName = SReplace(tag, "BOOST", "");
					wstring typeNameLower = strings[language]["Type Names"][typeName];
					result = WSReplace(result, L"$REPLACE1$", typeNameLower);
					int powerAsPercentage = combat.influenceLookups[tag] * (100 * influence);
					result = WSReplace(result, L"$REPLACE2$", to_wstring(powerAsPercentage));
					result += L"%.\n";
					return result;
				}
				if (tag.find("PLUS") != -1) {
					result = strings[language]["Item Effect Strings"]["ATTUPP"];
					string typeName = SReplace(tag, "PLUS", "");
					wstring typeNameLower = strings[language]["Other Stat Names"][typeName];
					result = WSReplace(result, L"$REPLACE1$", typeNameLower);
					result = WSReplace(result, L"$REPLACE2$", to_wstring(int(influence)));
					result += L".\n";
					return result;
				}
				if (tag.find("ARMOUR") != -1) {
					result = strings[language]["Item Effect Strings"]["ARMOUR"];
					string typeName = SReplace(tag, "ARMOUR", "");
					wstring typeNameLower = strings[language]["Type Names"][typeName];
					result = WSReplace(result, L"$REPLACE1$", typeNameLower);
					int powerAsPercentage = combat.influenceLookups[tag] * 100 * influence;
					result = WSReplace(result, L"$REPLACE2$", to_wstring(powerAsPercentage));
					result += L"%.\n";
					return result;
				}
				if (combat.AttributesInOrder.contains(tag)) {
					result = strings[language]["Item Effect Strings"]["ATTUPP"];
					wstring typeAsWS = strings[language]["Attribute Names"][tag];

					result = WSReplace(result, L"$REPLACE1$", typeAsWS);
					result = WSReplace(result, L"$REPLACE2$", to_wstring(int(influence)));
					result += L".\n";
					return result;
				}
				return strings[language]["Unique Item Strings"][tag];
			}

			string tag;
			float influence;
			bool goesUp;
			bool isFlat; // or a %
		};
		
		Equipment() {}
		Equipment(string _uniqueID, string _category, int _imageSource, List<Effect> _powers, string _textColour, int _price) {
			uniqueID = _uniqueID;
			category = _category;
			imageSource = _imageSource;
			powers = _powers;
			textColour = _textColour;
			price = _price;
		}
		
		wstring printout(string language, Combat& combat) {
			wstring colourToReplaceInEffectDescription = L"⑳";
			wstring result = L"";
			wstring itemName = strings[language]["Item Names"][uniqueID];
			wchar_t colourTag = graphics.colourTagLookupTable.getKeyAssociatedWithThisValue(textColour);
			wstring colourTagAsSymbol = wstring(1, colourTag);

			itemName = WSReplace(itemName, L" ", colourTagAsSymbol);

			result += colourTagAsSymbol + itemName + colourTagAsSymbol + L" \n";
			for (auto effect : powers.internalList) {
				result += L"✵" + WSReplace(effect.getPrintout(language, *&combat), colourToReplaceInEffectDescription, colourTagAsSymbol);
			}
			if (category == "Tome") {
				result += L"*" + strings[language]["Item Descriptions"][uniqueID];
			}
			return result;
		}

		wstring printoutOnOneLine(string language, Combat& combat) {
			wstring result = printout(language, *&combat);
			result = WSReplace(result, L"\n", L" ");
			return result;
		}
		wstring getColourForPrint() {
			wchar_t colourTag = graphics.colourTagLookupTable.getKeyAssociatedWithThisValue(textColour);
			wstring colourTagAsSymbol = wstring(1, colourTag);
			return colourTagAsSymbol;
		}

		string textColour;
		string uniqueID;
		string category;
		List<Effect> powers;
		int imageSource;
		int price;
	};
	class Combatant {
	public:
		Combatant() {}
		Combatant(string _uniqueID, string _displayName, Map<string, int> _intData, Map<string, Map<string, string>> _data) {
			defaultStats["LIFE"] = PowerValue("LIFE", 100, -999, 9999, true, list<string>({ "VITALITY" }));
			defaultStats["ENERGY"] = PowerValue("ENERGY", 30, -999, 9999, true, list<string>({ "PIETY"}));
			defaultStats["LIFEREGEN"] = PowerValue("LIFEREGEN", 0, 0, 9999, true, list<string>({ "LIFEREGENPLUS" }));
			defaultStats["ENERGYREGEN"] = PowerValue("ENERGYREGEN", 3, 0, 9999, true, list<string>({ "PIETY", "ENERGYREGENPLUS" }));
			defaultStats["SPEED"] = PowerValue("SPEED", 10, 0, 9999, true, list<string>({ "AGILITY"}));
			uniqueID = _uniqueID;
			displayName = _displayName;
			data = _data;
			intData = _intData;
		}
		
		
		wstring getPrintout(string language, Combat & combat, bool includeOtherInfluences) {
			wstring result = L"";
			result += StringToWString(displayName) + L"\n\n\n";

			string equippedSkillTreeName1 = data["equippedSkillTreeNames"]["1"];
			string equippedSkillTreeName2 = data["equippedSkillTreeNames"]["2"];

			if (equippedSkillTreeName1 != "" and equippedSkillTreeName2 != "") {
				result += strings[language]["Skill Tree Names"][equippedSkillTreeName1] + L", ";
				result += strings[language]["Skill Tree Names"][equippedSkillTreeName2];
			}

			if (equippedSkillTreeName1 == "" and equippedSkillTreeName2 != "") {
				result += strings[language]["Skill Tree Names"][equippedSkillTreeName2];
			}

			if (equippedSkillTreeName2 == "" and equippedSkillTreeName1 != "") {
				result += strings[language]["Skill Tree Names"][equippedSkillTreeName1];
			}

			result += L"\n";

			for (auto attribute : combat.AttributesInOrder.internalList) {
				wstring toAdd = strings[language]["Attribute Names"][attribute];
				wstring gap = getGap(toAdd);
				result += toAdd + L":" + gap + L"②" + to_wstring(getAttValue(attribute, *&combat, includeOtherInfluences)) + L" \n";
			}
			
			result += L"\n";

			for (auto stat : combat.statsInOrder.internalList) {
				wstring toAdd = strings[language]["Stat Names"][stat];
				wstring gap = getGap(toAdd);
				result += toAdd + L":" + gap + L"②" + to_wstring(getPowerOfThis(defaultStats[stat], includeOtherInfluences, *&combat)) + L" \n";
			}
			result += strings[language]["GUI"]["PHYSICALPOWER"] + to_wstring(getPowerOfThis(PowerValue("PHYSICAL", 100, 0, 9999, true, list<string>({ "STRENGTH" })), includeOtherInfluences, *&combat)) + L"% \n";
			result += strings[language]["GUI"]["MAGICALPOWER"] + to_wstring(getPowerOfThis(PowerValue("MAGICAL", 100, 0, 9999, true, list<string>({ "INTELLIGENCE" })), includeOtherInfluences, *&combat)) + L"% \n";
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
		wstring getSkillPrintOut(string language, string skillID, Combat & combat, bool includeOtherInfluences) {
			Combat::Skill theSkill = combat.skillDefinitions[skillID];
			wstring base = theSkill.getBaseDescription(language);
			wstring result = base;
			for (auto powerName : theSkill.powerValues.getKeys().internalList) {
				result = WSReplace(result, StringToWString("$" + powerName + "$"), to_wstring(getPowerOfThis(theSkill.powerValues[powerName], includeOtherInfluences, *&combat)));
			}
			return result;
		}
		wstring getSkillName(string language, string skillID) {
			return strings[language]["Skill Names"][skillID];
		}
		wstring getEquipmentPrintout(string language, Combat & combat) {
			wstring result = L"Current Equipment:\n";

			if (data["equipmentNames"].getKeys().empty()) {
				result += L"No Equipment";
				return result;
			}
			for (auto type : { "Weapon","Armour","Accessory" }) {
				if (data["equipmentNames"].getKeys().contains(type)) {
					string itemID = data["equipmentNames"][type];
					Equipment equipment = combat.equipmentDefinitions[itemID];
					result += equipment.printout(language, *&combat);
				}
			}

			return result;
		}
		Map<string, wstring> getSkillTreesNamesForEditPrintout(string language) {
			Map<string, wstring> result;
			Map<string, string> equippedSkillTrees; equippedSkillTrees.internalMap = saveContainer.current.equippedSkillTrees[uniqueID];
			for (auto x : { "1", "2" }) {
				if (equippedSkillTrees.hasKey(x)) {
					result[x] = strings[language]["Skill Tree Names"][equippedSkillTrees[x]] + L" ↓";
				}
				else {
					result[x] = L"Select Skill Tree↓";
				}
			}
			return result;
		}
		List<string> getNamesOfAllKnownSkillTrees(Combat& combat) {
			List<string> results;
			for (auto skill : saveContainer.current.knownSkills[uniqueID]) {
				Skill defined = combat.skillDefinitions[skill];
				results.addToBackIfNotAlreadyInList(defined.skillTree);
			}
			return results;
		}
		List<string> getNamesOfAllUnusedSkillTrees(Combat& combat) {
			// any unequipped skill tree names
			List<string> all = getNamesOfAllKnownSkillTrees(*&combat);
			List<string> equipped = data["equippedSkillTreeNames"].getValues();
			for (auto name : equipped.internalList) {
				all.forcibleRemove(name);
			}
			return all;
		}

		int getPowerOfThis(PowerValue P, bool includeOtherInfluences, Combat & combat) {
			Map<string, int> percentInfluences;
			for (auto att : intData.getKeys().internalList) {
				percentInfluences[att + "_ATT"] = intData[att];
			}
			if (!includeOtherInfluences) { // just use att investment
				return P.getResultantValue(*&combat, percentInfluences, {});
			}
			Map<string, int> flatInfluences;
			int slot = 0; // if all weps have same influence make them all stack by giving them unique ids
			for (auto equipment : data["equipmentNames"].getValues().internalList) {
				Equipment thisItem = combat.equipmentDefinitions[equipment];
				for (auto effect : thisItem.powers.internalList) {
					if (effect.isFlat) {
						if (intData.getKeys().contains(effect.tag)) {
							percentInfluences[effect.tag + "_ATT"] += effect.influence;
						}
						else {
							flatInfluences[effect.tag + "_EQUIP" + to_string(slot)] = round(effect.influence);
						}
					}
					else {
						if (intData.getKeys().contains(effect.tag)) {
							percentInfluences[effect.tag + "_ATT"] *= effect.influence;
						}
						percentInfluences[effect.tag + "_EQUIP" + to_string(slot)] = effect.influence;
					}
				}
				slot++;
			}
			return P.getResultantValue(*&combat, percentInfluences, flatInfluences);
		}
		int getAttValue(string att, Combat& combat, bool includeOtherInfluences) {
			int result = intData[att];
			if (!includeOtherInfluences) {
				return result;
			}
			for (auto equipment : data["equipmentNames"].getValues().internalList) {
				Equipment thisItem = combat.equipmentDefinitions[equipment];
				for (auto effect : thisItem.powers.internalList) {
					if (effect.tag == att) {
						if (effect.isFlat) {
							result += effect.influence;
						}
						else {
							result *= effect.influence;
						}}}}
			return result;
		}

		string uniqueID;
		string displayName;
		Map<string, Map<string, string>> data;
		Map<string, int> intData;
		Map<string, PowerValue> defaultStats;
		Map<string, PowerValue> combatStats; // stats that change during combat
	};
	void defineAllSkills() {
		// DEFAULT
		skillDefinitions["DEFAULT_ATTACK"] = Skill("DEFAULT_ATTACK", "DEFAULT_ATTACK", "Default", SKILLICON_ATTACK, 0, 0, 0, list<string>({"DAMAGE_SINGLE"}), list<string>({"PHYSICAL","ATTACK"}), Map<string, PowerValue>({
			pair<string, PowerValue>("POWER1",PowerValue("POWER1",10,0,999,true,list<string>({"STRENGTH"}))) }), list<string>({ "DEALDAMAGE","DEALPHYSICALDAMAGE" }));
		skillDefinitions["DEFAULT_WAIT"] = Skill("DEFAULT_WAIT", "DEFAULT_WAIT", "Default", SKILLICON_WAIT, 0, 0, 0, {}, {}, {}, list<string>({ "WAIT" }));

		// CLEROMANCY
		skillDefinitions["Heal Wounds"] = Skill("Heal Wounds", "Heal Wounds", "Cleromancy", SKILLICON_HEALWOUNDS, 5, 1, 1,
			list<string>({"LIFEHEAL_SINGLE"}),
			list<string>({"MAGICAL","HOLY", "HEAL","TARGETSALLIES"}),
			Map<string, PowerValue>({ pair<string, PowerValue>("POWER1", PowerValue("POWER1", 40, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"})))}),
			list<string>({"ALLYNEEDSHEALING"}));

		// HAGIOMANCY
		skillDefinitions["Heavenstrike"] = Skill("Heavenstrike", "Heavenstrike", "Hagiomancy", SKILLICON_HEAVENSTRIKE, 10, 2, 0, 
			list<string>({ "DAMAGE_SINGLE_HOLY", "HEAVENSTRIKE"}),
			list<string>({"MAGICAL","HOLY", "ELITE", "TARGETSFOES"}), 
			Map<string, PowerValue>({ 
				pair<string, PowerValue>("POWER1", PowerValue("POWER1", 70, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER2", PowerValue("POWER2", 30, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({"DEALDAMAGE", "DEALHOLYDAMAGE"}));

		skillDefinitions["Light of Day"] = Skill("Light of Day", "Light of Day", "Hagiomancy", SKILLICON_LIGHTOFDAY, 5, 0, 0, 
			list<string>({ "LIGHT OF DAY" }), 
			list<string>({ "MAGICAL","HOLY", "TARGETSFOES"}),
			Map<string, PowerValue>({ pair<string, PowerValue>("POWER1", PowerValue("POWER1", 30, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({"KILLUNDEAD", "KILLDEMONS"}));

		skillDefinitions["Exile"] = Skill("Exile", "Exile", "Hagiomancy", SKILLICON_EXILE, 10, 0, 1,
			list<string>({ "EXILE" }),
			list<string>({ "MAGICAL","HOLY", }),
			Map<string, PowerValue>({ pair<string, PowerValue>("POWER1", PowerValue("POWER1", 10, 1, 10, false, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({ "KILLSUMMON" }));

		skillDefinitions["Strength of Reason"] = Skill("Strength of Reason", "Strength of Reason", "Hagiomancy", SKILLICON_STRENGTHOFREASON, 10, 0, 1,
			list<string>({ "STRENGTHOFREASON" }),
			list<string>({ "MAGICAL","HOLY","BOON","TARGETSALLIES"}),
			Map<string, PowerValue>({ 
				pair<string, PowerValue>("POWER1", PowerValue("POWER1", 5, 0, 10, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER2", PowerValue("POWER2", 8, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))), }),
			list<string>({"ENCHANTSELF","ENCHANTALLY", "PHYSICALBUFFSELF","PHYSICALBUFFALLY"}));
	
		// SANGROMANCY
		skillDefinitions["Life Drain"] = Skill("Life Drain", "Life Drain", "Sangromancy", SKILLICON_LIFEDRAIN, 10, 0, 1,
			list<string>({ "LIFEDRAIN" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY"}),
			Map<string, PowerValue>({ pair<string, PowerValue>("POWER1", PowerValue("POWER1", 5, 5, 8, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("POWER2", PowerValue("POWER2", 5, 1, 10, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"})))
				}),
			list<string>({ "DEALDAMAGE", "INEEDHEALING"}));

		// NECROMANCY
		skillDefinitions["Animate Skeleton Warrior"] = Skill("Animate Skeleton Warrior", "Animate Skeleton Warrior", "Necromancy", SKILLICON_ANIMATESKELETONWARRIOR, 35, 2, 5,
			list<string>({ "ANIMATE_SKELETON_WARRIOR"}),
			list<string>({ "MAGICAL","UNHOLY", "ELITE", "SUMMON" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER1", PowerValue("POWER1", 60, 0, 999, true, list<string>({ "INTELLIGENCE", "UNHOLYBOOST"}))),
				pair<string, PowerValue>("POWER2", PowerValue("POWER2", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "UNHOLYBOOST"}))),
}),
				list<string>({ "SUMMON",}));

		// METEOMANCY
		skillDefinitions["Rainstorm"] = Skill("Rainstorm", "Rainstorm", "Meteomancy", SKILLICON_RAINSTORM, 15, 1, 0,
			list<string>({ "RAINSTORM" }),
			list<string>({ "MAGICAL","WATER","ELEMENTAL"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER1", PowerValue("POWER1", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),}),
				list<string>({ "MAKEITRAIN" }));

		// ELECTROMANCY
		skillDefinitions["Plasma Pulse"] = Skill("Plasma Pulse", "Plasma Pulse", "Electromancy", SKILLICON_PLASMAPULSE, 35, 2, 5,
			list<string>({ "DAMAGE_SINGLE_ELECTRIC", "APPLY_CONCUSSION", }),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", "ELITE"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER1", PowerValue("POWER1", 70, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER2", PowerValue("POWER2", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE",}));

		// DUAL WEP MASTERY
		skillDefinitions["Doublestrike"] = Skill("Doublestrike", "Doublestrike", "Dual Weapon Mastery", SKILLICON_DOUBLESTRIKE, 15, 0, 0,
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", "DAMAGE_SINGLE_PHYSICAL" }),
			list<string>({ "PHYSICAL",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER1", PowerValue("POWER1", 4, 0, 999, true, list<string>({ "STRENGTH", "Dual Weapon MasteryBOOST"}))), }),
				list<string>({ "DEALDAMAGE" }));

		// UMBROMANCY
		skillDefinitions["Shadow Spike"] = Skill("Shadow Spike", "Shadow Spike", "Umbromancy", SKILLICON_SHADOWSPIKE, 15, 0, 0,
			list<string>({ "INTERRUPT_AOE", "APPLY_BLINDNESS_AOE" }),
			list<string>({ "MAGICAL","ELITE"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER1", PowerValue("POWER1", 8, 1, 5, false, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))), }),
				list<string>({ "BLINDSOMEONE", "INTERRUPTSOMEONE" }));

		// 1H WEP MASTERY
		skillDefinitions["Fine Strike"] = Skill("Fine Strike", "Fine Strike", "1H Weapon Mastery", SKILLICON_FINESTRIKE, 5, 0, 0,
			list<string>({ "DAMAGE_SINGLE_PHYSICAL",}),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER1", PowerValue("POWER1", 33, 0, 999, true, list<string>({ "STRENGTH", "1H Weapon MasteryBOOST"}))), }),
				list<string>({ "DEALDAMAGE" }));

		// WAYFARING
		skillDefinitions["Gentleman's Riposte"] = Skill("Gentleman's Riposte", "Gentleman's Riposte", "Wayfaring", SKILLICON_GENTLEMANSRIPOSTE, 5, 0, 10,
			list<string>({ "RIPOSTE", }),
			list<string>({ "PHYSICAL","ELITE"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER1", PowerValue("POWER1", 2, 0, 15, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))), }),
				list<string>({ "DEALDAMAGE" }));

	}
	void defineAllEquipment() {
		// TOMES
		for (auto skill : skillDefinitions.getValues().internalList) {
			string colour = "EQUIPMENTBLUE";
			int price = 100;
			if (skill.isElite()) {
				colour = "ELITESKILLYELLOW";
				price = 1000;
			}
			string tomeOf = WStringToString(strings["ENG"]["Unique Item Strings"]["TOMEOF"]);
			equipmentDefinitions["Tome of " + skill.uniqueID] = Equipment("Tome of " + skill.uniqueID, "Tome", UNIMPLEMENTED_IMAGE, {}, colour, price);
			for (auto const& [language, value] : strings) {
				wstring name = strings[language]["Unique Item Strings"]["TOMEOF"] + strings[language]["Skill Names"][skill.uniqueID];
				wstring description = WSReplace(strings[language]["Unique Item Strings"]["TEACHES"], L"$REPLACE$", strings[language]["Skill Names"][skill.uniqueID]);
				strings[language]["Item Names"][tomeOf + skill.uniqueID] = name;
				strings[language]["Item Descriptions"][tomeOf + skill.uniqueID] = description;
			}
		}

		// WEAPONS
		equipmentDefinitions["Withered Secespita"] = Equipment("Withered Secespita", "Weapon", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("PIETY",1.0f,true,true), 
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Suero's Blade"] = Equipment("Suero's Blade", "Weapon", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("STRENGTH",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Blades of House JaqMaq"] = Equipment("Blades of House JaqMaq", "Weapon", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("AGILITY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Tibetan Tie Bian"] = Equipment("Tibetan Tie Bian", "Weapon", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("ENERGYREGENPLUS",5.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Roger Bacon's Quill"] = Equipment("Roger Bacon's Quill", "Weapon", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("INTELLIGENCE",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);

		// ARMOUR
		equipmentDefinitions["Vatican Vestiments"] = Equipment("Vatican Vestiments", "Armour", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("HOLYBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Martin's Cloak"] = Equipment("Martin's Cloak", "Armour", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("PHYSICALARMOUR",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Alhambran Tunic"] = Equipment("Alhambran Tunic", "Armour", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("SHADOWBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Insulating Gloves"] = Equipment("Insulating Gloves", "Armour", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("ELECTRICARMOUR",5.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Ming Theatre Costume"] = Equipment("Ming Theatre Costume", "Armour", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("BLOODBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);

		// ACCESSORIES
		equipmentDefinitions["Cross of St Jeanne-Marie"] = Equipment("Cross of St Jeanne-Marie", "Accessory", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("PIETY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Matteo Carreri's Locket"] = Equipment("Matteo Carreri's Locket", "Accessory", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("WAYFARINGBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Theoricae Novae Planetarum"] = Equipment("Theoricae Novae Planetarum", "Accessory", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("INTELLIGENCE",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Lunyu Page Fragment"] = Equipment("Lunyu Page Fragment", "Accessory", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("UNHOLYBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Mask of House JaqMaq"] = Equipment("Mask of House JaqMaq", "Accessory", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("AGILITY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Joan of Arc's Necklace"] = Equipment("Joan of Arc's Necklace", "Accessory", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("INTELLIGENCE",5.0f,true,true),
			Equipment::Effect("FIREBOOST",1.5f,true,false),
			Equipment::Effect("SELFIMMOLATE",0.0f,true,true),
			}), "ELITESKILLYELLOW",9999);
		equipmentDefinitions["The Eyes of St Lucy"] = Equipment("The Eyes of St Lucy", "Accessory", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("HOLYBOOST",0.5f,true,false),
			Equipment::Effect("PIETY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 150);
		equipmentDefinitions["al-Hajar al-Aswad"] = Equipment("al-Hajar al-Aswad", "Accessory", UNIMPLEMENTED_IMAGE, List<Equipment::Effect>({
			Equipment::Effect("HOLYBOOST",1.3f,true,false),
			Equipment::Effect("UNHOLYBOOST",1.3f,true,false),
			}), "ELITESKILLYELLOW", 9999);

	}
	List<string> getNamesOfAllSkillTreeNames(string language) {
		Map<string, wstring> skillTrees; skillTrees.internalMap = strings[language]["Skill Tree Names"];
		return skillTrees.getKeys();
	}
	List<string> getNamesOfAllItemTypeNames(string language) {
		Map<string, wstring> itemTypes; itemTypes.internalMap = strings[language]["Item Type Names"];
		return itemTypes.getKeys();
	}
	Map<string, int> getInventoryItemsOfXCategory(string type) {
		Map<string, int> inventoryItems; inventoryItems.internalMap = saveContainer.current.inventory;
		Map<string, int> results;
		for (auto item : inventoryItems.getKeys().internalList) {
			Equipment def = equipmentDefinitions[item];
			if (def.category == type and inventoryItems[item] > 0) {
				results[item] = inventoryItems[item];
			}
		}
		return results;
	}

	Combatant loadPartyMemberAsCombatant(string name) {
		Map<string, string> equippedSkillNames; equippedSkillNames.internalMap = saveContainer.current.equippedSkills[name];
		Map<string, string> equippedSkillTreeNames; equippedSkillTreeNames.internalMap = saveContainer.current.equippedSkillTrees[name];
		Map<string, string> equipment; equipment.internalMap = saveContainer.current.equippedItems[name];
		Map<string, int> attributeInvestments; attributeInvestments.internalMap = saveContainer.current.attributeInvestments[name];
		Map<string, Map<string, string>> data;
		data["equippedSkillNames"] = equippedSkillNames;
		data["equipmentNames"] = equipment;
		data["equippedSkillTreeNames"] = equippedSkillTreeNames;
		return Combatant(name, name, attributeInvestments, data);
	}
	Map<string, Skill> getSkillsOnASkillBar(Map<string, string> skillNames, bool full) {
		Map<string, Combat::Skill> skillsToDraw;
		if (full) {
			skillsToDraw["0"] = skillDefinitions["DEFAULT_ATTACK"];
			skillsToDraw["6"] = skillDefinitions["DEFAULT_WAIT"];
		}
		for (auto skillSlot : skillNames.getKeys().internalList) {
			skillsToDraw[skillSlot] = skillDefinitions[skillNames[skillSlot]];
		}
		return skillsToDraw;
	}
	Map<string, Skill> skillDefinitions;
	Map<string, Equipment> equipmentDefinitions;
	Map<string, float> influenceLookups;
	List<string> AttributesInOrder;
	List<string> statsInOrder;
	Map<string, map<string, int>> defaultAttInvestments;
	Map<string, map<string, string>> defaultSkillChoices;
	Map<string, map<string, string>> defaultSkillTreeChoices;
	Map<string, map<string, string>> defaultEquipment;
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
						audio.playSound(stoi(songName), volume, true, true, 0);
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
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["PLAYER"])),
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
					string scale = x.scale;
					Event("Load" + x.name, "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", x.imageSources),
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
				}
				if (data["getStringFromCombatant"] == "skillName") {
					message = combat.loadPartyMemberAsCombatant(data["message"]).getSkillName(gameEngine.language, data["skill"]);
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
				if (!graphics.doesThisTextAlreadyExist(uniqueID)) {
					Graphics::Text * theText = graphics.addText(new Graphics::Text(message, format, position, anchorStyle, size, colour, shadowColour, uniqueID), layer);
					if (animated) {
						if (styles.contains("TYPEWRITER")) {
							theText->startTypewriter();
						}
					}
				}
				else {
					Graphics::Text* theText = graphics.accessTextViaUniqueID(uniqueID);
					if (!animated) {
						if (theText->message != message) {
							theText->message = message;
						}
					}
					if (animated and styles.contains("TYPEWRITER")) {
						if (data["animateExisting"] == "0") {
							if (theText->fullMessage != message) {
								theText->message = message;
								theText->startTypewriter();
							}
						}
						int typewriterSpeed = 5;
						wstring full = theText->fullMessage;
						bool finished = theText->message == full;
						int howFarAlong = theText->howFarAlong();
						if (CLOCK.hasEnoughTimePassed("TYPEWRITER", typewriterSpeed)) {
							if (finished) {
								return true;
							}
							else {
								if (theText->fullMessage == L"") { return false;  } // not ready to be animated yet
								int size = theText->message.size();
								theText->message += full[size];
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
					graphics.addText(new Graphics::Text(L"", "Centaur_25", { 0,0 }, "TOPLEFT", { 100,100 }, graphics.Colours["OBVIOUSPINK"], graphics.Colours["BLACK"], uniqueID), layer);
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

				theText->message = StringToWString(message);
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
				return gameEngine.skillAnimationContainer.handleAnimation(*&gameEngine, graphics.accessPlayerImage(), graphics.accessImageViaUniqueID("victim"), "Life Drain", {}, {});
			}
			if (type == "CLICKANDDRAG") {				
				//Event("Debug", "DEBUGUSERINPUT", {}).run(*&gameEngine);
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
						graphics.bumpLayer(image, -2);
					}
					pair<float, float> click = controller.mouseClickPosition;
					for (auto const& ID : draggable.internalList) {
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(ID);
						if (theImage == NULL) { continue; }
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
						partyRegion.top = 120;
						partyRegion.left = 350;
						partyRegion.bottom = 250;
						partyRegion.right = 630;

						RECT reserveRegion;
						reserveRegion.top = 120;
						reserveRegion.left = 780;
						reserveRegion.bottom = 350;
						reserveRegion.right = 1083;

						pair<float, float> unclickPos = controller.mouseUnclickPosition;

						bool goingToParty = graphics.isThisInsideRect(unclickPos, partyRegion);
						bool goingToReserve = graphics.isThisInsideRect(unclickPos, reserveRegion);
						string lhs = split(graphics.recentlyFinishedBeingDragged.front(), "_").at(0);
						List<string> theCurrentParty = saveContainer.current.party;

						if (goingToReserve and theCurrentParty.contains(lhs) and theCurrentParty.size() > 1) {
						// move out of party
							theCurrentParty.internalList.remove(lhs);
							saveContainer.current.party = theCurrentParty.internalList;
							forceRedraw = true;

							string nextLeader = saveContainer.getCurrentMainCharacter();
							if (lhs != nextLeader) {
								Event("PrintoutPlayer", "SWAPEXPLORERS", List<pair<string, string>>({ pair<string, string>("previous", lhs),pair<string, string>("next", nextLeader), })).run(*&gameEngine);
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
						pair<string, string>("what", "PARTY"),
						pair<string, string>("moveExisting", "1"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							})).run(*&gameEngine);
						Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
							pair<string, string>("offsetX", "67"),
							pair<string, string>("offsetY", "25"),
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
				int layer = 10;
				if (!graphics.doesThisTextAlreadyExist(textUniqueID)) {
					graphics.addText(new Graphics::Text(L"", "Centaur_25", { 2,80 }, "TOPLEFT", { 100,100 }, graphics.Colours["WHITE"], graphics.Colours["BLACK"], textUniqueID), layer);
				}
				Graphics::Text* theText = graphics.accessTextViaUniqueID(textUniqueID);
				theText->message = StringToWString(explorer.debug());
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
				for (auto s : saveContainer.current.party) {
					saveContainer.current.knownSkills[s] = combat.skillDefinitions.getKeys().internalList;
				}
				// test any npc
				//saveContainer.current.party.push_front("White Knight");
				return true;
			}
			if (type == "EXPLORE") {
				//Event("userInput", "DEBUGWALKING", { }).run(*&gameEngine);
				//Event("", "DEBUGUSERINPUT", {}).run(*&gameEngine);

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
						Args.toggle("SPEEDCHEAT");
					}
					if (controller.hasThisBeenPressed(VK_F2)) {
						gameEngine.activeProcedure.eventList.push_front(Event("Debug Bezier", "DEBUGBEZIER", {}));
						return false;
					}
					if (controller.hasThisBeenPressed(VK_F3)) {
						saveContainer.current.flags["debugFlag"] = true;
					}
					if (controller.hasThisBeenPressed(VK_F4)) {
						gameEngine.stateFlags["SHOWFPS"] = "1";
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
				if (force or (moving and CLOCK.hasEnoughTimePassed("EXPLORE",exploreAnimationSpeeds["MOVE"]))) {
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
					for (auto walkable : explorer.getObjectsThatAreClose().internalList) {
						if (walkable.data.getKeys().contains("message") and walkable.canInteract) {
							popUpTextNeedsToBeDrawn = true;
							Event("PopUpTextOnMap", "DRAWTEXT", walkable.data).run(*&gameEngine);
								mapPopUpTextExists = true;
							Event("Map Move", "MAPMOVE", { pair<string, string>("copy",walkable.data["copy"]) }).run(*&gameEngine); // call again to move text to right position
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
						stopExploringStartCutscene = true;
					}
					if (walkable.data.getKeys().contains("cutscene") and userInput and walkable.canInteract) {
						whichCutscene = walkable.data["cutscene"];
						whichCutscene = FlagDependentCutsceneNameFinder::getNameOfCutsceneDependingOnFlags(whichCutscene);
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
					graphics.addText(new Graphics::Text(L"", "Centaur_25", { 2,95 }, "TOPLEFT", { 100,100 }, graphics.Colours["WHITE"], graphics.Colours["BLACK"], textUniqueID), layer);
				}
				Graphics::Text* theText = graphics.accessTextViaUniqueID(textUniqueID);
				string message = "Average Frame Render Time: ";
				message += to_string(average);
				message += " FPS: ";
				message += to_string(1000 / average);
				theText->message = StringToWString(message);
				return false;
			}
			if (type == "DIALOGUE") {
				string cutscene = data["cutscene"];
				string speakerID = data["speaker"];
				string displayName = speakerID;
				string line = name;
				string direct = data["direct"]; // 1 = use string here 0 = get string from Strings.h
				if (direct != "1") {
					line = gameEngine.language + "_" + cutscene + "_" + line + " " + speakerID;
				}
				bool hideSpeakerName = speakerID.find("?") != -1;
				if (hideSpeakerName) {
					speakerID = SReplace(speakerID, "?", "");
					speakerID = SReplace(speakerID, "$DIRECT$", "");
					displayName = "???";
				}
				if (speakerID == "PLAYER") {
					speakerID = saveContainer.getCurrentMainCharacter();
					displayName = saveContainer.getCurrentMainCharacter();
					displayName = gameEngine.language + "_NPCNames_" + displayName;
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
					pair<string, string>("message",displayName),
					pair<string, string>("direct", "0"),
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
				pair<string, string>("animateExisting","0"),
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
				if (finishedWriting and userInput) {
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
				float gap = 10;
				List<Menu::Button> dynamicButtons;
				dynamicButtons.push_back(Menu::standardButton("CANCELSKILLTREECHOICE", "GUI_CANCELSKILLTREECHOICE", startPosition));
				for (int x = 0; x < toDraw.size(); x++) {
					dynamicButtons.push_back(Menu::standardButton(toDraw.at(x), "Skill Tree Names_" + toDraw.at(x), { startPosition.first, startPosition.second + gap * (x + 1) }));
				}
				gameEngine.storedMenus[whichMenu].buttons = dynamicButtons;
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
				Menu menu = gameEngine.storedMenus[menuName];
				List<Menu::Button> toLoad = menu.getButtonsToLoad();
				for (int x = 0; x < toLoad.size(); x++) {
					Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
						pair<string, string>("uniqueID", menuName),
						pair<string, string>("which", to_string(x)),
						pair<string, string>("layer", layer),
						pair<string, string>("format", toLoad.at(x).extras["format"]),
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
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						})).run(*&gameEngine);
					Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
						pair<string, string>("offsetX", "67"),
						pair<string, string>("offsetY", "25"),
						pair<string, string>("what", "RESERVES"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						})).run(*&gameEngine);
				}
				if (menu.data.hasKey("SELECTCHARACTERTOEDIT")) {
					gameEngine.stateFlags["PARTYEDITSELECTED"] = saveContainer.getCurrentMainCharacter();
					Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
						pair<string, string>("offsetX", "5"),
						pair<string, string>("offsetY", "20"),
						pair<string, string>("what", "EVERYONE"),
						pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
						})).run(*&gameEngine);
					Event("LoadPartyGrid", "SELECTCHARACTERTOEDIT", Map<string, string>({
						pair<string, string>("offsetX", "5"),
						pair<string, string>("offsetY", "20"),
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
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->message = Graphics::Text::commonTextReplacements(gameEngine.language, merchants.merchantDefinitions[merchantID].getCorrectDialogue(gameEngine.language, menu.data["MERCHANT"]));
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter();
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

					graphics.accessTextViaUniqueID("TradeConfirm_TEXT")->message = dialogue;
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
					toUpdate->message = toPrint;
				}
				return true;
			}
			if (type == "LOADABUTTON") {
				string menuName = data["uniqueID"];
				Menu menu = gameEngine.storedMenus[menuName];
				List<Menu::Button> toLoad = menu.getButtonsToLoad();
				int which = stoi(data["which"]);
				bool justLoadText = data["justLoadText"] == "1";
				bool justLoadImage = data["justLoadImage"] == "1";
				Menu::Button button = toLoad.at(which);
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
				Event("LoadThisButtonText", "DRAWTEXT", Map<string, string>({
					pair<string, string>("message",gameEngine.language + "_" + button.buttonContent),
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
				string what = data["what"];
				string imageIDSuffix = "";
				Map<string, int> toDraw;
				List<string> toDrawOrder;
				float scale = 0.25;
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
				}
				if (what == "EVERYONE") {
					for (auto x : saveContainer.current.allCharacters) {
						toDraw[x] = imageLookup.animationFrames[x]["CARD"].front();
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
					scale = 0.5;
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
					scale = 0.5;
				}
				Map<int, List<pair<float, float>>> positions = Menu::getPlayerCardReformGridPositions(offsetX, offsetY);
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
						graphics.accessImageViaUniqueID(current + imageIDSuffix)->positionAsPercentage = positions[currentRow].at(x);
					}
					else {
					Event("LoadThisCharacter'sCard", "LOADIMAGE", List <pair<string, string>>({
						pair<string, string>("sources", to_string(currentValue)),
						pair<string, string>("x", to_string(positions[currentRow].at(x).first)),
						pair<string, string>("y", to_string(positions[currentRow].at(x).second)),
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
				for (auto who : saveContainer.current.allCharacters) {
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
					theText->message = theCombatant.getEquipmentPrintout(gameEngine.language, *&combat);
					if (gameEngine.stateFlags["PARTYEDITSELECTED"] != "") {
						// someone else was selected so do teardown
						List<Menu::Button> toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons;
						for (int x = 0; x < toHandle.size(); x++) {
							Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
								pair<string, string>("imageID", toHandle.at(x).imageID),
								pair<string, string>("textID",toHandle.at(x).textID),
								})).run(*&gameEngine);
						}
						if (gameEngine.stateFlags["WEPTYPESELECTED"] == "") {
							gameEngine.stateFlags["WEPTYPESELECTED"] = "Weapon";
						}
						Event("AddButtons", "ADDEQUIPBUTTONSTOMENU", Map<string, string>({
						pair<string, string>("category", gameEngine.stateFlags["WEPTYPESELECTED"]),
						pair<string, string>("who", who),
							})).run(*&gameEngine);

						toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons;
						for (int x = 0; x < toHandle.size(); x++) {
							Menu::Button button = toHandle.at(x);
							Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
								pair<string, string>("uniqueID", menuName),
								pair<string, string>("which", to_string(x)),
								pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
								pair<string, string>("format", toHandle.at(x).extras["format"]),
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
							pair<string, string>("offsetY", "18"),
							pair<string, string>("what", "SKILLS"),
							pair<string, string>("skillTreeName",equippedSkillTrees[whichTree]),
							pair<string, string>("who", who),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							})).run(*&gameEngine);
						Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
							pair<string, string>("offsetX", xPositions[whichTree]),
							pair<string, string>("offsetY", "18"),
							pair<string, string>("what", "SKILLBORDERS"),
							pair<string, string>("skillTreeName",equippedSkillTrees[whichTree]),
							pair<string, string>("who", who),
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
					graphics.accessTextViaUniqueID("SKILLTREE1_TEXT")->message = combat.loadPartyMemberAsCombatant(who).getSkillTreesNamesForEditPrintout(gameEngine.language)["1"];
					graphics.accessTextViaUniqueID("SKILLTREE2_TEXT")->message = combat.loadPartyMemberAsCombatant(who).getSkillTreesNamesForEditPrintout(gameEngine.language)["2"];
					return true;
				}

				return false;
			}
			if (type == "HANDLEMENU") {
				//Event("Debug", "DEBUGUSERINPUT", {}).run(*&gameEngine);
				string whichMenu = data["uniqueID"];
				List<Menu::Button> clickables = gameEngine.storedMenus[whichMenu].getButtonsToLoad();
				if (gameEngine.storedMenus[whichMenu].data.getKeys().contains("PARTYSHUFFLE")) {
					Event("HandleRearrange", "HANDLEPARTYREARRANGECLICKANDDRAG", { Map<string, string>({pair<string, string>("mode","shuffle"),}),}).run(*&gameEngine);
				}
				if (gameEngine.storedMenus[whichMenu].data.getKeys().contains("PARTYREFORM")) {
					Event("HandleRearrange", "HANDLEPARTYREARRANGECLICKANDDRAG", { Map<string, string>({
						pair<string, string>("mode","reform"),
						}), }).run(*&gameEngine);
				}
				if (gameEngine.storedMenus[whichMenu].data["SKILLEXPLAIN"] == "1") {
					Event("HandleSkillBarExplain", "HANDLESKILLEXPLAIN", Map<string, string>({
						pair<string, string>("x","5"),
						pair<string, string>("y","10"),
						pair<string, string>("scale","1.0"),
						})).run(*&gameEngine);
				}
				if (gameEngine.storedMenus[whichMenu].data["SKILLEXPLAIN"] == "2") {
					Event("HandleSkillBarExplain", "HANDLESKILLEXPLAIN", Map<string, string>({
						pair<string, string>("x","5"),
						pair<string, string>("y","45"),
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
					Event("CharacterSelection", "HANDLESKILLBAREDIT", Map<string, string>({})).run(*&gameEngine);
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
						})).run(*&gameEngine);
					return false;
				}
				for (auto clickable : hoveredOverItems.internalList) {
					if (!clickable.clickable) { continue; }
					Graphics::Image* theImage = graphics.accessImageViaUniqueID(clickable.imageID);
					theImage->frame = 1;
					string hoveredOverItem = clickable.imageID;
					if (hoveredOverItem != controller.latestMenuItemHovered) {
						Event("PlayHoverSound", "PLAYSFX", Map<string, string>({
							pair<string,string>("audio",to_string(clickable.audioHover)),
							pair<string,string>("direct","1"),
							})).run(*&gameEngine);
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
				}
				if (!anythingHovered) {
					controller.latestMenuItemHovered = "";
					for (auto text : popUpTexts.internalList) {
						Event("RemoveHoverText", "TEARDOWNTEXT", Map<string, string>({ "uniqueID", text })).run(*&gameEngine);
					}
				}
				if (CLOCK.hasEnoughTimePassed("MENUINPUTDELAY", 100) and not controller.menuItemCooldown) {
					for (auto const& [key, value] : gameEngine.storedMenus[whichMenu].getKeyboardShortcutsForThisMenu().internalMap) {
						if (controller.hasThisBeenPressed(key)) {
							controller.menuItemCooldown = true;
							Event("ButtonLogic", "HANDLEBUTTON", Map<string, string>({
								pair<string, string>("uniqueID", value),
								pair<string, string>("menuName", data["uniqueID"]),
								pair<string, string>("whichMerchant", data["whichMerchant"]),
								pair<string, string>("objectID", data["objectID"]),
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
					graphics.accessTextViaUniqueID(who + "_PRINTOUTTEXT")->message = message1;
					graphics.accessTextViaUniqueID(who + "_POINTSLEFTTEXT")->message = message2;
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
					List<Menu::Button> toHandle = gameEngine.storedMenus["MERCHANT"].buttons;
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
					List<Menu::Button> toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons;
					for (int x = 0; x < toHandle.size(); x++) {
						Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
							pair<string, string>("imageID", toHandle.at(x).imageID),
							pair<string, string>("textID",toHandle.at(x).textID),
							})).run(*&gameEngine);
					}
					Event("AddButtons", "ADDEQUIPBUTTONSTOMENU", Map<string, string>({ 
						pair<string, string>("category", category),
						pair<string, string>("who", gameEngine.stateFlags["PARTYEDITSELECTED"]),
						})).run(*&gameEngine);
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "EQUIPMENTEDIT")).run(*&gameEngine);
					toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons;
					for (int x = 0; x < toHandle.size(); x++) {
						Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "EQUIPMENTMANAGEMENT"),
							pair<string, string>("which", to_string(x)),
							pair<string, string>("layer", gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].data["layer"]),
							pair<string, string>("format", toHandle.at(x).extras["format"]),
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
					List<Menu::Button> toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons;
					for (int x = 0; x < toHandle.size(); x++) {
						Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
							pair<string, string>("imageID", toHandle.at(x).imageID),
							pair<string, string>("textID",toHandle.at(x).textID),
							})).run(*&gameEngine);
					}
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
					List<Menu::Button> toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons;
					for (int x = 0; x < toHandle.size(); x++) {
						Event("TearDown", "TEARDOWNABUTTON", Map<string, string>({
							pair<string, string>("imageID", toHandle.at(x).imageID),
							pair<string, string>("textID",toHandle.at(x).textID),
							})).run(*&gameEngine);
					}
					Event("AddButtons", "ADDEQUIPBUTTONSTOMENU", Map<string, string>({
						pair<string, string>("category", category),
						pair<string, string>("who", who),
						})).run(*&gameEngine);
					toHandle = gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons;
					for (int x = 0; x < toHandle.size(); x++) {
						Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
							pair<string, string>("uniqueID", "EQUIPMENTMANAGEMENT"),
							pair<string, string>("which", to_string(x)),
							pair<string, string>("layer", gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].data["layer"]),
							pair<string, string>("format", toHandle.at(x).extras["format"]),
							})).run(*&gameEngine);
					}
					graphics.accessTextViaUniqueID("EQUIPMENTEXPLAIN")->message = combat.loadPartyMemberAsCombatant(who).getEquipmentPrintout(gameEngine.language, *&combat);
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
					string whichMerchant = data["whichMerchant"];
					gameEngine.storedMenus["MERCHANT"].data["MERCHANT"] = "Buy";
					Event("Change Merch Buttons", "ADDMERCHANTITEMBUTTONS", Map<string, string>({
						pair<string, string>("whichMerchant", whichMerchant),
						})).run(*&gameEngine);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->message = Graphics::Text::commonTextReplacements(gameEngine.language, merchants.merchantDefinitions[whichMerchant].getCorrectDialogue(gameEngine.language, "Buy"));
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter();
					return true;
				}
				if (buttonLogic == "MERCHANTSELL") {
					string whichMerchant = data["whichMerchant"];
					gameEngine.storedMenus["MERCHANT"].data["MERCHANT"] = "Sell";
					Event("Change Merch Buttons", "ADDMERCHANTITEMBUTTONS", Map<string, string>({
						pair<string, string>("whichMerchant", whichMerchant),
						})).run(*&gameEngine);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->message = Graphics::Text::commonTextReplacements(gameEngine.language, merchants.merchantDefinitions[whichMerchant].getCorrectDialogue(gameEngine.language, "Sell"));
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter();
					return true;
				}
				if (buttonLogic == "MERCHANTBUYBACK") {
					string whichMerchant = data["whichMerchant"];
					gameEngine.storedMenus["MERCHANT"].data["MERCHANT"] = "Buyback";
					Event("Change Merch Buttons", "ADDMERCHANTITEMBUTTONS", Map<string, string>({
						pair<string, string>("whichMerchant", whichMerchant),
						})).run(*&gameEngine);
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->message = Graphics::Text::commonTextReplacements(gameEngine.language, merchants.merchantDefinitions[whichMerchant].getCorrectDialogue(gameEngine.language, "Buyback"));
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter();
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
						saveContainer.increaseItemInventoryCount(item);
						saveContainer.loseMoney(price);
					}
					if (mode == "Buyback") {
						saveContainer.increaseItemInventoryCount(item);
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
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->message = strings[gameEngine.language]["GUI"]["MERCHANTTHANKS" + mode];
					graphics.accessTextViaUniqueID("MERCHANTDIALOGUE_TEXT")->startTypewriter();
					graphics.accessTextViaUniqueID("MONEYSTATUS_TEXT")->message = to_wstring(saveContainer.current.money);
					return true;
				}
				if (buttonLogic == "AcceptItem") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "YOUFOUNDANITEM")).run(*&gameEngine);
					gameEngine.activeProcedure = Procedure("Explore", { Event("Explore","EXPLORE",{}) });
					string objectName = data["objectID"];
					string flagName = data["objectID"] + "_OPENED";
					saveContainer.current.flags[flagName] = true;
					Explorer::mapObject & obj = explorer.getThisMapObject(objectName);
					obj.canInteract = false;
					Event("UpdateMap", "EXPLORE", Map<string, string>({
						pair <string,string>("force","1"),
						})).run(*&gameEngine);
					saveContainer.save();
					return true;
				}
				if (buttonLogic == "TUTNO") {
					Event("Return", "TEARDOWNMENU", pair<string, string>("uniqueID", "TUTORIALCONFIRM")).run(*&gameEngine);
					string player = saveContainer.getCurrentMainCharacter();
					gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, "HorsemanCutscene2$" + player, player, "EXPLORE");
					return false;
				}
				return true;
}
			if (type == "LOADSKILLBARHERE") {
				bool drawFullSkillbar = data["full"] == "1"; // draw background and default skills
				string who = data["who"];
				float scale = stof(data["scale"]);
				pair<float, float> centreAnchor = { stof(data["x"]), stof(data["y"]) };
				Map<string, string> skills = saveContainer.getNamesOfCurrentEquippedSkills(who, drawFullSkillbar);
				float widthOfOneSkill = Graphics::Drawable().convertActualToPercent(graphics.hwndRenderTarget->GetSize(), { 100,100 }).first * scale;
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
				for (Graphics::Image* theImage : graphics.ImageMap[imageLookup.layerDefaults["BUTTONS"]].internalList) {
					bool hoveredOver = false;
					bool hoverable = false;
					string namingStyle = "SKILLSLOT";

					if (theImage->unique_ID.find("_SKILLSLOT_") != -1) { hoverable = true;}
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

						}
					}
				if (!isMouseHoveredOverAnySkill and graphics.doesThisImageAlreadyExist(skillExplainID)) {
					Event("TearDownImage", "TEARDOWNIMAGE", Map<string, string>({
						pair<string, string>("uniqueID", skillExplainID)})).run(*&gameEngine);
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
				if (isMouseHoveredOverAnySkill) {
					Combat::Skill toDraw = combat.skillDefinitions[whichSkill];
					string textColour = "WHITE";
					int border = toDraw.getBorderSource();
					isElite = toDraw.isElite();
					if (isElite) {
						textColour = "ELITESKILLYELLOW";
					}
					if (!graphics.doesThisImageAlreadyExist(skillExplainID)) {
						Event("DrawText", "DRAWTEXT", Map<string, string>({
						pair<string, string>("message",who),
						pair<string, string>("skill",whichSkill),
						pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
						pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("message",to_string(toDraw.baseCost)),
							pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
							pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("message",to_string(toDraw.baseCastingTime)),
							pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
							pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("message",to_string(toDraw.baseRecharge)),
							pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
							pair<string, string>("format", "LightText_20"),
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
					else {
						Combat::Skill toDraw = combat.skillDefinitions[whichSkill];
						Graphics::Image* theImage = graphics.accessImageViaUniqueID(skillExplainID);
						Graphics::Image* theBorder = graphics.accessImageViaUniqueID(skillExplainBorderID);
						if (theImage->sources.front() != toDraw.imageSource) {
							theImage->resetSources(*&graphics, { toDraw.imageSource });
							theBorder->resetSources(*&graphics, { toDraw.getBorderSource() });
						}
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
							pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("message",to_string(toDraw.baseCost)),
							pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
							pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("message",to_string(toDraw.baseCastingTime)),
							pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
							pair<string, string>("format", "LightText_20"),
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
							pair<string, string>("message",to_string(toDraw.baseRecharge)),
							pair<string, string>("influences",gameEngine.stateFlags["includeEquipmentInStatView"]),
							pair<string, string>("format", "LightText_20"),
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
						return true;
					}
				}	
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
							equippedSkills.internalMap.erase(skillIndex);
							equippedSkills[destinationSlot] = skillName;
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
							pair<string, string>("offsetY", "18"),
							pair<string, string>("what", "SKILLS"),
							pair<string, string>("skillTreeName",equippedSkillTrees[whichTree]),
							pair<string, string>("who", who),
							pair<string, string>("moveExisting", "1"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
							})).run(*&gameEngine);
						Event("LoadPartyGrid", "LOADXINAGRID", Map<string, string>({
							pair<string, string>("offsetX", xPositions[whichTree]),
							pair<string, string>("offsetY", "18"),
							pair<string, string>("what", "SKILLBORDERS"),
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
				gameEngine.storedMenus["EQUIPMENTMANAGEMENT"].buttons = result;
				return true;
			}
			if (type == "ADDMERCHANTITEMBUTTONS") {
				string whichMerchant = data["whichMerchant"];
				string mode = gameEngine.storedMenus["MERCHANT"].data["MERCHANT"];
				List<Menu::Button> toHandle = gameEngine.storedMenus["MERCHANT"].buttons;
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
				for (int x = 0; x < toAdd.size(); x++) {
					Combat::Equipment def = combat.equipmentDefinitions[toAdd.at(x)];
					string itemID = toAdd.at(x);
					Menu::Button current = Menu::smallButton(mode + "_" + toAdd.at(x), "Item Names_" + itemID, positions[x]);
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
					current.extras["hoverTextFormat"] = "Centaur_13",
					result.push_back(current);
				}
				string layer = to_string(imageLookup.layerDefaults["BUTTONS"]);
				gameEngine.storedMenus["MERCHANT"].buttons = result;
				for (int x = 0; x < result.size(); x++) {
					if (result.at(x).uniqueID == "MERCHANTDIALOGUE") { continue; }
					Event("LoadThisButton", "LOADABUTTON", Map<string, string>({
						pair<string, string>("uniqueID", "MERCHANT"),
						pair<string, string>("which", to_string(x)),
						pair<string, string>("layer", layer),
						pair<string, string>("format", result.at(x).extras["format"]),
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
				float unitOfMovement = explorer.unitOfMovement / 2;
				float ignore = -1; // use this to move camera only along x or y
				pair<float, float> currentPosition = explorer.activeCamera.position;
				pair<float, float> targetPosition = { stof(data["x"]), stof(data["y"]) };
				bool reachedDestination = true;
				if (CLOCK.hasEnoughTimePassed("CAMERAMOVE", 50)) {
					if (targetPosition.first != ignore) {
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
					if (object == "CAMERA") { continue; }
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
					if (objectName == "CAMERA") { continue; }
					pair<float, float> currentPosition = currentPositions[objectName];
					pair<float, float> targetPosition = targetPositions[objectName];
					if (objectName == "PLAYER1") {
						currentPosition = explorer.playerOnMap.position;
						List<int> sources;
						List<int> shadowSources;
						string character = saveContainer.getCurrentMainCharacter();
						string newDirection = explorer.decideDirectionDependingOnTwoPoints(currentPosition, targetPosition);
						int animationSpeed = explorer.getAnimationSpeeds()["WALK"];
						if (newDirection != "") {
							sources = imageLookup.animationFrames[character]["WALK_" + newDirection];
							shadowSources = imageLookup.animationFrames["Shadow " + character]["WALK_" + newDirection];
							for (auto const& x : { graphics.accessImageViaUniqueID(character + "_Explore"), graphics.accessImageViaUniqueID(character + "_Shadow") }) {
								x->action = "WALK";
								x->animationSpeed = animationSpeed;
							}
							graphics.accessImageViaUniqueID(character + "_Explore")->resetSources(*&graphics, sources);
							graphics.accessImageViaUniqueID(character + "_Shadow")->resetSources(*&graphics, shadowSources);
						}


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
								})).run(*&gameEngine);
							continue;
						}

						if (objectName == "PLAYER1") {
							pair<float, float> current = explorer.playerOnMap.position;
							pair<float, float> target = targetPositions["PLAYER1"];
							explorer.playerOnMap.position = explorer.moveLHSCloserToRHS(current, target, (targetPositions[objectName].first == ignore), (targetPositions[objectName].second == ignore), unitOfMovement);
						}
						else {
							currentPositions[objectName] = explorer.moveLHSCloserToRHS(currentPositions[objectName], targetPositions[objectName], (targetPositions[objectName].first == ignore), (targetPositions[objectName].second == ignore), unitOfMovement);
						}
						if (CLOCK.hasEnoughTimePassed(objectName + "PLAYAUDIO", 200)) {
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
				string sources = imageLookup.getSequenceAsString(data["sources_1"], data["sources_2"] + "_" + data["sources_3"]);
				string layer = data["layer"];
				string x = data["x"];
				string y = data["y"];
				string opacity = data["opacity"];
				bool followPlayer = data["followPlayer"] == "1";
				string anchor = data["anchor"];
				string scale = "1.0";
				if (data.hasKey("scale")) {
					scale = data["scale"];
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
				
				if (uniqueID.find("PLAYER1") != -1) {
					if (uniqueID == "PLAYER1") {
						image = graphics.accessPlayerImage();
						character = SReplace(character, "PLAYER1", saveContainer.getCurrentMainCharacter());
					}
					if (uniqueID == "Shadow PLAYER1") {
						image = graphics.accessPlayerShadowImage();
						character = SReplace(character, "PLAYER1", saveContainer.getCurrentMainCharacter());
					}
				}
				else {
					image = graphics.accessImageViaUniqueID(uniqueID);
				}

				List<int> sources = imageLookup.animationFrames[character][action + "_" + direction];
				image->resetSources(*&graphics, sources);
				image->action = action;
				image->animationSpeed = explorer.getAnimationSpeeds()[action];
				Event("UpdateMap", "MAPMOVE", {}).run(*&gameEngine);
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
				uniqueID = SReplace(uniqueID, "PLAYER1", saveContainer.getCurrentMainCharacter());
				Graphics::Image* theImage = graphics.accessImageViaUniqueID(uniqueID);
				return theImage->hasThisFinishedAnimating();
			}
			if (type == "STARTCUTSCENEDIRECTLY") {
				string cutsceneName = data["cutsceneName"];
				gameEngine.activeProcedure = gameEngine.makeDynamicCutsceneProcedure(gameEngine.language, cutsceneName, saveContainer.getCurrentMainCharacter(), "EXPLORE");
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
				return gameEngine.skillAnimationContainer.handleAnimation(*&gameEngine, graphics.accessImageViaUniqueID(caster), graphics.accessImageViaUniqueID(target), skillName, {}, extras);
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
		bool handleAnimation(GameEngine & gameEngine, Graphics::Image* caster, Graphics::Image* target, string skill, Map<string, Map<string, string>> numbersToDraw, Map<string,string> extras) {
			pair<float, float> targetLocation = target->positionAsPercentage;
			pair<float, float> casterLocation = caster->positionAsPercentage;
			Map<string, bool> finished; // each bubble
			if (skill == "Life Drain") {
				int numberOfBubbles = 50;
				if (!started) {
					pair<float, float> destination = { casterLocation.first, casterLocation.second - 5 };
					if (extras.hasKey("xOffset")) {
						destination.first += stof(extras["xOffset"]);
					}
					bezierPlots.clear();
					for (int x = 0; x < numberOfBubbles; x++) {
						string imageName = "LIFEBUBBLE" + to_string(x);
						List<string> randomScales = List<string>({"0.1","0.11","0.12","0.13","0.14","0.15","0.16","0.17","0.18","0.19","0.2"});
						List<int> randomTimes = List<int>({50,60,70,80,90,100,110,120,130,});
						Event("Loading Screen", "LOADIMAGE", Map<string, string>(List<pair<string, string>>({
						pair<string, string>("sources", imageLookup.getSequenceAsString("Life Drain", "ACTION_1")),
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
							pair<string, string>("audio","1060"),
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
			return false;
		}
		bool started = false;
		Map<string, List<pair<float, float>>> bezierPlots;
		Map<string, int> timers;
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
				pair<string, string>("targetMap", "BénouvilleTown1"),
			}))),
			Event("Load Map", "LOADMAP", Map<string,string>(List<pair<string,string>>({
				pair<string, string>("targetMap", "BénouvilleTown1"),
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
				Menu::standardButton("QUITNO", "GUI_CANCELBUTTON", {50, 45}),
				Menu::standardButton("QUITYES", "GUI_QUITBUTTON", {50, 60}),
				}),Map<string, string>({
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " QUITNO")}))),
		pair<string, Menu>(
			"PARTYREFORM", Menu("PARTYREFORM", List<Menu::Button>({
				Menu::TextBox("TEXTBOX1", "GUI_PARTYLABEL", "VERY_SMALL", {40, 10}),
				Menu::TextBox("TEXTBOX2", "GUI_RESERVELABEL", "VERY_SMALL", {75, 10}),
				Menu::standardButton("FROMPARTYREFORMTOPARTYMANAGEMENT", "GUI_FROMAUDIOTOPAUSEBUTTON", {12, 90}),
				}),Map<string, string>({
					pair<string, string>("PARTYREFORM", "1"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMPARTYREFORMTOPARTYMANAGEMENT")}))),
		pair<string, Menu>(
			"PARTYMANAGEMENT", Menu("PARTYMANAGEMENT", List<Menu::Button>({
				Menu::standardButton("FROMPARTYMANAGEMENTTOSKILLMANAGE", "GUI_FROMPARTYMANAGEMENTTOSKILLMANAGE", {12, 49.8}),
				Menu::standardButton("FROMPARTYMANAGEMENTTOEQUIPMENT", "GUI_FROMPARTYMANAGEMENTTOEQUIPMENT", {12, 60}),
				Menu::standardButton("FROMPARTYMANAGEMENTTOPARTYREFORM", "GUI_FROMPARTYTOREFORM", {12, 70}),
				Menu::standardButton("PARTYMANAGEHELP", "GUI_PARTYMANAGEHELP", {12, 80}),
				Menu::Button("equipmentToggle","GUI_NOTEXT","GUI_NOTEXT", "equipmentToggle",{SHOWEQUIPMENT},BUTTON_CLICK_WAV, BUTTON_HOVER_WAV,{25,60}, true,true, Map<string, string>({
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
				Menu::standardButton("SKILLTREE1", "GUI_SKILLTREESELECTIONEMPTY", {45, 8}),
				Menu::standardButton("SKILLTREE2", "GUI_SKILLTREESELECTIONEMPTY", {75, 8}),
			}),Map<string, string>({
					pair<string, string>("SKILLEXPLAIN", "2"),
					pair<string, string>("SELECTCHARACTERTOEDIT", "1"),
					pair<string, string>("MANAGESKILLS", "1"),
					pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " FROMSKILLMANAGETOPARTYMANAGE")}))),
		pair<string, Menu>(
			"EQUIPMENTMANAGEMENT", Menu("EQUIPMENTMANAGEMENT", Menu::getDefaultButtonsForEquipmentSelect(),Map<string, string>({
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["BUTTONS"])),
					pair<string, string>("SELECTCHARACTERTOEDIT", "1"),
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
							pair<string, string>("BUTTONMAP1", to_string(VK_ESCAPE) + " AcceptItem") }))),
		pair<string, Menu>(
			"TUTORIALCONFIRM", Menu("TUTORIALCONFIRM", List<Menu::Button>({
				Menu::TextBox("TEXTBOX1", "GUI_TUTTEXTBOX", "SMALL", {50, 20}),
				Menu::standardButton("TUTNO", "GUI_TUTNO", {50, 45}),
				Menu::standardButton("TUTYES", "GUI_TUTYES", {50, 60}),
				}), Map<string, string>({}))),
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
		events.push_back(Event("Explore", "EXPLORE", Map<string, string>(
			pair<string, string>({"force","1"}))));
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
			})));
		return Procedure("HandleChest", events);
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

		string mainCharacter = saveContainer.getCurrentMainCharacter();
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
				if (speaker.find("+") != -1) {
					// add something where the line itself must change according to the player
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
				if (speaker == "$FROMCUTSCENETOMENU$") {
					results.push_back(Event("LoadMenu", "FROMCUTSCENETOMENU", Map<string, string>({
						pair<string, string>("menuName", WStringToString(val)),
						})));
					acceptedLines.push_back(thisLine);
					continue;
				}
				if (speaker == "$STARTCUTSCENEDIRECTLY$") {
					string whichCutscene = SReplace(WStringToString(val), "PLAYER",saveContainer.getCurrentMainCharacter());
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
				if (speaker != "PLAYER" and speaker != mainCharacter) {
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
			activeProcedure = storedProcedures["DEBUG3"];
		}
		else {
			throw runtime_error("Not Implemented yet");
		}
		CLOCK.startClock("FPS");
		stateFlags["QUIT"] = "0";
		stateFlags["includeEquipmentInStatView"] = "1";
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

	Procedure activeProcedure;
	Map<string, string> stateFlags;
	string language = "ENG";
	SkillAnimationContainer skillAnimationContainer;
};
GameEngine game;