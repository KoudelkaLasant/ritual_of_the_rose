#pragma once

#include "Graphics.h"

class Event {
public:
	Event() {}
	Event(string _name, Map<string, string> _data) {
		name = _name;
		data = _data;
	}
	virtual bool run() = 0;
	string name;
	bool finished = false;
	Map<string, string> data;
};


class LoadImageEvent : public Event {
public:
	LoadImageEvent() {}
	LoadImageEvent(string _name, Map<string, string> _data) {
		name = _name;
		data = _data;
	}
	bool run() {
		int image = stoi(data["imageID"]);
		pair<int, int> position = { stoi(data["width"]), stoi(data["height"]) };
		string anchor = data["anchor"];
		float opacity = stof(data["opacity"]);
		int layer = stoi(data["layer"]);
		string uniqueID = data["uniqueID"];
		graphics.addImage(new Graphics::Image(image, position, anchor, opacity, uniqueID), layer);
		return true;
	}
};


class Runnable {
public:
	Runnable() {}
	Runnable(string _name, List<Event> _eventList) {
		name = _name;
		eventList = _eventList;
	}
	bool run() {
		while (!eventList.empty() and eventList.pop_front().run()) {}
		return eventList.empty();
	}
	string name = "";
	List<Event> eventList;
};


class Procedure : public Runnable {
public:
	using Runnable::Runnable;
	string name = "None";
	bool run() {
		while (!eventList.empty() and eventList.pop_front().run()) {}
		return eventList.empty();
	}
};


class GameEngine {
	public:
		void setup() {
			string mode = Args.get("mode");
			if (mode == "DEBUG") {
				activeProcedure = Procedure("Load Debug", {
					LoadImageEvent("Load Background", Map<string, string>(List<pair<string,string>>({
						pair<string, string>("imageID", "MAIN_MENU_BACKGROUND"),
						pair<string, string>("x", "50"),
						pair<string, string>("y", "50"),
						pair<string, string>("anchor", "CENTRE"),
						pair<string, string>("opacity", "1.0"),
						pair<string, string>("layer", "0"),
					})))});
			}
			else {
				ErrorHelper::warning({ "Not Implemented yet" }, true);
			}
		}
		void run() {
			activeProcedure.run();
		}

		Procedure activeProcedure;
};
GameEngine game;