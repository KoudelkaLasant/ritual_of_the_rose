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
		buttonReplace(_buttons);
		data = _data;
	}
	Map<int, string> getKeyboardShortcutsForThisMenu() {
		Map<int, string> result;
		for (auto& node : data.getKeys().internalList) {
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
		for (auto button : buttons.getValues().internalList) {
			if (button.clickable and button.visible) {
				results.push_back(button);
			}
		}
		return results;
	}
	List<Button> getButtonsToLoad() {
		List<Button> result = buttons.getValues();
		return result;
	}
	void buttonReplace(List<Button> _buttons) {
		buttons.clear();
		for (auto b : _buttons.internalList) {
			buttons[b.uniqueID] = b;
		}
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
	static Button smallerButton(string baseID, string buttonMessage, pair<float, float> position) {
		Button result;
		result.uniqueID = baseID;
		result.textID = baseID + "_TEXT";
		result.imageID = baseID + "_IMAGE";
		result.sources = { SMALLERBUTTON_DEFAULT, SMALLERBUTTON_HOVERED, SMALLERBUTTON_PRESSED };
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
		if (size == "LARGE") {
			result.sources = List<int>({ TEXTBOX_LARGE });
			result.width = 30;
			result.height = 30;
		}
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
		if (size == "TINY") {
			result.sources = List<int>({ SMALLBUTTON });
			result.width = 14;
			result.height = 8;
			result.extras["format"] = "Centaur_13";
		}
		return result;
	}
	static Button MerchantTalkingBox(string baseID) {
		Button result = Menu::TextBox(baseID, "", "", { 0,0 });
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
		return List<pair<float, float>>({ pair<float, float>(35,25),pair<float, float>(53,25),pair<float, float>(71,25),pair<float, float>(89,25) });
	}
	static const Map<int, List<pair<float, float>>> getPlayerCardReformGridPositions(float xOffset, float yOffset, float xSpace, float ySpace) {
		Map<int, List<pair<float, float>>> results;

		List<pair<float, float>> firstRow({ pair<float, float>(xOffset,yOffset),pair<float, float>(xSpace + xOffset,yOffset),pair<float, float>(xSpace*2 + xOffset,yOffset),
			pair<float, float>(xSpace*3 + xOffset,yOffset), pair<float, float>(xSpace*4 + xOffset,yOffset) });
		for (int x = 0; x < 5; x++) {
			List<pair<float, float>> currentRow = firstRow;
			for (int y = 0; y < currentRow.size(); y++) {
				currentRow.at(y).second += (ySpace * x);
			}
			results[x] = currentRow;
		}
		return results;
	}
	static const Map<string, string> getSkillGridPositions() {
		Map<string, string> result = Map<string, string>({ pair<string, string>("1","35"),pair<string, string>("2","65"), });
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
		result["PARTYSLOT1"] = graphics.makeRect(top, left, bottom, left + gap);
		result["PARTYSLOT2"] = graphics.makeRect(top, (left + gap + 1), bottom, left + gap * 2);
		result["PARTYSLOT3"] = graphics.makeRect(top, left + (gap * 2) + 1, bottom, left + (gap * 3));
		result["PARTYSLOT4"] = graphics.makeRect(top, left + (gap * 3) + 1, bottom, left + (gap * 4));

		return result;
	}
	static const Map<int, pair<float, float>> getItemGridPositions() {
		Map<int, pair<float, float>> results;
		int rowLength = 4;
		int columnLength = 10;
		pair<float, float> topLeftCorner = { 40,35 };
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
	static const List<Button> getDefaultCombatMenuButtons() {
		List<Button> result = List<Button>({
			Menu::smallButton("COMBAT1OPENMENU", "GUI_COMBAT1BUTTON", { 92, 96 }),
			});
		return result;
	}
	static const List<Button> getDefaultMenuButtonsForTomes() {
		List<Button> result = List<Button>({
			Menu::smallButton("SKILLTREETOME", "GUI_AVAILABLETOMES", {20, 10}),
			Menu::smallButton("FROMTOMETOPAUSE", "GUI_FROMAUDIOTOPAUSEBUTTON", {85, 80}),
			});
		return result;
	}
	void editExistingButton(string language, string buttonName, Button replacement) {
		graphics.accessTextViaUniqueID(buttonName + "_TEXT")->resetMessage(*&graphics, strings[language]["GUI"][SReplace(replacement.buttonContent, "GUI_", "")]);
		graphics.accessTextViaUniqueID(buttonName + "_TEXT")->unique_ID = replacement.textID;
		graphics.accessImageViaUniqueID(buttonName + "_IMAGE")->unique_ID = replacement.imageID;
		buttons[buttonName] = replacement;
	}
	void removeThisButton(string buttonName) {
		graphics.tearDownSpecifiedImage(buttonName + "_IMAGE");
		graphics.tearDownSpecifiedText(buttonName + "_TEXT");
		buttons.internalMap.erase(buttonName);
	}
	static const List<Button> getCodexButtons(string currentBook, string currentPage) {
		List<Button> results;
		results.push_back(Menu::smallerButton("FROMCODEXTOPAUSE", "GUI_FROMCODEXTOPAUSEBUTTON", { 95, 96 }));
		Map<string, List<string>> unlockedCodex = codex.getUnlockedCodex();
		int startX = 5;
		int xDistance = 10;
		int startY = 96;
		int yDistance = 5;
		for (auto book : unlockedCodex.getKeys().internalList) {
			Button bookButton = Menu::smallerButton("CODEXBOOK_" + book, "Codex Categories_" + book, { startX, startY });
			bookButton.audioHover = SILENCE_WAV;
			bookButton.audioClick = PARCHMENT_1_WAV;
			results.push_back(bookButton);
			int currentY = startY;
			if (currentBook == book) {
				List<string> sortedPages = unlockedCodex[book]; sortedPages.internalList.sort(); sortedPages.internalList.reverse();
				for (auto page : sortedPages.internalList) {
					currentY -= yDistance;
					Button pageButton = Menu::smallerButton("CODEXPAGE_" + page, codex.nameSources[book] + "_" + page, { startX, currentY });
					pageButton.audioHover = SILENCE_WAV;
					pageButton.audioClick = PARCHMENT_2_WAV;
					results.push_back(pageButton);
				}
			}
			startX += xDistance;
		}
		return results;
	}
	static const Map<int, pair<float, float>> getEffectIconPositions(pair<float, float> anchor) {
		Map<int, pair<float, float>> results;
		int columnHeight = 10;
		int itemSize = 3;
		pair<float, float> start = anchor;
		start.first -= 5;
		start.second -= 10;

		int currentColumn = 0;
		int currentItem = 0;
		for (int x = 0; x < 100; x++) {
			float xPos = start.first + currentColumn;
			float yPos = start.second - (currentItem * itemSize);
			results[x] = { xPos, yPos };
			currentItem++;
			if (currentItem == columnHeight) {
				currentColumn++;
				currentItem = 0;
			}
		}
		return results;
	}
	static string skillGridYLoc() {
		return "25";
	}
	int getIndexOfThisButton(string buttonName) {
		int x = 0;
		for (auto b : buttons.getValues().internalList) {
			if (b.uniqueID == buttonName) {
				return x;
			}
			x++;
		}
		throw exception("Trying to get a button that doesn't currently exist.");
	}

	string uniqueID;
	Map<string, Button> buttons;
	Map<string, string> data;
};