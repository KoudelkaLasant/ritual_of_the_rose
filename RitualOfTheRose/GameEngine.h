#pragma once

#include "Graphics.h"
#include "Strings.h"

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
				if (!theImage->animated) {
					theImage->animated = true;
					theImage->animationStyles = styles;
				}
				bool wait = data["wait"] == "TRUE";
				if (wait and styles.contains("FADEOUT") and theImage->opacity != 0.0f) {
					return false;
				}
				return true;
			}
			if (type == "DRAWTEXT") {
				List<string> mp = split(data["message"], " ");
				wstring message = strings[mp.at(0)][mp.at(1)][mp.at(2)];
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
						if (CLOCK.hasEnoughTimePassed("TYPEWRITER", typewriterSpeed)) {
							if (finished) {
								return true;
							}
							else {
								int size = graphics.TextMap[layer].internalMap[uniqueID].message.size();
								graphics.TextMap[layer].internalMap[uniqueID].message += full[size];
								if (styles.contains("PARCHMENT")) {
									
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
	GameEngine() {}
	void setup() {
		string mode = Args.get("mode");
		if (mode == "DEBUG") {
			activeProcedure = Procedure("Load Debug", List<Event>({
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
					pair<string, string>("wait", "TRUE"),}))),
				Event("TeardownImage", "TEARDOWNIMAGE", Map<string, string>(List<pair<string,string>>({
					pair<string, string>("uniqueID", "debugmenulogo"),}))),
				Event("Text", "DRAWTEXT", Map<string, string>(List<pair<string,string>>({
					pair<string, string>("message","ENG DEBUG TestString"),
					pair<string, string>("format", "TESTFONT1"),
					pair<string, string>("anchorStyle", "TOPLEFT"),
					pair<string, string>("x", "0"),
					pair<string, string>("y", "0"),
					pair<string, string>("w", "500"),
					pair<string, string>("h", "500"),
					pair<string, string>("colour", "WHITE"),
					pair<string, string>("shadowColour", "BLACK"),
					pair<string, string>("layer", "10"),
					pair<string, string>("uniqueID", "debugText1"),
					pair<string, string>("animated", "TRUE"),
					pair<string, string>("styles", "TYPEWRITER"),
					}))),
				Event("Wait", "WAIT", Map<string, string>(List<pair<string,string>>({
					pair<string, string>("clockID", "DEBUGCLOCK2"),
					pair<string, string>("waitDuration", "5000"),}))),
				Event("TeardownText", "TEARDOWNTEXT", Map<string, string>(List<pair<string,string>>({
					pair<string, string>("uniqueID", "debugText1"),}))),
				}));
		}
		else {
			ErrorHelper::warning({ "Not Implemented yet" }, true);
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