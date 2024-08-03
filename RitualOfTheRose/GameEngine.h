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
				theImage->animated = true;
				theImage->animationStyles = styles;
				return true;
			}
			if (type == "DRAWTEXT") {
				string message = data["message"];
				string format = data["format"];
				pair<int, int> position = { stoi(data["x"]), stoi(data["y"]) };
				string anchorStyle = data["anchorStyle"];
				pair<int, int> size = { stoi(data["w"]), stoi(data["h"]) };
				vector<float> colour = graphics.Colours[data["colour"]];
				int layer = stoi(data["layer"]);
				string uniqueID = data["uniqueID"];
				graphics.addText(Graphics::Text(message, format, position, anchorStyle, size, colour, unique_ID), layer);
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
					pair<string, string>("styles", "FADEOUT"),}))),
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