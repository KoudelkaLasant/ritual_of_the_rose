#pragma once
#include "Utils.h"

class Explorer
{
public:
	Explorer() {
		defineAllMaps();
	}
	class obstruction {
	public:
		class triangle {
		public:
			triangle() {}
			triangle(list<pair<float, float>> _points) {
				points = _points;
			}
			List<pair<float, float>> points;
			bool isSteppedOn(pair<float, float> playerPosition) {
				float denominator = ((points.at(1).second - points.at(2).second) * (points.at(0).first - points.at(2).first) + (points.at(2).first - points.at(1).first) * (points.at(0).second - points.at(2).second));
				float a = ((points.at(1).second - points.at(2).second) * (playerPosition.first - points.at(2).first) + (points.at(2).first - points.at(1).first) * (playerPosition.second - points.at(2).second)) / denominator;
				float b = ((points.at(2).second - points.at(0).second) * (playerPosition.first - points.at(2).first) + (points.at(0).first - points.at(2).first) * (playerPosition.second - points.at(2).second)) / denominator;
				float c = 1 - a - b;
				bool result = ((0 <= a && a <= 1) && (0 <= b && b <= 1) && (0 <= c && c <= 1));
				return result;
			}
		};
		bool isSteppedOn(pair<float, float> playerPosition) {
			for (auto x : triangles.internalList) {
				if (x.isSteppedOn(playerPosition)) {
					return true;
				}
			}
			return false;
		}
		List <triangle> triangles;
	};
	class mapFloor : public obstruction {
	public:
		mapFloor() {}
		mapFloor(string _uniqueID, List<triangle> _triangles, bool _isTraversable, Map<string, string> _data) {
			uniqueID = _uniqueID;
			triangles = _triangles;
			traversable = _isTraversable;
			data = _data;
		}
		bool hasAudio() {
			return data["audio"] == "1";
		}
		string audioSource() {
			return data["audio source"];
		}
		void playAudio() {
			if (CLOCK.hasEnoughTimePassed("Explorer Footstep", 500)) {
				audio.playRandomSFXFromThisCollection(audioSource(), audio.volumes["SFXVolume"]);
			}
		}

		string uniqueID;
		bool traversable = true;
		Map<string, string> data;

	};
	class mapObject : public obstruction {
	public:
		mapObject() {};
		mapObject(string _name, bool _canInteract, bool _visible, bool _tracksToPlayer, string _imageSources, string _animated, int _animationSpeed,
			int _layer, string _opacity, string _scale, string _anchor, pair<float, float> _positionOnMap, bool _isObstruction, List<mapFloor::triangle> _triangles, Map<string, string> _data) {
			name = _name;
			canInteract = _canInteract;
			visible = _visible;
			tracksToPlayer = _tracksToPlayer;
			imageSources = _imageSources;
			animationSpeed = _animationSpeed;
			positionOnMap = _positionOnMap;
			anchor = _anchor;
			layer = _layer;
			opacity = _opacity;
			animated = _animated;
			obstruction = _isObstruction;
			triangles = _triangles;
			data = _data;
			scale = _scale;
		}
		bool isSteppedOn(pair<float, float> position) {
			for (auto x : triangles.internalList) {
				if (x.isSteppedOn(position)) {
					return true;
				}
			}
			return false;
		}
		bool isSteppedNear(pair<float, float> position) {
			if (!data.hasKey("expandedInteractionX")) {
				return isSteppedOn(position);
			}
			List<mapFloor::triangle> toCheck = mapObject::resizeLinear(triangles, stof(data["expandedInteractionX"]), stof(data["expandedInteractionY"]), positionOnMap);


			for (auto x : toCheck.internalList) {
				if (x.isSteppedOn(position)) {
					return true;
				}
			}
			return false;

		}
		static mapObject getTreasureChest(string _uniqueID, pair<float, float> _position, string _appearance, string contents) {
			mapObject result;
			result.name = _uniqueID;
			result.data["uniqueID"] = _uniqueID;
			result.canInteract = true;
			result.visible = true;
			result.tracksToPlayer = false;
			string xOffsetForTextOnMap = "-2";
			List<mapFloor::triangle> chestTriangles;
			if (_appearance == "NORMAL_FRONT") {
				result.imageSources = imageLookup.getSequenceAsString("Chest1", "STAND_FRONT");
				result.data["ChestImageSource"] = "Chest1";
				chestTriangles = { mapFloor::triangle({{49.4265079498291,4.762154072523117}, {-2.8783023357391357,48.89237880706787}, {48.81502389907837,29.82090711593628}}),mapFloor::triangle({{-19.547539949417114,13.16099464893341}, {49.4265079498291,4.762154072523117}, {42.853039503097534,-8.155468851327896}}),mapFloor::triangle({{33.22214484214783,-18.321406841278076}, {-19.547539949417114,13.16099464893341}, {42.853039503097534,-8.155468851327896}}),
mapFloor::triangle({{49.4265079498291,4.762154072523117}, {-19.623970985412598,25.84931254386902}, {-2.8783023357391357,48.89237880706787}}),mapFloor::triangle({{-19.547539949417114,13.16099464893341}, {-19.623970985412598,25.84931254386902}, {49.4265079498291,4.762154072523117}}),mapFloor::triangle({{33.22214484214783,-18.321406841278076}, {-19.47110742330551,0.47267675399780273}, {-19.547539949417114,13.16099464893341}}), };
			}
			if (_appearance == "NORMAL2_FRONT") {
				xOffsetForTextOnMap = "-4";
				result.imageSources = imageLookup.getSequenceAsString("Chest2", "STAND_FRONT");
				result.data["ChestImageSource"] = "Chest2";
				chestTriangles = { mapFloor::triangle({{29.89441156387329,-15.748412907123566}, {-27.801993489265442,34.07939672470093}, {29.928600788116455,34.07939672470093}}),mapFloor::triangle({{29.89441156387329,-15.748412907123566}, {-27.880877256393433,-15.748415887355804}, {-27.801993489265442,34.07939672470093}}), };
			}
			if (_appearance == "NORMAL3_FRONT") {
				xOffsetForTextOnMap = "-2";
				result.imageSources = imageLookup.getSequenceAsString("Chest3", "STAND_FRONT");
				result.data["ChestImageSource"] = "Chest3";
				chestTriangles = { mapFloor::triangle({{29.89441156387329,-15.748412907123566}, {-27.801993489265442,34.07939672470093}, {29.928600788116455,34.07939672470093}}),mapFloor::triangle({{29.89441156387329,-15.748412907123566}, {-27.880877256393433,-15.748415887355804}, {-27.801993489265442,34.07939672470093}}), };
			}
			if (_appearance == "ELITE1_FRONT") {
				xOffsetForTextOnMap = "-2";
				result.imageSources = imageLookup.getSequenceAsString("EliteChest", "STAND_FRONT");
				result.data["ChestImageSource"] = "EliteChest";
				chestTriangles = { mapFloor::triangle({{29.89441156387329,-15.748412907123566}, {-27.801993489265442,34.07939672470093}, {29.928600788116455,34.07939672470093}}),mapFloor::triangle({{29.89441156387329,-15.748412907123566}, {-27.880877256393433,-15.748415887355804}, {-27.801993489265442,34.07939672470093}}), };
			}

			result.opacity = "1.0";
			result.animated = "0";
			result.animationSpeed = 100;
			result.layer = imageLookup.layerDefaults["SMALLOBJECTS"];
			result.anchor = "CENTRE";
			result.positionOnMap = _position;
			result.data["contents"] = contents;
			result.obstruction = true;
			// the treasure chest is 3% of the size of a map. Shrink these triangles to X% and move them to the coordiantes
			result.triangles = mapObject::convertFrom100toX(chestTriangles, 0.03, _position);
			if (saveContainer.current.flags[_uniqueID + "_OPENED"]) {
				result.imageSources = imageLookup.getSequenceAsString("Chest1", "OPEN_FRONT");
				if (_appearance == "NORMAL2_FRONT") {
					result.imageSources = imageLookup.getSequenceAsString("Chest2", "OPEN_FRONT");
				}
				result.data["open"] = "1";
			}
			else {
				result.data["open"] = "0";
				result.data["message"] = "$LANGUAGE$_Map Pop Up Text_Open Chest";
				result.data["copy"] = _uniqueID;
				result.data["imageID"] = _uniqueID;
				result.data["colour"] = "WHITE";
				result.data["shadowColour"] = "BLACK";
				result.data["anchorStyle"] = "TOPLEFT";
				result.data["x"] = xOffsetForTextOnMap;
				result.data["y"] = "-10";
				result.data["h"] = "50";
				result.data["w"] = "50";
				result.data["format"] = "LightText_20";
				result.data["layer"] = to_string(imageLookup.layerDefaults["TEXTONMAP"]);
				result.data["uniqueID"] = "mappopuptextID";
				result.data["expandedInteractionX"] = "1";
				result.data["expandedInteractionY"] = "1.1";
				result.data["isAChest"] = "1";
			}
			return result;
		}
		static mapObject getOnetimeTrigger(string _uniqueID, List<mapFloor::triangle> zone, string dontLoadIfNot) {
			// no image, invisible, can be stepped on, if interact = 1 do the cutscene if 0 nothing
			mapObject result;
			result.name = _uniqueID;
			result.visible = false;
			if (saveContainer.current.flags[_uniqueID + "_TRIGGERED"]) {
				result.canInteract = false;
				return result;
			}
			result.triangles = zone;
			bool canInteract = true;
			string trigger = "1";
			if (dontLoadIfNot != "") {
				if (!saveContainer.current.flags[dontLoadIfNot]) {
					canInteract = false;
					trigger = "0";
				}
			}
			result.canInteract = canInteract;
			result.data["cutscene"] = _uniqueID;
			result.data["trigger"] = trigger;
			return result;
		}
		static List<mapFloor::triangle> convertFrom100toX(List<mapFloor::triangle> input, float scale, pair<float, float> position) {
			for (auto& t : input.internalList) {
				for (auto& p : t.points.internalList) {
					p.first *= scale;
					p.second *= scale;
					p.first += position.first;
					p.second += position.second;
				}
			}
			return input;
		}
		static List<mapFloor::triangle> resize(List<mapFloor::triangle> input, float scaleX, float scaleY, pair<float, float> position) {
			for (auto& t : input.internalList) {
				for (auto& p : t.points.internalList) {
					if (p.first > position.first) {
						p.first *= scaleX;
					}
					else {
						p.first *= (1.0f + (1.0 - scaleX));
					}
					if (p.second > position.second) {
						p.second *= scaleY;
					}
					else {
						p.second *= (1.0f + (1.0 - scaleY));
					}
				}
			}
			return input;
		}
		static List<mapFloor::triangle> resizeLinear(List<mapFloor::triangle> input, float amountX, float amountY, pair<float, float> position) {
			for (auto& t : input.internalList) {
				for (auto& p : t.points.internalList) {
					if (p.first > position.first) {
						p.first += amountX;
					}
					else {
						p.first -= amountX;
					}
					if (p.second > position.second) {
						p.second += amountY;
					}
					else {
						p.second -=amountY;
					}
				}
			}
			return input;
		}

		string name = "";
		string anchor;
		bool canInteract = false;
		bool visible = false;
		bool tracksToPlayer = false; // if true it follows the player around the map, i.e. fog or light effect
		string animated = "0";
		string imageSources;
		int animationSpeed = 1;
		pair<float, float> positionOnMap;
		int layer = 1;
		string opacity = "1.0f";
		string scale = "1.0f";
		bool obstruction = false;
		Map<string, string> data; // if interacting with this object triggers something
	};
	class mapInstance {
	public:
		mapInstance() {}
		mapInstance(string _name, int _source, pair<float, float> _playerStartPosition, List<mapObject> _objects, List<mapFloor> _walkables, Map<string, bool> _flags, pair<float, float> _imageSize,
			Map<string, string> _data) {
			name = _name;
			source = _source;
			playerStartPosition = _playerStartPosition;
			objects = _objects;
			walkables = _walkables;
			flags = _flags;
			imageSize = _imageSize;
			data = _data;
		}
		List<string> getSongNames() {
			List<string> result;
			for (auto x : data.getKeys().internalList) {
				if (x.find("song") != -1) {
					result.push_back(data[x]);
				}
			}
			return result;
		}
		List<string> getAmbienceNames() {
			List<string> result;
			for (auto x : data.getKeys().internalList) {
				if (x.find("ambience") != -1) {
					result.push_back(data[x]);
				}
			}
			return result;
		}
		void addNewCutsceneObject(string uniqueID, string characterName, string dirplusaction, pair<float, float> loc, int animationSpeed, int layer) {
			mapObject result;
			result.visible = true;
			result.canInteract = false;
			result.animationSpeed = animationSpeed;
			result.tracksToPlayer = false;
			result.animated = "1";
			result.layer = layer;
			result.imageSources = imageLookup.getSequenceAsString(characterName, dirplusaction);
			result.positionOnMap = loc;
			objects.push_back(result);
		}
		bool doesAnObjectWithThisNameExist(string uniqueID) {
			for (auto x : objects.internalList) {
				if (x.name == uniqueID) {
					return true;
				}
			}
			return false;
		}

		string name = "";
		int source;
		pair<float, float> playerStartPosition;
		List<mapObject> objects;
		List<mapFloor> walkables;
		Map<string, bool> flags; // can influence what gets drawn and how
		pair<float, float> imageSize;
		Map<string, string> data;
	};
	struct playerObject {
		pair<float, float> position;
	};
	struct camera {
		pair<float, float> position;
	};
	class puzzles {
	public:
		puzzles() {
			resetSpigotPuzzle();
		}
		pair<Map<string, string>, string> runWaterPuzzle1Logic(string whichObject) {
			// whichFont - status
						/*
						x
					x		x
				x		x		x
			x		x		x
				x		x		x
					x		x

								9
					7		8
				4		5		6
			1		2		3
				10		11		12
					13		14
								15
			*/
			List<int> chain1 = List<int>({1,4,7,5,3,6,8,9});
			List<int> chain2 = List<int>({1,10,13,11,14,15});

			Map<string, string> result = loadCurrentWaterPuzzle1Status();
			int fontsOn = 0;
			for (auto [key, value] : result.internalMap) {
				if (key.find("FONT") != -1 and key.find("FLOOR") == -1 and value == "ON") {
					fontsOn++;
				}
			}
			if (result["WaterPuzzleActivated"] == "OFF") {
				return { result, "NoChange" };
			}
			string currentStatusOfCurrentObject = result[whichObject];
			if (currentStatusOfCurrentObject == "OFF" and fontsOn >= waterPuzzle1FontsAllowed) {
				return {result, "NoMoreWater"};
			}
			if (whichObject == "FONT2") {
				return { result, "Broken" };
			}
			if (currentStatusOfCurrentObject == "ON" and whichObject == "FONT1") {
				// turn all off
				for (auto res : result.getKeys().internalList) {
					if (res.find("FONT") != -1) {
						result[res] = "OFF";
						result[res + "FLOOR"] = "OFF";
					}
				}
			}
			if (currentStatusOfCurrentObject == "OFF") {
				if (whichObject == "FONT1") {
					result[whichObject] = "ON";
					result[whichObject + "FLOOR"] = "ON";
				}
				if (whichObject == "FONT4" and areAllTheseOn(result, { 1 }, "FONT")) {result[whichObject] = "ON";result[whichObject + "FLOOR"] = "ON";}
				if (whichObject == "FONT7" and areAllTheseOn(result, List<int>({ 1, 4 }), "FONT")) {result[whichObject] = "ON";result[whichObject + "FLOOR"] = "ON";}
				if (whichObject == "FONT5" and areAllTheseOn(result, List<int>({ 1, 4, 7 }), "FONT")) {result[whichObject] = "ON";result[whichObject + "FLOOR"] = "ON";}
				if (whichObject == "FONT3" and areAllTheseOn(result, List<int>({ 1, 4, 7, 5 }), "FONT")) {result[whichObject] = "ON";result[whichObject + "FLOOR"] = "ON";}
				if (whichObject == "FONT6" and areAllTheseOn(result, List<int>({ 1, 4, 7, 5, 3 }), "FONT")) { result[whichObject] = "ON"; result[whichObject + "FLOOR"] = "ON"; }
				if (whichObject == "FONT8" and areAllTheseOn(result, List<int>({ 1, 4, 7, 5, 3, 6 }), "FONT")) { result[whichObject] = "ON"; result[whichObject + "FLOOR"] = "ON"; }
				if (whichObject == "FONT9" and areAllTheseOn(result, List<int>({ 1, 4, 7, 5, 3, 6, 8 }), "FONT")) { result[whichObject] = "ON"; result[whichObject + "FLOOR"] = "ON"; }

				if (whichObject == "FONT10" and areAllTheseOn(result, { 1 }, "FONT")) { result[whichObject] = "ON"; result[whichObject + "FLOOR"] = "ON"; }
				if (whichObject == "FONT13" and areAllTheseOn(result, List<int>({ 1,10 }), "FONT")) { result[whichObject] = "ON"; result[whichObject + "FLOOR"] = "ON"; }
				if (whichObject == "FONT11" and areAllTheseOn(result, List<int>({ 1,10 }), "FONT")) { result[whichObject] = "ON"; result[whichObject + "FLOOR"] = "ON"; }
				if (whichObject == "FONT14" and areAllTheseOn(result, List<int>({ 1,10, 13, 11 }), "FONT")) { result[whichObject] = "ON"; result[whichObject + "FLOOR"] = "ON"; }
				if (whichObject == "FONT15" and areAllTheseOn(result, List<int>({ 1,10, 13, 11,14 }), "FONT")) { result[whichObject] = "ON"; result[whichObject + "FLOOR"] = "ON"; }
			}
			for (auto res : result.getKeys().internalList) {
				if (res.find("FONT") != -1 and res.find("FLOOR") == -1) {
					bool isOn = result[res] == "ON";
					saveContainer.current.flags["WATERPUZZLE1" + res] = isOn;
				}
			}
			saveContainer.save();
			return {result, result[whichObject]};
		}
		bool areAllTheseOn(Map<string, string> status, List<int> which, string keyword) {
			for (auto key : which.internalList) {
				string keyAsString = keyword + to_string(key);
				if (status[keyAsString] == "OFF") {
						return false;
					}
			}
			return true;
		}
		List<mapObject> getWaterPuzzle1(pair<float, float> position) {
			Map<string, string> status = loadCurrentWaterPuzzle1Status();
			List<mapObject> results;
			// distances = 0.45, 2.8
			float x = 4.55;
			float y = 2.8;
			mapObject puzzleStart;
			puzzleStart.name = "PUZZLESTARTFLOOR";
			puzzleStart.data["uniqueID"] = "PUZZLESTARTFLOOR";
			puzzleStart.anchor = "CENTRE";
			puzzleStart.positionOnMap = {position.first - 8.1, position.second};
			puzzleStart.canInteract = false;
			puzzleStart.visible = true;
			puzzleStart.obstruction = false;
			puzzleStart.layer = imageLookup.layerDefaults["PLAYER"] - 1;
			puzzleStart.imageSources = imageLookup.getSequenceAsString("WATERFONTSTARTFLOOR", status["WaterPuzzleActivated"] + "_FRONT");
			puzzleStart.animated = "1";
			puzzleStart.animationSpeed = 90;
			results.push_back(puzzleStart);
			/*			
						x
					x		x
				x		x		x
			x		x		x
				x		x		x	
					x		x

								9
					7		8
				4		5		6
			1		2		3
				10		11		12
					13		14
								15
			
			*/

			for (auto res : getFont("FONT1", "WATERFONTDUO", "RIGHT", status["FONT1"], {position.first - 8.1, position.second}).internalList) { results.push_back(res); }
			for (auto res : getFont("FONT2", "WATERFONTBROKEN", "FRONT", status["FONT2"], { position.first, position.second }).internalList) { results.push_back(res); }
			for (auto res : getFont("FONT3", "WATERFONTDUO", "RIGHT", status["FONT3"], { position.first + 8.1, position.second }).internalList) { results.push_back(res); }

			for (auto res : getFont("FONT4", "WATERFONTTRIS", "FRONT", status["FONT4"], { position.first - 4.05, position.second - 2.8 }).internalList) { results.push_back(res); }
			for (auto res : getFont("FONT5", "WATERFONTDUO", "FRONT", status["FONT5"], { position.first + 4.05, position.second - 2.8 }).internalList) { results.push_back(res); }
			for (auto res : getFont("FONT6", "WATERFONTDUO", "LEFT", status["FONT6"], { position.first + 12.15, position.second - 2.8 }).internalList) { results.push_back(res); }

			for (auto res : getFont("FONT7", "WATERFONTDUO", "FRONT", status["FONT7"], { position.first, position.second - 5.6 }).internalList) { results.push_back(res); }
			for (auto res : getFont("FONT8", "WATERFONTMONO", "BACK", status["FONT8"], { position.first + 8.1, position.second - 5.6 }).internalList) { results.push_back(res); }

			for (auto res : getFont("FONT9", "WATERFONTMONO", "FRONT", status["FONT9"], { position.first + 12.15, position.second - 8.4 }).internalList) { results.push_back(res); }

			for (auto res : getFont("FONT10", "WATERFONTDUO", "RIGHT", status["FONT10"], { position.first - 4.05, position.second + 2.8 }).internalList) { results.push_back(res); }
			for (auto res : getFont("FONT11", "WATERFONTDUO2", "FRONT", status["FONT11"], { position.first + 4.05, position.second + 2.8 }).internalList) { results.push_back(res); }
			for (auto res : getFont("FONT12", "WATERFONTBROKEN", "LEFT", status["FONT12"], { position.first + 12.15, position.second + 2.8 }).internalList) { results.push_back(res); }

			for (auto res : getFont("FONT13", "WATERFONTMONO", "BACK", status["FONT13"], { position.first, position.second + 5.6 }).internalList) { results.push_back(res); }
			for (auto res : getFont("FONT14", "WATERFONTMONO", "RIGHT", status["FONT14"], { position.first + 8.1, position.second + 5.6 }).internalList) { results.push_back(res); }
			for (auto res : getFont("FONT15", "WATERFONTSECRETEND", "FRONT", status["FONT15"], { position.first + 12.15, position.second + 8.4 }).internalList) { results.push_back(res); }


			return results;
		}
		List<mapObject> getFont(string uniqueID, string type, string direction, string status, pair<float, float> position) {
			List<mapFloor::triangle> fontTriangles = List<mapFloor::triangle>({ mapFloor::triangle({{20.29440402984619,15.218010544776917}, {-8.58709141612053,23.781627416610718}, {8.741944283246994,23.781627416610718}}),mapFloor::triangle({{19.940221309661865,2.893342450261116}, {-20.150218904018402,15.218007564544678}, {20.29440402984619,15.218010544776917}}),mapFloor::triangle({{19.940221309661865,2.893342450261116}, {-15.702112019062042,-0.04478186892811209}, {-19.808906316757202,2.8933385387063026}}), mapFloor::triangle({{20.29440402984619,15.218010544776917}, {-20.150218904018402,15.218007564544678}, {-8.58709141612053,23.781627416610718}}),mapFloor::triangle({{19.940221309661865,2.893342450261116}, {-19.808906316757202,2.8933385387063026}, {-20.150218904018402,15.218007564544678}}),mapFloor::triangle({{19.940221309661865,2.893342450261116}, {15.833427011966705,-0.0447787664597854}, {-15.702112019062042,-0.04478186892811209}}) });
			fontTriangles = mapObject::convertFrom100toX(fontTriangles, 0.03, position);
			mapObject result;
			result.name = uniqueID;
			result.data["uniqueID"] = uniqueID;
			result.canInteract = true;
			result.visible = true;
			result.layer = imageLookup.layerDefaults["PLAYER"];
			result.anchor = "CENTRE";
			result.positionOnMap = position;
			result.obstruction = true;
			result.triangles = fontTriangles;
			result.data["message"] = "$LANGUAGE$_Map Pop Up Text_Turn Handle";
			result.data["copy"] = uniqueID;
			result.data["imageID"] = uniqueID;
			result.data["colour"] = "WHITE";
			result.data["shadowColour"] = "BLACK";
			result.data["anchorStyle"] = "CENTRE";
			result.data["x"] = "0";
			result.data["y"] = "-10";
			result.data["h"] = "50";
			result.data["w"] = "50";
			result.data["format"] = "LightText_20";
			result.data["layer"] = to_string(imageLookup.layerDefaults["TEXTONMAP"]);
			result.data["uniqueID"] = "mappopuptextID";
			result.data["expandedInteractionX"] = "1.02";
			result.data["expandedInteractionY"] = "1.02";
			result.data["cutscene"] = "WaterPuzzle1RunPuzzle";
			result.data["character"] = type;
			result.data["direction"] = direction;
			result.animated = "1";
			result.animationSpeed = 90;
			result.imageSources = imageLookup.getSequenceAsString(type, status + "_" + direction );
			mapObject result2;
			result2.name = uniqueID + "FLOOR";
			result2.data["uniqueID"] = uniqueID + "FLOOR";
			result2.anchor = "CENTRE";
			result2.positionOnMap = position;
			result2.canInteract = false;
			result2.visible = true;
			result2.obstruction = false;
			result2.layer = imageLookup.layerDefaults["PLAYER"] - 1;
			result2.imageSources = imageLookup.getSequenceAsString(type + "FLOOR", status + "_" + direction);
			result2.animated = "1";
			result2.animationSpeed = 90;
			result2.data["character"] = type + "FLOOR";
			result2.data["direction"] = direction;
			return List<mapObject>({ result, result2 });
		}
		mapObject getFireSpigot(string uniqueID, pair<float, float> position) {
			List<mapFloor::triangle> triangles = List<mapFloor::triangle>({ mapFloor::triangle({{20.29440402984619,15.218010544776917}, {-8.58709141612053,23.781627416610718}, {8.741944283246994,23.781627416610718}}),mapFloor::triangle({{19.940221309661865,2.893342450261116}, {-20.150218904018402,15.218007564544678}, {20.29440402984619,15.218010544776917}}),mapFloor::triangle({{19.940221309661865,2.893342450261116}, {-15.702112019062042,-0.04478186892811209}, {-19.808906316757202,2.8933385387063026}}), mapFloor::triangle({{20.29440402984619,15.218010544776917}, {-20.150218904018402,15.218007564544678}, {-8.58709141612053,23.781627416610718}}),mapFloor::triangle({{19.940221309661865,2.893342450261116}, {-19.808906316757202,2.8933385387063026}, {-20.150218904018402,15.218007564544678}}),mapFloor::triangle({{19.940221309661865,2.893342450261116}, {15.833427011966705,-0.0447787664597854}, {-15.702112019062042,-0.04478186892811209}}) });
			triangles = mapObject::convertFrom100toX(triangles, 0.03, position);
			mapObject result;
			result.name = uniqueID;
			result.data["uniqueID"] = uniqueID;
			result.canInteract = true;
			result.visible = true;
			result.layer = imageLookup.layerDefaults["PLAYER"];
			result.anchor = "CENTRE";
			result.positionOnMap = position;
			result.obstruction = true;
			result.triangles = triangles;
			result.data["message"] = "$LANGUAGE$_Map Pop Up Text_Turn Handle";
			result.data["copy"] = uniqueID;
			result.data["imageID"] = uniqueID;
			result.data["colour"] = "WHITE";
			result.data["shadowColour"] = "BLACK";
			result.data["anchorStyle"] = "CENTRE";
			result.data["x"] = "0";
			result.data["y"] = "-10";
			result.data["h"] = "50";
			result.data["w"] = "50";
			result.data["format"] = "LightText_20";
			result.data["layer"] = to_string(imageLookup.layerDefaults["TEXTONMAP"]);
			result.data["uniqueID"] = "mappopuptextID";
			result.data["expandedInteractionX"] = "1.02";
			result.data["expandedInteractionY"] = "1.02";
			result.data["cutscene"] = uniqueID;
			result.data["character"] = "FireSpigot";
			result.data["direction"] = "STAND_FRONT";
			result.animated = "0";
			result.animationSpeed = 0;
			result.imageSources = imageLookup.getSequenceAsString("FireSpigot", "STAND_FRONT");
			return result;
		}
		Map<string, string> loadCurrentWaterPuzzle1Status() {
			Map<string, bool> flags; flags.internalMap = saveContainer.current.flags;
			Map<string, string> result;
			if (flags["WaterPuzzleActivated"]) {
				result["WaterPuzzleActivated"] = "ON";
			}
			else {
				result["WaterPuzzleActivated"] = "OFF";
			}
			for (int x = 1; x <= waterPuzzle1FontCount; x++) {
				result["FONT" + to_string(x)] = "OFF";
				result["FONT" + to_string(x) + "FLOOR"] = "OFF";
				if (flags["WATERPUZZLE1FONT" + to_string(x)]) {
					result["FONT" + to_string(x)] = "ON";
					result["FONT" + to_string(x) + "FLOOR"] = "ON";
				}
			}
			return result;
		}
		void runSpigotPuzzleLogic(string whichLever) {
			if (spigotPuzzleLevers[whichLever] == "OFF") {
				spigotPuzzleLevers[whichLever] = "ON";
			}
			else {
				spigotPuzzleLevers[whichLever] = "OFF";
			}
			Map<string, int> lookupTable = Map<string, int>({
				pair<string, int>({"1", 3}),
				pair<string, int>({"2", -2 }),
				pair<string, int>({"3", 6}),
				pair<string, int>({"4", 7 }),
				pair<string, int>({"5", -3}),
			});
			currentSpigotPuzzleValue = 0;
			for (auto x : spigotPuzzleLevers.getKeys().internalList) {
				if (spigotPuzzleLevers[x] == "ON") {
					currentSpigotPuzzleValue += lookupTable[x];
				}
			}
			if (currentSpigotPuzzleValue < 0) { currentSpigotPuzzleValue = 0; }
			if (currentSpigotPuzzleValue > 10) { currentSpigotPuzzleValue = 10; }
		}
		bool spigotPuzzleSolved() {
			return currentSpigotPuzzleValue == 5;
		}
		void resetSpigotPuzzle() {
			for (int x = 1; x < 6; x++) {
				spigotPuzzleLevers[to_string(x)] = "OFF";
			}
		}

		int waterPuzzle1FontCount = 15;
		int waterPuzzle1FontsAllowed = 8;
		Map<string, string> spigotPuzzleLevers;
		int currentSpigotPuzzleValue;
	};
	puzzles puzzleContainer;
	void defineAllMaps() {
		maps["RoadToBénouville"] = mapInstance("RoadToBénouville", MAP_DEBUG, { 45,8 /*51,55*/ }, List<mapObject>({
			mapObject("WilliamCombat", true, true, false, imageLookup.getSequenceAsString("White Knight", "STAND_BACK"), "1", 800, imageLookup.layerDefaults["PLAYER"], "2.0", "2.0","CENTRE", { 55, 87 }, false, List<mapFloor::triangle>({mapFloor::triangle({{59.13970470428467,84.17178392410278}, {52.62807011604309,89.39529061317444}, {52.72292494773865,84.57980155944824}}),mapFloor::triangle({{59.13970470428467,84.17178392410278}, {60.45340299606323,89.39529061317444}, {52.62807011604309,89.39529061317444}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Attack the Mad Rider"}),
										pair<string, string>({"copy","WilliamCombat"}),
										pair<string, string>({"cutscene","AttackMadRider"}),
										pair<string, string>({"x","5"}),
										pair<string, string>({"y","-5"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("don'tLoadIfNot", "WilliamQuestStarted"),
										pair<string, string>("don'tLoadIf", "WilliamDefeated"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
			mapObject("WilliamCombatShadow", true, true, false, imageLookup.getSequenceAsString("Shadow White Knight", "STAND_BACK"), "1", 800, imageLookup.layerDefaults["PLAYER"]-1, "2.0", "2.0","CENTRE", { 55, 87 }, false, List<mapFloor::triangle>({}), Map<string, string>({
					pair<string, string>("don'tLoadIfNot", "WilliamQuestStarted"),
					pair<string, string>("don'tLoadIf", "WilliamDefeated"),
				})),
			mapObject::getOnetimeTrigger("HorsemanCutscene1",List<mapFloor::triangle>({mapFloor::triangle({{47.42600917816162,9.98448133468628}, {48.882490396499634,6.590679287910461}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{48.918330669403076,10.155074298381805}, {50.9097695350647,6.964127719402313}, {50.94560980796814,10.528524219989777}}),mapFloor::triangle({{48.882490396499634,6.590679287910461}, {49.91276562213898,5.539841949939728}, {50.9097695350647,6.964127719402313}}),
mapFloor::triangle({{48.94671440124512,12.747283279895782}, {48.918330669403076,10.155074298381805}, {50.50344467163086,12.811049818992615}}),mapFloor::triangle({{50.46741962432861,15.510085225105286}, {50.50344467163086,12.811049818992615}, {52.088552713394165,15.467023849487305}}),mapFloor::triangle({{52.088552713394165,15.467023849487305}, {54.11888360977173,12.615998089313507}, {55.40493726730347,14.890195429325104}}),
mapFloor::triangle({{50.94560980796814,10.528524219989777}, {50.50344467163086,12.811049818992615}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{47.42600917816162,9.98448133468628}, {47.411930561065674,6.761273741722107}, {48.882490396499634,6.590679287910461}}),mapFloor::triangle({{48.918330669403076,10.155074298381805}, {48.882490396499634,6.590679287910461}, {50.9097695350647,6.964127719402313}}),
mapFloor::triangle({{48.882490396499634,6.590679287910461}, {48.882490396499634,5.7226985692977905}, {49.91276562213898,5.539841949939728}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {47.42600917816162,9.98448133468628}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{50.46741962432861,15.510085225105286}, {48.94671440124512,12.747283279895782}, {50.50344467163086,12.811049818992615}}),
mapFloor::triangle({{52.088552713394165,15.467023849487305}, {50.50344467163086,12.811049818992615}, {54.11888360977173,12.615998089313507}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {54.11888360977173,12.615998089313507}, {50.50344467163086,12.811049818992615}}),}), ""),
			mapObject("AT_EnterTown", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 55, 3 }, false, List<mapFloor::triangle>({mapFloor::triangle({{50.96829533576965,2.710600197315216}, {56.24496936798096,-0.025719404220581055}, {56.244683265686035,2.710600197315216}}),mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.96829533576965,2.710600197315216}, {56.244683265686035,2.710600197315216}}),mapFloor::triangle({{50.96829533576965,2.710600197315216}, {49.12346601486206,-0.025719404220581055}, {56.24496936798096,-0.025719404220581055}}),
mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.94001889228821,4.765757918357849}, {50.96829533576965,2.710600197315216}}),}), Map<string, string>({
											pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Bénouville"}),
											pair<string, string>({"copy","AT_EnterTown"}),
											pair<string, string>({"areaTransition","BénouvilleTown1"}),
											pair<string, string>({"direction","STAND_BACK"}),
											pair<string, string>({"playerPosX","62"}),
											pair<string, string>({"playerPosY","96"}),
											pair<string, string>({"audio","STONESTEPS"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","0"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>({"format","LightText_20"}),
			})),
			mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("TopLayer", false, true, false, imageLookup.getSequenceAsString("RoadToTownTop","ACTION_1"),"0",0,imageLookup.layerDefaults["ENVIRONMENT"] - 1,"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("Lamp1", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{60.5f, 20.0f}, false, {}, {}),
			mapObject("Lamp2", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{49.3, 29.7}, false, {}, {}),
			mapObject("Lamp3", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{34.9, 18.0}, false, {}, {}),
			mapObject("Lamp4", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{46.8, 13.7}, false, {}, {}),
			mapObject("Lamp5", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{49.2, 1.2}, false, {}, {}),
			mapObject("Well", false, true, false, imageLookup.getSequenceAsString("Well","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{42, 26.6}, false, {}, {}),
			mapObject("Lamp6", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{56.9, 1.2}, false, {}, {}),
			mapObject("DeadHorse", true, false, false, "","0",0,imageLookup.layerDefaults["SMALLOBJECTS"],"1.0","1.0","CENTRE",{54.6, 16.9}, false, List<mapFloor::triangle>({mapFloor::triangle({{50.72616934776306,23.629747331142426}, {56.28034472465515,16.637805104255676}, {50.594234466552734,16.637805104255676}}),mapFloor::triangle({{50.72616934776306,23.629747331142426}, {56.76738619804382,23.295582830905914}, {56.28034472465515,16.637805104255676}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Dead Horse"}),
										pair<string, string>({"copy","DeadHorse"}),
										pair<string, string>({"cutscene","DeadHorse"}),
										pair<string, string>({"x","0"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("DeadWoman", true, true, false, imageLookup.getSequenceAsString("DeadWoman1","STAND_FRONT"),"0",0,imageLookup.layerDefaults["SMALLOBJECTS"],"1.0","1.0","CENTRE",{44.7, 21}, false, List<mapFloor::triangle>({mapFloor::triangle({{43.252843618392944,23.397547006607056}, {45.84275484085083,18.938007950782776}, {41.23336672782898,20.89352011680603}}),mapFloor::triangle({{43.252843618392944,23.397547006607056}, {47.981709241867065,21.335013210773468}, {45.84275484085083,18.938007950782776}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Corpse of a female villager"}),
										pair<string, string>({"copy","DeadWoman"}),
										pair<string, string>({"cutscene","DeadWoman"}),
										pair<string, string>({"x","-10"}),
										pair<string, string>({"y","-10"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("LeaveBénouville", true, false, false, "","0",0,0,"1.0","1.0","CENTRE",{57, 96}, false, List<mapFloor::triangle>({mapFloor::triangle({{68.51502060890198,99.87432956695557}, {48.152658343315125,93.04946660995483}, {48.152658343315125,99.87432956695557}}),mapFloor::triangle({{68.51502060890198,99.87432956695557}, {68.51502060890198,93.04946660995483}, {48.152658343315125,93.04946660995483}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave Bénouville"}),
										pair<string, string>({"copy","LeaveBénouville"}),
										pair<string, string>({"cutscene","Leave Bénouville"}),
										pair<string, string>({"x","0"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"})})),
			mapObject("DeadMan", true, true, false, imageLookup.getSequenceAsString("DeadMan1", "STAND_FRONT"), "0", 0, imageLookup.layerDefaults["SMALLOBJECTS"], "1.0","1.0", "CENTRE", { 37.9, 26.1 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{33.55035483837128,27.531203627586365}, {41.157734394073486,24.36063140630722}, {33.66403877735138,24.066226184368134}}),mapFloor::triangle({{33.55035483837128,27.531203627586365}, {41.06677174568176,28.645145893096924}, {41.157734394073486,24.36063140630722}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Corpse of a male villager"}),
										pair<string, string>({"copy","DeadMan"}),
										pair<string, string>({"cutscene","DeadMan"}),
										pair<string, string>({"x","-10"}),
										pair<string, string>({"y","-10"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
			mapObject("Fog1", false, true, true, imageLookup.getSequenceAsString("Fog2","STAND_FRONT"),"1",2500,imageLookup.layerDefaults["ENVIRONMENT"] + 1,"1","1.0","CENTRE",{0, 0}, false, {}, {}),
			mapObject("OldBookManShadow", true, true, false, imageLookup.getSequenceAsString("Shadow OldBookMan", "STAND_FRONT"), "1", 800, imageLookup.layerDefaults["PLAYERSHADOW"], "1.0", "1.0","CENTRE", { 9, 37 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{12.475095689296722,35.404083132743835}, {6.834037601947784,39.8309051990509}, {7.403770089149475,35.25550961494446}}),mapFloor::triangle({{12.475095689296722,35.404083132743835}, {12.759986519813538,40.50284922122955}, {6.834037601947784,39.8309051990509}}),}), Map<string, string>({})),
			mapObject("OldBookMan", true, true, false, imageLookup.getSequenceAsString("OldBookMan", "STAND_FRONT"), "1", 800, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 9, 37 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{12.475095689296722,35.404083132743835}, {6.834037601947784,39.8309051990509}, {7.403770089149475,35.25550961494446}}),mapFloor::triangle({{12.475095689296722,35.404083132743835}, {12.759986519813538,40.50284922122955}, {6.834037601947784,39.8309051990509}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to OldBookMan"}),
										pair<string, string>({"copy","OldBookMan"}),
										pair<string, string>({"x","5"}),
										pair<string, string>({"y","-5"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("Merchant", "1"),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}) })),
			mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{36.2, 19.1}, false, List<mapFloor::triangle>({mapFloor::triangle({{37.298646569252014,20.028436183929443}, {35.527339577674866,21.976549923419952}, {35.056111216545105,20.607496798038483}}),mapFloor::triangle({{37.298646569252014,20.028436183929443}, {37.60534226894379,21.315868198871613}, {35.527339577674866,21.976549923419952}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter House"}),
										pair<string, string>({"copy","AT_Door"}),
										pair<string, string>({"areaTransition","House1Inside1"}),
										pair<string, string>({"direction","STAND_BACK"}),
										pair<string, string>({"playerPosX","46"}),
										pair<string, string>({"playerPosY","53"}),
										pair<string, string>({"audio","WOODENDOOR"}),
										pair<string, string>({"x","-5"}),
										pair<string, string>({"y","-10"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>({"format","LightText_20"}),
			})),
			}), List<mapFloor>({
			mapFloor("GrassFloor", List<mapFloor::triangle>({mapFloor::triangle({{43.571436405181885,100.0}, {50.17085075378418,43.96422207355499}, {42.67935752868652,43.96422207355499}}),mapFloor::triangle({{42.67935752868652,43.96422207355499}, {45.86067199707031,34.00440216064453}, {39.01277184486389,34.00440216064453}}),mapFloor::triangle({{39.01277184486389,34.00440216064453}, {45.009586215019226,23.523083329200745}, {38.076019287109375,23.014554381370544}}),
			mapFloor::triangle({{61.50498986244202,34.00440216064453}, {100.07518529891968,43.96422505378723}, {100.07518529891968,34.00440514087677}}),mapFloor::triangle({{45.86067199707031,34.00440216064453}, {52.96544432640076,43.96422207355499}, {61.50498986244202,34.00440216064453}}),mapFloor::triangle({{100.07518529891968,4.586407914757729}, {61.50498986244202,0.0}, {61.50498986244202,4.58640493452549}}),
		mapFloor::triangle({{61.50498986244202,20.45578807592392}, {100.07518529891968,4.586407914757729}, {61.50498986244202,4.58640493452549}}),mapFloor::triangle({{36.98071539402008,4.58640493452549}, {43.09951066970825,0.0}, {36.66390776634216,0.0}}),mapFloor::triangle({{44.76052224636078,20.45578807592392}, {61.50498986244202,4.58640493452549}, {43.47192645072937,4.58640493452549}}),
		mapFloor::triangle({{100.07518529891968,43.96422505378723}, {62.45462894439697,100.0}, {100.07518529891968,100.0}}),mapFloor::triangle({{45.86067199707031,34.00440216064453}, {61.50498986244202,23.523081839084625}, {45.009586215019226,23.523083329200745}}),mapFloor::triangle({{100.07518529891968,34.00440514087677}, {61.50498986244202,23.523081839084625}, {61.50498986244202,34.00440216064453}}),
		mapFloor::triangle({{38.076019287109375,23.014554381370544}, {44.76052224636078,20.45578807592392}, {37.005507946014404,20.4557865858078}}),mapFloor::triangle({{37.005507946014404,20.4557865858078}, {43.47192645072937,4.58640493452549}, {36.98071539402008,4.58640493452549}}),mapFloor::triangle({{45.009586215019226,23.523083329200745}, {61.50498986244202,20.45578807592392}, {44.76052224636078,20.45578807592392}}),
		mapFloor::triangle({{100.07518529891968,23.523084819316864}, {61.50498986244202,20.45578807592392}, {61.50498986244202,23.523081839084625}}),mapFloor::triangle({{0.0,20.45578807592392}, {34.178343415260315,4.58640493452549}, {0.0,4.58640493452549}}),mapFloor::triangle({{0.0,23.523081839084625}, {35.39458215236664,20.97281813621521}, {0.0,20.45578807592392}}),
		mapFloor::triangle({{0.0,4.58640493452549}, {33.885544538497925,0.0}, {0.0,0.0}}),mapFloor::triangle({{0.0,23.523081839084625}, {36.05641424655914,34.00440216064453}, {36.39649152755737,23.523083329200745}}),mapFloor::triangle({{0.0,43.96422207355499}, {36.05641424655914,34.00440216064453}, {0.0,34.00440216064453}}),
		mapFloor::triangle({{0.0,100.0}, {39.445146918296814,43.96422207355499}, {0.0,43.96422207355499}}),mapFloor::triangle({{34.178343415260315,4.58640493452549}, {37.005507946014404,20.4557865858078}, {36.98071539402008,4.58640493452549}}),mapFloor::triangle({{34.178343415260315,4.58640493452549}, {36.66390776634216,0.0}, {33.885544538497925,0.0}}),
		mapFloor::triangle({{36.39649152755737,23.523083329200745}, {39.01277184486389,34.00440216064453}, {38.076019287109375,23.014554381370544}}),mapFloor::triangle({{39.445146918296814,43.96422207355499}, {39.01277184486389,34.00440216064453}, {36.05641424655914,34.00440216064453}}),mapFloor::triangle({{40.269625186920166,100.0}, {42.67935752868652,43.96422207355499}, {39.445146918296814,43.96422207355499}}),
		mapFloor::triangle({{43.571436405181885,100.0}, {51.21951699256897,100.0}, {50.17085075378418,43.96422207355499}}),mapFloor::triangle({{42.67935752868652,43.96422207355499}, {50.17085075378418,43.96422207355499}, {45.86067199707031,34.00440216064453}}),mapFloor::triangle({{39.01277184486389,34.00440216064453}, {45.86067199707031,34.00440216064453}, {45.009586215019226,23.523083329200745}}),
		mapFloor::triangle({{61.50498986244202,34.00440216064453}, {52.96544432640076,43.96422207355499}, {100.07518529891968,43.96422505378723}}),mapFloor::triangle({{45.86067199707031,34.00440216064453}, {50.17085075378418,43.96422207355499}, {52.96544432640076,43.96422207355499}}),mapFloor::triangle({{100.07518529891968,4.586407914757729}, {100.07518529891968,2.9802322387695312e-06}, {61.50498986244202,0.0}}),
		mapFloor::triangle({{61.50498986244202,20.45578807592392}, {100.07518529891968,20.45578956604004}, {100.07518529891968,4.586407914757729}}),mapFloor::triangle({{36.98071539402008,4.58640493452549}, {43.47192645072937,4.58640493452549}, {43.09951066970825,0.0}}),mapFloor::triangle({{44.76052224636078,20.45578807592392}, {61.50498986244202,20.45578807592392}, {61.50498986244202,4.58640493452549}}),
		mapFloor::triangle({{100.07518529891968,43.96422505378723}, {52.96544432640076,43.96422207355499}, {62.45462894439697,100.0}}),mapFloor::triangle({{45.86067199707031,34.00440216064453}, {61.50498986244202,34.00440216064453}, {61.50498986244202,23.523081839084625}}),mapFloor::triangle({{100.07518529891968,34.00440514087677}, {100.07518529891968,23.523084819316864}, {61.50498986244202,23.523081839084625}}),
		mapFloor::triangle({{38.076019287109375,23.014554381370544}, {45.009586215019226,23.523083329200745}, {44.76052224636078,20.45578807592392}}),mapFloor::triangle({{37.005507946014404,20.4557865858078}, {44.76052224636078,20.45578807592392}, {43.47192645072937,4.58640493452549}}),mapFloor::triangle({{45.009586215019226,23.523083329200745}, {61.50498986244202,23.523081839084625}, {61.50498986244202,20.45578807592392}}),
		mapFloor::triangle({{100.07518529891968,23.523084819316864}, {100.07518529891968,20.45578956604004}, {61.50498986244202,20.45578807592392}}),mapFloor::triangle({{0.0,20.45578807592392}, {35.39458215236664,20.97281813621521}, {34.178343415260315,4.58640493452549}}),mapFloor::triangle({{0.0,23.523081839084625}, {36.39649152755737,23.523083329200745}, {35.39458215236664,20.97281813621521}}),
		mapFloor::triangle({{0.0,4.58640493452549}, {34.178343415260315,4.58640493452549}, {33.885544538497925,0.0}}),mapFloor::triangle({{0.0,23.523081839084625}, {0.0,34.00440216064453}, {36.05641424655914,34.00440216064453}}),mapFloor::triangle({{0.0,43.96422207355499}, {39.445146918296814,43.96422207355499}, {36.05641424655914,34.00440216064453}}),
		mapFloor::triangle({{0.0,100.0}, {40.269625186920166,100.0}, {39.445146918296814,43.96422207355499}}),mapFloor::triangle({{34.178343415260315,4.58640493452549}, {35.39458215236664,20.97281813621521}, {37.005507946014404,20.4557865858078}}),mapFloor::triangle({{34.178343415260315,4.58640493452549}, {36.98071539402008,4.58640493452549}, {36.66390776634216,0.0}}),
		mapFloor::triangle({{36.39649152755737,23.523083329200745}, {36.05641424655914,34.00440216064453}, {39.01277184486389,34.00440216064453}}),mapFloor::triangle({{39.445146918296814,43.96422207355499}, {42.67935752868652,43.96422207355499}, {39.01277184486389,34.00440216064453}}),mapFloor::triangle({{40.269625186920166,100.0}, {43.571436405181885,100.0}, {42.67935752868652,43.96422207355499}}),}),false, Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "GRASS"})})),
			mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{37.005507946014404,20.4557865858078}, {36.39649152755737,23.523083329200745}, {35.39458215236664,20.97281813621521}}),mapFloor::triangle({{37.005507946014404,20.4557865858078}, {38.076019287109375,23.014554381370544}, {36.39649152755737,23.523083329200745}}),}),
				false, Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "WOOD"})})),
			mapFloor("StoneFloor", List<mapFloor::triangle>({mapFloor::triangle({{43.47192645072937,4.58640493452549}, {61.50498986244202,0.0}, {43.09951066970825,0.0}}),
		mapFloor::triangle({{53.09876203536987,43.96422207355499}, {51.21951699256897,100.0}, {50.17085075378418,43.96422207355499}}),
		mapFloor::triangle({{43.47192645072937,4.58640493452549}, {61.50498986244202,4.58640493452549}, {61.50498986244202,0.0}}),
		mapFloor::triangle({{53.09876203536987,43.96422207355499}, {62.45462894439697,100.0}, {51.21951699256897,100.0}}),}), false, Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "STONE"})})),
			mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{56.409090757369995,84.11365747451782}, {48.95790219306946,99.01602268218994}, {58.32327604293823,99.01602268218994}}),mapFloor::triangle({{56.409090757369995,84.11365747451782}, {56.69816732406616,52.426278591156006}, {51.22690200805664,52.426278591156006}}),mapFloor::triangle({{51.22690200805664,43.17668974399567}, {53.78797650337219,46.73760533332825}, {56.69816732406616,43.17668974399567}}),
		mapFloor::triangle({{51.22690200805664,43.17668974399567}, {56.69816732406616,37.51395642757416}, {49.18171763420105,37.187260389328}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {20.253488421440125,41.367724537849426}, {21.099352836608887,45.86978852748871}}),mapFloor::triangle({{20.253488421440125,41.367724537849426}, {0.7176756858825684,45.86978852748871}, {21.099352836608887,45.86978852748871}}),
		mapFloor::triangle({{14.84365314245224,22.325602173805237}, {0.7176756858825684,18.91578882932663}, {0.7176756858825684,22.325602173805237}}),mapFloor::triangle({{0.7176756858825684,18.91578882932663}, {14.473368227481842,14.809629321098328}, {0.7176756858825684,14.809629321098328}}),mapFloor::triangle({{14.473368227481842,14.809629321098328}, {4.930481314659119,12.489937245845795}, {0.7176756858825684,14.809629321098328}}),
		mapFloor::triangle({{4.930481314659119,12.489937245845795}, {11.069545149803162,1.7711609601974487}, {4.930481314659119,1.7711609601974487}}),mapFloor::triangle({{25.638803839683533,12.295009940862656}, {25.365564227104187,16.636018455028534}, {30.643922090530396,15.309351682662964}}),mapFloor::triangle({{11.069545149803162,1.7711609601974487}, {15.852071344852448,10.81363558769226}, {14.69796895980835,1.7711624503135681}}),
	mapFloor::triangle({{0.7176756858825684,39.37466740608215}, {16.428416967391968,32.5550377368927}, {0.7176756858825684,32.55504071712494}}),mapFloor::triangle({{0.7176756858825684,32.55504071712494}, {14.412814378738403,25.735414028167725}, {0.7176756858825684,25.735414028167725}}),mapFloor::triangle({{35.928815603256226,22.28292226791382}, {37.00892925262451,20.508450269699097}, {37.574702501297,21.74929529428482}}),
	mapFloor::triangle({{14.412814378738403,25.735414028167725}, {0.7176756858825684,22.325602173805237}, {0.7176756858825684,25.735414028167725}}),mapFloor::triangle({{25.112950801849365,24.15255606174469}, {32.011452317237854,24.94175285100937}, {30.711272358894348,22.862792015075684}}),mapFloor::triangle({{32.011452317237854,24.94175285100937}, {35.283029079437256,29.562664031982422}, {34.35523211956024,23.0474054813385}}),
	mapFloor::triangle({{36.48173213005066,23.58163148164749}, {37.574702501297,21.74929529428482}, {38.14047574996948,22.990140318870544}}),mapFloor::triangle({{32.436954975128174,19.135470688343048}, {30.194365978240967,14.588676393032074}, {28.888151049613953,14.775575697422028}}),mapFloor::triangle({{35.928815603256226,22.28292226791382}, {36.48173213005066,23.58163148164749}, {35.10341942310333,22.988635301589966}}),
	mapFloor::triangle({{45.55982947349548,20.328429341316223}, {44.489115476608276,23.975835740566254}, {41.85015261173248,21.659284830093384}}),mapFloor::triangle({{30.329573154449463,19.46827918291092}, {28.888151049613953,14.775575697422028}, {26.02885067462921,15.719172358512878}}),mapFloor::triangle({{40.79534113407135,26.822954416275024}, {39.365559816360474,30.083242058753967}, {35.283029079437256,29.562664031982422}}),
	mapFloor::triangle({{41.15120470523834,27.904421091079712}, {43.14132332801819,30.57653307914734}, {39.365559816360474,30.083242058753967}}),mapFloor::triangle({{43.14132332801819,30.57653307914734}, {43.19913685321808,27.694016695022583}, {45.69516181945801,29.04760241508484}}),mapFloor::triangle({{45.69516181945801,29.04760241508484}, {43.17961633205414,26.720523834228516}, {46.51808142662048,25.763723254203796}}),
	mapFloor::triangle({{41.85015261173248,21.659284830093384}, {41.1347359418869,26.018506288528442}, {38.14047574996948,22.990140318870544}}),mapFloor::triangle({{42.80505180358887,26.06889009475708}, {44.489115476608276,23.975835740566254}, {45.49221396446228,24.859747290611267}}),mapFloor::triangle({{43.17961633205414,26.720523834228516}, {45.49221396446228,24.859747290611267}, {46.51808142662048,25.763723254203796}}),
	mapFloor::triangle({{52.160751819610596,19.51773315668106}, {53.209251165390015,23.281770944595337}, {53.05097699165344,23.289769887924194}}),mapFloor::triangle({{55.63579201698303,18.122999370098114}, {59.937602281570435,20.150715112686157}, {55.63579201698303,19.27337348461151}}),mapFloor::triangle({{55.63579201698303,19.27337348461151}, {61.40407919883728,22.59443998336792}, {53.209251165390015,23.281770944595337}}),
	mapFloor::triangle({{67.4842119216919,28.42770516872406}, {50.79437494277954,32.81976878643036}, {51.82164907455444,29.17156219482422}}),mapFloor::triangle({{53.209251165390015,23.281770944595337}, {61.40369772911072,25.52335262298584}, {52.84892916679382,25.52335262298584}}),mapFloor::triangle({{61.40369772911072,25.52335262298584}, {51.82164907455444,29.17156219482422}, {52.84892916679382,25.52335262298584}}),
	mapFloor::triangle({{45.75563967227936,43.17668974399567}, {49.18171763420105,37.187260389328}, {45.75563967227936,37.038350105285645}}),mapFloor::triangle({{51.22690200805664,46.73760533332825}, {45.75563967227936,43.17668974399567}, {45.75563967227936,46.73760533332825}}),mapFloor::triangle({{47.866201400756836,84.11365747451782}, {51.22690200805664,52.426278591156006}, {45.75563967227936,52.426278591156006}}),
	mapFloor::triangle({{64.95198011398315,84.11365747451782}, {58.32327604293823,99.01602268218994}, {67.68865585327148,99.01602268218994}}),mapFloor::triangle({{50.79437494277954,32.81976878643036}, {56.69816732406616,37.51395642757416}, {49.18171763420105,37.187260389328}}),mapFloor::triangle({{66.43429398536682,42.67500340938568}, {56.69816732406616,37.51395642757416}, {56.69816732406616,43.17668974399567}}),
	mapFloor::triangle({{68.94510388374329,32.81976878643036}, {72.21961617469788,27.64536738395691}, {73.74469041824341,32.81976878643036}}),mapFloor::triangle({{68.94510388374329,32.81976878643036}, {71.88587784767151,37.955498695373535}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{51.98812484741211,18.27288717031479}, {45.55982947349548,20.328429341316223}, {44.63401734828949,18.92685443162918}}),
	mapFloor::triangle({{47.84349203109741,22.889067232608795}, {52.160751819610596,19.51773315668106}, {53.05097699165344,23.289769887924194}}),mapFloor::triangle({{48.992958664894104,25.7653146982193}, {51.104480028152466,28.884437680244446}, {47.402188181877136,29.858967661857605}}),mapFloor::triangle({{46.51808142662048,25.763723254203796}, {47.402188181877136,29.858967661857605}, {45.69516181945801,29.04760241508484}}),
	mapFloor::triangle({{47.84349203109741,22.889067232608795}, {45.49221396446228,24.859747290611267}, {44.489115476608276,23.975835740566254}}),mapFloor::triangle({{46.51808142662048,25.763723254203796}, {48.41822385787964,24.327191710472107}, {48.992958664894104,25.7653146982193}}),mapFloor::triangle({{53.05097699165344,23.289769887924194}, {48.41822385787964,24.327191710472107}, {47.84349203109741,22.889067232608795}}),
	mapFloor::triangle({{52.20814347267151,25.766906142234802}, {48.41822385787964,24.327191710472107}, {52.76750922203064,24.20814037322998}}),mapFloor::triangle({{43.14132332801819,30.57653307914734}, {39.365559816360474,32.3360949754715}, {39.365559816360474,30.083242058753967}}),mapFloor::triangle({{35.283029079437256,29.562664031982422}, {39.365559816360474,32.3360949754715}, {39.365559816360474,30.083242058753967}}),
	mapFloor::triangle({{43.14132332801819,32.82938599586487}, {46.023595333099365,32.59912431240082}, {46.11753821372986,33.69433879852295}}),mapFloor::triangle({{46.023595333099365,32.59912431240082}, {47.69207239151001,33.86508226394653}, {46.11753821372986,33.69433879852295}}),mapFloor::triangle({{44.24490034580231,11.806239932775497}, {41.35455787181854,4.575447738170624}, {47.04160690307617,4.761527478694916}}),
	mapFloor::triangle({{36.58044934272766,13.886196911334991}, {41.35455787181854,4.575447738170624}, {33.14061164855957,6.1720110476017}}),mapFloor::triangle({{25.638803839683533,12.295009940862656}, {31.578153371810913,13.562016189098358}, {28.182879090309143,8.703697472810745}}),mapFloor::triangle({{31.578153371810913,13.562016189098358}, {33.14061164855957,6.1720110476017}, {28.182879090309143,8.703697472810745}}),
	mapFloor::triangle({{47.04160690307617,4.761527478694916}, {47.42600917816162,9.98448133468628}, {44.24490034580231,11.806239932775497}}),mapFloor::triangle({{47.42600917816162,9.98448133468628}, {48.67419898509979,6.1746396124362946}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{48.918330669403076,10.155074298381805}, {50.9097695350647,6.964127719402313}, {50.94560980796814,10.528524219989777}}),
	mapFloor::triangle({{48.67419898509979,6.1746396124362946}, {49.91276562213898,5.539841949939728}, {50.9097695350647,6.964127719402313}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {56.71147704124451,6.964127719402313}, {56.74731731414795,10.528524219989777}}),mapFloor::triangle({{50.96829533576965,2.710600197315216}, {56.24496936798096,-0.025719404220581055}, {56.244683265686035,2.710600197315216}}),
	mapFloor::triangle({{50.9097695350647,6.964127719402313}, {56.244462728500366,4.765757918357849}, {56.71147704124451,6.964127719402313}}),mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.96829533576965,2.710600197315216}, {56.244683265686035,2.710600197315216}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {48.918330669403076,10.155074298381805}, {50.50344467163086,12.811049818992615}}),
	mapFloor::triangle({{50.46741962432861,15.510085225105286}, {50.50344467163086,12.811049818992615}, {52.088552713394165,15.467023849487305}}),mapFloor::triangle({{47.42600917816162,9.98448133468628}, {44.582581520080566,13.52628767490387}, {44.24490034580231,11.806239932775497}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {44.60830092430115,16.226571798324585}, {44.582581520080566,13.52628767490387}}),
	mapFloor::triangle({{52.088552713394165,15.467023849487305}, {54.11888360977173,12.615998089313507}, {55.40493726730347,14.890195429325104}}),mapFloor::triangle({{55.40493726730347,14.890195429325104}, {57.7343225479126,12.42094561457634}, {58.72132182121277,14.313367009162903}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {50.50344467163086,12.811049818992615}, {48.918330669403076,10.155074298381805}}),
	mapFloor::triangle({{56.74731731414795,10.528524219989777}, {54.11888360977173,12.615998089313507}, {50.94560980796814,10.528524219989777}}),mapFloor::triangle({{57.7343225479126,12.42094561457634}, {65.53108096122742,10.528524219989777}, {65.53108096122742,12.42094561457634}}),mapFloor::triangle({{58.72132182121277,14.313367009162903}, {65.53108096122742,12.42094561457634}, {65.53108096122742,14.313367009162903}}),
	mapFloor::triangle({{56.74731731414795,10.528524219989777}, {65.53108096122742,6.964127719402313}, {65.53108096122742,10.528524219989777}}),mapFloor::triangle({{65.53108096122742,4.751096665859222}, {56.71147704124451,6.964127719402313}, {56.244462728500366,4.765757918357849}}),mapFloor::triangle({{30.711272358894348,22.862792015075684}, {34.35523211956024,23.0474054813385}, {34.04190540313721,21.865569055080414}}),
	mapFloor::triangle({{35.10341942310333,22.988635301589966}, {36.48173213005066,23.58163148164749}, {34.35523211956024,23.0474054813385}}),mapFloor::triangle({{51.22690200805664,52.426278591156006}, {45.75563967227936,50.29851794242859}, {45.75563967227936,52.426278591156006}}),mapFloor::triangle({{56.69816732406616,52.426278591156006}, {51.22690200805664,50.29851794242859}, {51.22690200805664,52.426278591156006}}),
	mapFloor::triangle({{45.75563967227936,47.924575209617615}, {51.22690200805664,46.73760533332825}, {45.75563967227936,46.73760533332825}}),mapFloor::triangle({{53.47152352333069,47.92457818984985}, {51.22690200805664,46.73760533332825}, {51.22690200805664,47.92457818984985}}),mapFloor::triangle({{56.69816732406616,50.29851794242859}, {51.22690200805664,49.11154806613922}, {51.22690200805664,50.29851794242859}}),
	mapFloor::triangle({{45.75563967227936,50.29851794242859}, {51.22690200805664,49.11154806613922}, {45.75563967227936,49.11154508590698}}),mapFloor::triangle({{48.7857311964035,50.94045400619507}, {53.550052642822266,46.095672249794006}, {48.7857311964035,46.095672249794006}}),mapFloor::triangle({{53.78797650337219,46.73760533332825}, {56.66423439979553,46.73760533332825}, {56.69816732406616,43.17668974399567}}),
	mapFloor::triangle({{51.22690200805664,49.11154806613922}, {56.69816732406616,47.92904555797577}, {51.22690200805664,47.92457818984985}}),mapFloor::triangle({{39.17883038520813,25.58600902557373}, {38.14047574996948,22.990140318870544}, {39.74739611148834,25.532087683677673}}),mapFloor::triangle({{37.376514077186584,25.61688721179962}, {36.48173213005066,23.58163148164749}, {39.17883038520813,25.58600902557373}}),
	mapFloor::triangle({{37.961217761039734,26.580342650413513}, {40.79534113407135,26.822954416275024}, {35.283029079437256,29.562664031982422}}),mapFloor::triangle({{39.74739611148834,25.532087683677673}, {41.1347359418869,26.018506288528442}, {40.088462829589844,26.320040225982666}}),mapFloor::triangle({{40.088462829589844,26.320040225982666}, {40.79534113407135,26.822954416275024}, {39.93552327156067,26.437121629714966}}),
	mapFloor::triangle({{35.05798876285553,26.265668869018555}, {34.35523211956024,23.0474054813385}, {34.95745658874512,25.559699535369873}}),mapFloor::triangle({{37.376514077186584,25.61688721179962}, {34.95745658874512,25.559699535369873}, {34.35523211956024,23.0474054813385}}),mapFloor::triangle({{35.05798876285553,26.265668869018555}, {37.961217761039734,26.580342650413513}, {35.283029079437256,29.562664031982422}}),
	mapFloor::triangle({{30.711272358894348,22.862792015075684}, {32.436954975128174,19.135470688343048}, {30.329573154449463,19.46827918291092}}),mapFloor::triangle({{34.36081111431122,21.524284780025482}, {32.436954975128174,19.135470688343048}, {34.04190540313721,21.865569055080414}}),mapFloor::triangle({{26.02885067462921,15.719172358512878}, {24.616236984729767,20.074045658111572}, {30.329573154449463,19.46827918291092}}),
	mapFloor::triangle({{56.409090757369995,84.11365747451782}, {47.866201400756836,84.11365747451782}, {48.95790219306946,99.01602268218994}}),mapFloor::triangle({{56.409090757369995,84.11365747451782}, {64.95198011398315,84.11365747451782}, {56.69816732406616,52.426278591156006}}),mapFloor::triangle({{51.22690200805664,43.17668974399567}, {51.22690200805664,46.73760533332825}, {53.78797650337219,46.73760533332825}}),
	mapFloor::triangle({{51.22690200805664,43.17668974399567}, {56.69816732406616,43.17668974399567}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {45.75563967227936,37.038350105285645}, {20.253488421440125,41.367724537849426}}),mapFloor::triangle({{20.253488421440125,41.367724537849426}, {0.7176756858825684,39.37466740608215}, {0.7176756858825684,45.86978852748871}}),
	mapFloor::triangle({{14.84365314245224,22.325602173805237}, {15.379007160663605,18.625155091285706}, {0.7176756858825684,18.91578882932663}}),mapFloor::triangle({{0.7176756858825684,18.91578882932663}, {15.379007160663605,18.625155091285706}, {14.473368227481842,14.809629321098328}}),mapFloor::triangle({{14.473368227481842,14.809629321098328}, {13.1720170378685,12.489937245845795}, {4.930481314659119,12.489937245845795}}),
	mapFloor::triangle({{4.930481314659119,12.489937245845795}, {13.1720170378685,12.489937245845795}, {11.069545149803162,1.7711609601974487}}),mapFloor::triangle({{25.638803839683533,12.295009940862656}, {22.91545867919922,14.052563905715942}, {25.365564227104187,16.636018455028534}}),mapFloor::triangle({{11.069545149803162,1.7711609601974487}, {13.1720170378685,12.489937245845795}, {15.852071344852448,10.81363558769226}}),
	mapFloor::triangle({{0.7176756858825684,39.37466740608215}, {20.253488421440125,41.367724537849426}, {16.428416967391968,32.5550377368927}}),mapFloor::triangle({{0.7176756858825684,32.55504071712494}, {16.428416967391968,32.5550377368927}, {14.412814378738403,25.735414028167725}}),mapFloor::triangle({{35.928815603256226,22.28292226791382}, {35.37590205669403,20.984214544296265}, {37.00892925262451,20.508450269699097}}),
	mapFloor::triangle({{14.412814378738403,25.735414028167725}, {14.84365314245224,22.325602173805237}, {0.7176756858825684,22.325602173805237}}),mapFloor::triangle({{25.112950801849365,24.15255606174469}, {33.026671409606934,30.587339401245117}, {32.011452317237854,24.94175285100937}}),mapFloor::triangle({{32.011452317237854,24.94175285100937}, {33.026671409606934,30.587339401245117}, {35.283029079437256,29.562664031982422}}),
	mapFloor::triangle({{36.48173213005066,23.58163148164749}, {35.928815603256226,22.28292226791382}, {37.574702501297,21.74929529428482}}),mapFloor::triangle({{32.436954975128174,19.135470688343048}, {32.794931530952454,18.85164976119995}, {30.194365978240967,14.588676393032074}}),mapFloor::triangle({{45.55982947349548,20.328429341316223}, {47.84349203109741,22.889067232608795}, {44.489115476608276,23.975835740566254}}),
	mapFloor::triangle({{30.329573154449463,19.46827918291092}, {32.436954975128174,19.135470688343048}, {28.888151049613953,14.775575697422028}}),mapFloor::triangle({{40.79534113407135,26.822954416275024}, {41.15120470523834,27.904421091079712}, {39.365559816360474,30.083242058753967}}),mapFloor::triangle({{41.15120470523834,27.904421091079712}, {42.646223306655884,28.02833616733551}, {43.14132332801819,30.57653307914734}}),
	mapFloor::triangle({{43.14132332801819,30.57653307914734}, {42.646223306655884,28.02833616733551}, {43.19913685321808,27.694016695022583}}),mapFloor::triangle({{45.69516181945801,29.04760241508484}, {43.19913685321808,27.694016695022583}, {43.17961633205414,26.720523834228516}}),mapFloor::triangle({{41.85015261173248,21.659284830093384}, {44.489115476608276,23.975835740566254}, {41.1347359418869,26.018506288528442}}),
	mapFloor::triangle({{42.80505180358887,26.06889009475708}, {41.1347359418869,26.018506288528442}, {44.489115476608276,23.975835740566254}}),mapFloor::triangle({{43.17961633205414,26.720523834228516}, {42.80505180358887,26.06889009475708}, {45.49221396446228,24.859747290611267}}),mapFloor::triangle({{52.160751819610596,19.51773315668106}, {55.63579201698303,19.27337348461151}, {53.209251165390015,23.281770944595337}}),
	mapFloor::triangle({{55.63579201698303,18.122999370098114}, {61.51499152183533,18.122999370098114}, {59.937602281570435,20.150715112686157}}),mapFloor::triangle({{55.63579201698303,19.27337348461151}, {59.937602281570435,20.150715112686157}, {61.40407919883728,22.59443998336792}}),mapFloor::triangle({{67.4842119216919,28.42770516872406}, {68.94510388374329,32.81976878643036}, {50.79437494277954,32.81976878643036}}),
	mapFloor::triangle({{53.209251165390015,23.281770944595337}, {61.40407919883728,22.59443998336792}, {61.40369772911072,25.52335262298584}}),mapFloor::triangle({{61.40369772911072,25.52335262298584}, {67.4842119216919,28.42770516872406}, {51.82164907455444,29.17156219482422}}),mapFloor::triangle({{45.75563967227936,43.17668974399567}, {51.22690200805664,43.17668974399567}, {49.18171763420105,37.187260389328}}),
	mapFloor::triangle({{51.22690200805664,46.73760533332825}, {51.22690200805664,43.17668974399567}, {45.75563967227936,43.17668974399567}}),mapFloor::triangle({{47.866201400756836,84.11365747451782}, {56.409090757369995,84.11365747451782}, {51.22690200805664,52.426278591156006}}),mapFloor::triangle({{64.95198011398315,84.11365747451782}, {56.409090757369995,84.11365747451782}, {58.32327604293823,99.01602268218994}}),
	mapFloor::triangle({{50.79437494277954,32.81976878643036}, {68.94510388374329,32.81976878643036}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{66.43429398536682,42.67500340938568}, {71.88587784767151,37.955498695373535}, {56.69816732406616,37.51395642757416}}),mapFloor::triangle({{68.94510388374329,32.81976878643036}, {67.4842119216919,28.42770516872406}, {72.21961617469788,27.64536738395691}}),
	mapFloor::triangle({{68.94510388374329,32.81976878643036}, {73.74469041824341,32.81976878643036}, {71.88587784767151,37.955498695373535}}),mapFloor::triangle({{51.98812484741211,18.27288717031479}, {52.160751819610596,19.51773315668106}, {45.55982947349548,20.328429341316223}}),mapFloor::triangle({{47.84349203109741,22.889067232608795}, {45.55982947349548,20.328429341316223}, {52.160751819610596,19.51773315668106}}),
	mapFloor::triangle({{48.992958664894104,25.7653146982193}, {52.20814347267151,25.766906142234802}, {51.104480028152466,28.884437680244446}}),mapFloor::triangle({{46.51808142662048,25.763723254203796}, {48.992958664894104,25.7653146982193}, {47.402188181877136,29.858967661857605}}),mapFloor::triangle({{47.84349203109741,22.889067232608795}, {48.41822385787964,24.327191710472107}, {45.49221396446228,24.859747290611267}}),
	mapFloor::triangle({{46.51808142662048,25.763723254203796}, {45.49221396446228,24.859747290611267}, {48.41822385787964,24.327191710472107}}),mapFloor::triangle({{53.05097699165344,23.289769887924194}, {52.76750922203064,24.20814037322998}, {48.41822385787964,24.327191710472107}}),mapFloor::triangle({{52.20814347267151,25.766906142234802}, {48.992958664894104,25.7653146982193}, {48.41822385787964,24.327191710472107}}),
	mapFloor::triangle({{43.14132332801819,30.57653307914734}, {43.14132332801819,32.82938599586487}, {39.365559816360474,32.3360949754715}}),mapFloor::triangle({{35.283029079437256,29.562664031982422}, {33.026671409606934,30.587339401245117}, {39.365559816360474,32.3360949754715}}),mapFloor::triangle({{43.14132332801819,32.82938599586487}, {43.14132332801819,30.57653307914734}, {46.023595333099365,32.59912431240082}}),
	mapFloor::triangle({{46.023595333099365,32.59912431240082}, {47.69207239151001,31.58455491065979}, {47.69207239151001,33.86508226394653}}),mapFloor::triangle({{44.24490034580231,11.806239932775497}, {42.1939343214035,12.28756457567215}, {41.35455787181854,4.575447738170624}}),mapFloor::triangle({{36.58044934272766,13.886196911334991}, {42.1939343214035,12.28756457567215}, {41.35455787181854,4.575447738170624}}),
	mapFloor::triangle({{25.638803839683533,12.295009940862656}, {30.643922090530396,15.309351682662964}, {31.578153371810913,13.562016189098358}}),mapFloor::triangle({{31.578153371810913,13.562016189098358}, {36.58044934272766,13.886196911334991}, {33.14061164855957,6.1720110476017}}),mapFloor::triangle({{47.04160690307617,4.761527478694916}, {47.195008397102356,6.11434206366539}, {47.42600917816162,9.98448133468628}}),
	mapFloor::triangle({{47.42600917816162,9.98448133468628}, {47.195008397102356,6.11434206366539}, {48.67419898509979,6.1746396124362946}}),mapFloor::triangle({{48.918330669403076,10.155074298381805}, {48.67419898509979,6.1746396124362946}, {50.9097695350647,6.964127719402313}}),mapFloor::triangle({{48.67419898509979,6.1746396124362946}, {48.882490396499634,5.7226985692977905}, {49.91276562213898,5.539841949939728}}),
	mapFloor::triangle({{50.94560980796814,10.528524219989777}, {50.9097695350647,6.964127719402313}, {56.71147704124451,6.964127719402313}}),mapFloor::triangle({{50.96829533576965,2.710600197315216}, {49.12346601486206,-0.025719404220581055}, {56.24496936798096,-0.025719404220581055}}),mapFloor::triangle({{50.9097695350647,6.964127719402313}, {50.94001889228821,4.765757918357849}, {56.244462728500366,4.765757918357849}}),
	mapFloor::triangle({{56.244462728500366,4.765757918357849}, {50.94001889228821,4.765757918357849}, {50.96829533576965,2.710600197315216}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {47.42600917816162,9.98448133468628}, {48.918330669403076,10.155074298381805}}),mapFloor::triangle({{50.46741962432861,15.510085225105286}, {48.94671440124512,12.747283279895782}, {50.50344467163086,12.811049818992615}}),
	mapFloor::triangle({{47.42600917816162,9.98448133468628}, {48.94671440124512,12.747283279895782}, {44.582581520080566,13.52628767490387}}),mapFloor::triangle({{48.94671440124512,12.747283279895782}, {50.46741962432861,15.510085225105286}, {44.60830092430115,16.226571798324585}}),mapFloor::triangle({{52.088552713394165,15.467023849487305}, {50.50344467163086,12.811049818992615}, {54.11888360977173,12.615998089313507}}),
	mapFloor::triangle({{55.40493726730347,14.890195429325104}, {54.11888360977173,12.615998089313507}, {57.7343225479126,12.42094561457634}}),mapFloor::triangle({{50.94560980796814,10.528524219989777}, {54.11888360977173,12.615998089313507}, {50.50344467163086,12.811049818992615}}),mapFloor::triangle({{56.74731731414795,10.528524219989777}, {57.7343225479126,12.42094561457634}, {54.11888360977173,12.615998089313507}}),
	mapFloor::triangle({{57.7343225479126,12.42094561457634}, {56.74731731414795,10.528524219989777}, {65.53108096122742,10.528524219989777}}),mapFloor::triangle({{58.72132182121277,14.313367009162903}, {57.7343225479126,12.42094561457634}, {65.53108096122742,12.42094561457634}}),mapFloor::triangle({{56.74731731414795,10.528524219989777}, {56.71147704124451,6.964127719402313}, {65.53108096122742,6.964127719402313}}),
	mapFloor::triangle({{65.53108096122742,4.751096665859222}, {65.53108096122742,6.964127719402313}, {56.71147704124451,6.964127719402313}}),mapFloor::triangle({{30.711272358894348,22.862792015075684}, {32.011452317237854,24.94175285100937}, {34.35523211956024,23.0474054813385}}),mapFloor::triangle({{51.22690200805664,52.426278591156006}, {51.22690200805664,50.29851794242859}, {45.75563967227936,50.29851794242859}}),
	mapFloor::triangle({{56.69816732406616,52.426278591156006}, {56.69816732406616,50.29851794242859}, {51.22690200805664,50.29851794242859}}),mapFloor::triangle({{45.75563967227936,47.924575209617615}, {51.22690200805664,47.92457818984985}, {51.22690200805664,46.73760533332825}}),mapFloor::triangle({{53.47152352333069,47.92457818984985}, {53.78797650337219,46.73760533332825}, {51.22690200805664,46.73760533332825}}),
	mapFloor::triangle({{56.69816732406616,50.29851794242859}, {56.69816732406616,49.11154806613922}, {51.22690200805664,49.11154806613922}}),mapFloor::triangle({{45.75563967227936,50.29851794242859}, {51.22690200805664,50.29851794242859}, {51.22690200805664,49.11154806613922}}),mapFloor::triangle({{48.7857311964035,50.94045400619507}, {53.550052642822266,50.94045400619507}, {53.550052642822266,46.095672249794006}}),
	mapFloor::triangle({{51.22690200805664,49.11154806613922}, {56.69816732406616,49.11154806613922}, {56.69816732406616,47.92904555797577}}),mapFloor::triangle({{39.17883038520813,25.58600902557373}, {36.48173213005066,23.58163148164749}, {38.14047574996948,22.990140318870544}}),mapFloor::triangle({{37.376514077186584,25.61688721179962}, {34.35523211956024,23.0474054813385}, {36.48173213005066,23.58163148164749}}),
	mapFloor::triangle({{37.961217761039734,26.580342650413513}, {39.93552327156067,26.437121629714966}, {40.79534113407135,26.822954416275024}}),mapFloor::triangle({{39.74739611148834,25.532087683677673}, {38.14047574996948,22.990140318870544}, {41.1347359418869,26.018506288528442}}),mapFloor::triangle({{40.088462829589844,26.320040225982666}, {41.1347359418869,26.018506288528442}, {40.79534113407135,26.822954416275024}}),
	mapFloor::triangle({{35.05798876285553,26.265668869018555}, {35.283029079437256,29.562664031982422}, {34.35523211956024,23.0474054813385}}),mapFloor::triangle({{30.711272358894348,22.862792015075684}, {34.04190540313721,21.865569055080414}, {32.436954975128174,19.135470688343048}}),mapFloor::triangle({{34.36081111431122,21.524284780025482}, {32.794931530952454,18.85164976119995}, {32.436954975128174,19.135470688343048}}),
	mapFloor::triangle({{26.02885067462921,15.719172358512878}, {24.694694578647614,15.66227376461029}, {24.616236984729767,20.074045658111572}}),}), true, Map<string, string>({})),
			mapFloor("Trans", List<mapFloor::triangle>({mapFloor::triangle({{43.38904917240143,7.563062757253647}, {31.48369789123535,15.224528312683105}, {44.944414496421814,16.091090440750122}}),mapFloor::triangle({{46.79390788078308,13.37292343378067}, {45.05474865436554,16.049426794052124}, {44.831475615501404,12.381072342395782}}),mapFloor::triangle({{43.38904917240143,7.563062757253647}, {31.43555521965027,11.303985118865967}, {31.48369789123535,15.224528312683105}}),
	mapFloor::triangle({{46.79390788078308,13.37292343378067}, {46.32799029350281,15.946558117866516}, {45.05474865436554,16.049426794052124}}),}), false, Map<string, string>({{"trans","0"}})),
				}), {}, { 5000, 5000 }, Map<string, string>({
				pair<string,string>({"song1",to_string(WINDOUTSIDE1_WAV) + " " + "AmbienceVolume"}),
				pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
					}));
		maps["House1Inside1"] = mapInstance("House1Inside1", EMPTYMAP, { 46,53 }, List<mapObject>({
										mapObject("House1Inside1AsObject", false, true, false, imageLookup.getSequenceAsString("House1Inside1AsObject","STAND_FRONT"),"0",0,imageLookup.layerDefaults["Map"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
										mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
										mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{46, 54}, false, List<mapFloor::triangle>({mapFloor::triangle({{44.78374421596527,54.089611768722534}, {46.707287430763245,51.44556760787964}, {47.589874267578125,53.23103070259094}}),mapFloor::triangle({{44.78374421596527,54.089611768722534}, {43.931591510772705,52.344727516174316}, {46.707287430763245,51.44556760787964}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave House"}),
																		pair<string, string>({"copy","AT_Door"}),
																		pair<string, string>({"areaTransition","RoadToBénouville"}),
																		pair<string, string>({"direction","STAND_FRONT"}),
																		pair<string, string>({"playerPosX","36.4"}),
																		pair<string, string>({"playerPosY","21"}),
																		pair<string, string>({"audio","WOODENDOOR"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"}),
											})),
										mapObject("AT_Stairs", true, false, false, "","0",0,0,"1","1.0","CENTRE",{49, 46}, false, List<mapFloor::triangle>({mapFloor::triangle({{47.162121534347534,46.98895812034607}, {52.725422382354736,43.473583459854126}, {47.90157377719879,48.25414419174194}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {51.98596715927124,42.42135286331177}, {52.725422382354736,43.473583459854126}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Upstairs"}),
																		pair<string, string>({"copy","AT_Stairs"}),
																		pair<string, string>({"areaTransition","House1Inside2"}),
																		pair<string, string>({"direction","STAND_FRONT"}),
																		pair<string, string>({"playerPosX","51"}),
																		pair<string, string>({"playerPosY","45"}),
																		pair<string, string>({"audio","WOODENSTAIRS"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"}),
											})),
										mapObject("Fireplace", true, true, false, imageLookup.getSequenceAsString("Fireplace1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["Map"] + 2,"1.0","1.0","CENTRE",{53.6, 49.2}, false, List<mapFloor::triangle>({mapFloor::triangle({{52.78176665306091,47.05861508846283}, {52.88730263710022,51.08172297477722}, {50.99630951881409,47.94119894504547}}),mapFloor::triangle({{52.78176665306091,47.05861508846283}, {54.733604192733765,50.480109453201294}, {52.88730263710022,51.08172297477722}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_A Roaring Fireplace"}),
																		pair<string, string>({"copy","Fireplace"}),
																		pair<string, string>({"cutscene","Fireplace1"}),
																		pair<string, string>({"x","5"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
				}), List<mapFloor>({
	mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{45.79947292804718,51.260411739349365}, {44.93342936038971,53.57964038848877}, {46.94136381149292,52.92918682098389}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {45.80681324005127,50.14413595199585}, {44.362592697143555,47.89978265762329}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {46.95880115032196,50.853925943374634}, {45.79947292804718,51.260411739349365}}),
mapFloor::triangle({{48.06930422782898,52.55924463272095}, {48.11813235282898,50.4474401473999}, {46.95880115032196,50.853925943374634}}),mapFloor::triangle({{49.1972416639328,52.18930244445801}, {49.27746057510376,50.04095435142517}, {48.11813235282898,50.4474401473999}}),mapFloor::triangle({{50.3251850605011,51.81936025619507}, {51.59047245979309,49.22996759414673}, {49.27746057510376,50.04095435142517}}),
mapFloor::triangle({{52.57557034492493,51.08126401901245}, {52.7554452419281,48.821502923965454}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{46.95880115032196,50.853925943374634}, {45.80681324005127,50.14413595199585}, {45.79947292804718,51.260411739349365}}),mapFloor::triangle({{47.90157377719879,48.25414419174194}, {48.11813235282898,50.4474401473999}, {49.41757321357727,47.47089147567749}}),
mapFloor::triangle({{49.41757321357727,47.47089147567749}, {49.27746057510376,50.04095435142517}, {49.83961284160614,47.57797718048096}}),mapFloor::triangle({{49.83961284160614,47.57797718048096}, {51.59047245979309,49.22996759414673}, {51.19330286979675,47.28615880012512}}),mapFloor::triangle({{51.19330286979675,47.28615880012512}, {52.7554452419281,48.821502923965454}, {52.187734842300415,47.12257981300354}}),
mapFloor::triangle({{47.162121534347534,46.98895812034607}, {52.725422382354736,43.473583459854126}, {47.90157377719879,48.25414419174194}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {45.05603313446045,53.894245624542236}, {47.06396758556366,53.24379205703735}}),mapFloor::triangle({{45.79947292804718,51.260411739349365}, {44.01206374168396,51.8357515335083}, {44.93342936038971,53.57964038848877}}),
mapFloor::triangle({{47.162121534347534,46.98895812034607}, {47.90157377719879,48.25414419174194}, {45.80681324005127,50.14413595199585}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {48.06930422782898,52.55924463272095}, {46.95880115032196,50.853925943374634}}),mapFloor::triangle({{48.06930422782898,52.55924463272095}, {49.1972416639328,52.18930244445801}, {48.11813235282898,50.4474401473999}}),
mapFloor::triangle({{49.1972416639328,52.18930244445801}, {50.3251850605011,51.81936025619507}, {49.27746057510376,50.04095435142517}}),mapFloor::triangle({{50.3251850605011,51.81936025619507}, {52.57557034492493,51.08126401901245}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{52.57557034492493,51.08126401901245}, {53.8967490196228,50.61107873916626}, {52.7554452419281,48.821502923965454}}),
mapFloor::triangle({{46.95880115032196,50.853925943374634}, {47.90157377719879,48.25414419174194}, {45.80681324005127,50.14413595199585}}),mapFloor::triangle({{47.90157377719879,48.25414419174194}, {46.95880115032196,50.853925943374634}, {48.11813235282898,50.4474401473999}}),mapFloor::triangle({{49.41757321357727,47.47089147567749}, {48.11813235282898,50.4474401473999}, {49.27746057510376,50.04095435142517}}),
mapFloor::triangle({{49.83961284160614,47.57797718048096}, {49.27746057510376,50.04095435142517}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{51.19330286979675,47.28615880012512}, {51.59047245979309,49.22996759414673}, {52.7554452419281,48.821502923965454}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {51.98596715927124,42.42135286331177}, {52.725422382354736,43.473583459854126}}),
mapFloor::triangle({{46.94136381149292,52.92918682098389}, {44.93342936038971,53.57964038848877}, {45.05603313446045,53.894245624542236}}),}),true,{}),
		mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{45.79947292804718,51.260411739349365}, {44.93342936038971,53.57964038848877}, {46.94136381149292,52.92918682098389}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {45.80681324005127,50.14413595199585}, {44.362592697143555,47.89978265762329}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {46.95880115032196,50.853925943374634}, {45.79947292804718,51.260411739349365}}),
mapFloor::triangle({{48.06930422782898,52.55924463272095}, {48.11813235282898,50.4474401473999}, {46.95880115032196,50.853925943374634}}),mapFloor::triangle({{49.1972416639328,52.18930244445801}, {49.27746057510376,50.04095435142517}, {48.11813235282898,50.4474401473999}}),mapFloor::triangle({{50.3251850605011,51.81936025619507}, {51.59047245979309,49.22996759414673}, {49.27746057510376,50.04095435142517}}),
mapFloor::triangle({{52.57557034492493,51.08126401901245}, {52.7554452419281,48.821502923965454}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{46.95880115032196,50.853925943374634}, {45.80681324005127,50.14413595199585}, {45.79947292804718,51.260411739349365}}),mapFloor::triangle({{47.90157377719879,48.25414419174194}, {48.11813235282898,50.4474401473999}, {49.41757321357727,47.47089147567749}}),
mapFloor::triangle({{49.41757321357727,47.47089147567749}, {49.27746057510376,50.04095435142517}, {49.83961284160614,47.57797718048096}}),mapFloor::triangle({{49.83961284160614,47.57797718048096}, {51.59047245979309,49.22996759414673}, {51.19330286979675,47.28615880012512}}),mapFloor::triangle({{51.19330286979675,47.28615880012512}, {52.7554452419281,48.821502923965454}, {52.187734842300415,47.12257981300354}}),
mapFloor::triangle({{47.162121534347534,46.98895812034607}, {52.725422382354736,43.473583459854126}, {47.90157377719879,48.25414419174194}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {45.05603313446045,53.894245624542236}, {47.06396758556366,53.24379205703735}}),mapFloor::triangle({{45.79947292804718,51.260411739349365}, {44.01206374168396,51.8357515335083}, {44.93342936038971,53.57964038848877}}),
mapFloor::triangle({{47.162121534347534,46.98895812034607}, {47.90157377719879,48.25414419174194}, {45.80681324005127,50.14413595199585}}),mapFloor::triangle({{46.94136381149292,52.92918682098389}, {48.06930422782898,52.55924463272095}, {46.95880115032196,50.853925943374634}}),mapFloor::triangle({{48.06930422782898,52.55924463272095}, {49.1972416639328,52.18930244445801}, {48.11813235282898,50.4474401473999}}),
mapFloor::triangle({{49.1972416639328,52.18930244445801}, {50.3251850605011,51.81936025619507}, {49.27746057510376,50.04095435142517}}),mapFloor::triangle({{50.3251850605011,51.81936025619507}, {52.57557034492493,51.08126401901245}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{52.57557034492493,51.08126401901245}, {53.8967490196228,50.61107873916626}, {52.7554452419281,48.821502923965454}}),
mapFloor::triangle({{46.95880115032196,50.853925943374634}, {47.90157377719879,48.25414419174194}, {45.80681324005127,50.14413595199585}}),mapFloor::triangle({{47.90157377719879,48.25414419174194}, {46.95880115032196,50.853925943374634}, {48.11813235282898,50.4474401473999}}),mapFloor::triangle({{49.41757321357727,47.47089147567749}, {48.11813235282898,50.4474401473999}, {49.27746057510376,50.04095435142517}}),
mapFloor::triangle({{49.83961284160614,47.57797718048096}, {49.27746057510376,50.04095435142517}, {51.59047245979309,49.22996759414673}}),mapFloor::triangle({{51.19330286979675,47.28615880012512}, {51.59047245979309,49.22996759414673}, {52.7554452419281,48.821502923965454}}),mapFloor::triangle({{47.162121534347534,46.98895812034607}, {51.98596715927124,42.42135286331177}, {52.725422382354736,43.473583459854126}}),
mapFloor::triangle({{46.94136381149292,52.92918682098389}, {44.93342936038971,53.57964038848877}, {45.05603313446045,53.894245624542236}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
					}), {}, { 5000,5000 }, { Map<string, string>({
						pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
						pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
						}) });
		maps["House1Inside2"] = mapInstance("House1Inside2", EMPTYMAP, { 49,46 }, List<mapObject>({
		mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
		mapObject("House1Inside2AsObject", false, true, false, imageLookup.getSequenceAsString("House1Inside2AsObject","STAND_FRONT"),"0",0,imageLookup.layerDefaults["Map"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
										mapObject::getTreasureChest("House1Inside2Chest1", {48,44},"NORMAL_FRONT","The Eyes of St Lucy"),
										mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{52, 43}, false, List<mapFloor::triangle>({mapFloor::triangle({{52.46635675430298,44.129398465156555}, {51.47292613983154,44.85166668891907}, {52.68528461456299,44.555461406707764}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {51.08272433280945,44.92737650871277}, {51.47292613983154,44.85166668891907}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {51.75027251243591,42.783528566360474}, {50.81353783607483,43.60819458961487}}),
							mapFloor::triangle({{52.46635675430298,44.129398465156555}, {51.25399827957153,44.42560374736786}, {51.47292613983154,44.85166668891907}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {50.86379647254944,44.50131356716156}, {51.08272433280945,44.92737650871277}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {52.46635675430298,44.129398465156555}, {51.75027251243591,42.783528566360474}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Back Downstairs"}),
																		pair<string, string>({"copy","AT_Door"}),
																		pair<string, string>({"areaTransition","House1Inside1"}),
																		pair<string, string>({"direction","STAND_LEFT"}),
																		pair<string, string>({"playerPosX","47"}),
																		pair<string, string>({"playerPosY","48"}),
																		pair<string, string>({"audio","WOODENSTAIRS"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"}),
											})),
		}), List<mapFloor>({
				mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{53.379178047180176,47.85220921039581}, {53.849244117736816,46.73902094364166}, {52.86875367164612,47.029563784599304}}),mapFloor::triangle({{50.04163384437561,44.72180902957916}, {47.95736372470856,48.5080748796463}, {51.88826322555542,47.320106625556946}}),mapFloor::triangle({{51.88826322555542,47.320106625556946}, {48.50795567035675,49.34563338756561}, {52.43885517120361,48.15766513347626}}),
mapFloor::triangle({{45.90473473072052,45.17439305782318}, {42.659807205200195,47.07562029361725}, {46.3507205247879,45.898714661598206}}),mapFloor::triangle({{49.52903091907501,44.00055408477783}, {46.3507205247879,45.898714661598206}, {50.04163384437561,44.72180902957916}}),mapFloor::triangle({{52.86875367164612,47.029563784599304}, {52.46635675430298,44.129398465156555}, {51.25399827957153,44.42560374736786}}),
mapFloor::triangle({{51.88826322555542,47.320106625556946}, {51.25399827957153,44.42560374736786}, {50.04163384437561,44.72180902957916}}),mapFloor::triangle({{52.43885517120361,48.15766513347626}, {52.86875367164612,47.029563784599304}, {51.88826322555542,47.320106625556946}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {51.75027251243591,42.783528566360474}, {50.81353783607483,43.60819458961487}}),
mapFloor::triangle({{44.996535778045654,46.330517530441284}, {43.67576241493225,49.023592472076416}, {46.12543284893036,48.27328622341156}}),mapFloor::triangle({{46.3507205247879,45.898714661598206}, {46.12543284893036,48.27328622341156}, {47.5450724363327,47.83846735954285}}),mapFloor::triangle({{48.04847836494446,47.506579756736755}, {47.494322061538696,48.897457122802734}, {48.91396164894104,48.46263527870178}}),
mapFloor::triangle({{53.379178047180176,47.85220921039581}, {54.31949496269226,47.54675328731537}, {53.849244117736816,46.73902094364166}}),mapFloor::triangle({{50.04163384437561,44.72180902957916}, {46.3507205247879,45.898714661598206}, {47.95736372470856,48.5080748796463}}),mapFloor::triangle({{51.88826322555542,47.320106625556946}, {47.95736372470856,48.5080748796463}, {48.50795567035675,49.34563338756561}}),
mapFloor::triangle({{45.90473473072052,45.17439305782318}, {42.28043854236603,46.34822905063629}, {42.659807205200195,47.07562029361725}}),mapFloor::triangle({{49.52903091907501,44.00055408477783}, {45.90473473072052,45.17439305782318}, {46.3507205247879,45.898714661598206}}),mapFloor::triangle({{52.86875367164612,47.029563784599304}, {53.849244117736816,46.73902094364166}, {52.46635675430298,44.129398465156555}}),
mapFloor::triangle({{51.88826322555542,47.320106625556946}, {52.86875367164612,47.029563784599304}, {51.25399827957153,44.42560374736786}}),mapFloor::triangle({{52.43885517120361,48.15766513347626}, {53.379178047180176,47.85220921039581}, {52.86875367164612,47.029563784599304}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {52.46635675430298,44.129398465156555}, {51.75027251243591,42.783528566360474}}),
mapFloor::triangle({{44.996535778045654,46.330517530441284}, {42.659807205200195,47.07562029361725}, {43.67576241493225,49.023592472076416}}),mapFloor::triangle({{46.3507205247879,45.898714661598206}, {44.996535778045654,46.330517530441284}, {46.12543284893036,48.27328622341156}}),mapFloor::triangle({{48.04847836494446,47.506579756736755}, {46.628838777542114,47.94139862060547}, {47.494322061538696,48.897457122802734}}),}),true,{}),
				mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{53.379178047180176,47.85220921039581}, {53.849244117736816,46.73902094364166}, {52.86875367164612,47.029563784599304}}),mapFloor::triangle({{50.04163384437561,44.72180902957916}, {47.95736372470856,48.5080748796463}, {51.88826322555542,47.320106625556946}}),mapFloor::triangle({{51.88826322555542,47.320106625556946}, {48.50795567035675,49.34563338756561}, {52.43885517120361,48.15766513347626}}),
mapFloor::triangle({{45.90473473072052,45.17439305782318}, {42.659807205200195,47.07562029361725}, {46.3507205247879,45.898714661598206}}),mapFloor::triangle({{49.52903091907501,44.00055408477783}, {46.3507205247879,45.898714661598206}, {50.04163384437561,44.72180902957916}}),mapFloor::triangle({{52.86875367164612,47.029563784599304}, {52.46635675430298,44.129398465156555}, {51.25399827957153,44.42560374736786}}),
mapFloor::triangle({{51.88826322555542,47.320106625556946}, {51.25399827957153,44.42560374736786}, {50.04163384437561,44.72180902957916}}),mapFloor::triangle({{52.43885517120361,48.15766513347626}, {52.86875367164612,47.029563784599304}, {51.88826322555542,47.320106625556946}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {51.75027251243591,42.783528566360474}, {50.81353783607483,43.60819458961487}}),
mapFloor::triangle({{44.996535778045654,46.330517530441284}, {43.67576241493225,49.023592472076416}, {46.12543284893036,48.27328622341156}}),mapFloor::triangle({{46.3507205247879,45.898714661598206}, {46.12543284893036,48.27328622341156}, {47.5450724363327,47.83846735954285}}),mapFloor::triangle({{47.5450724363327,47.83846735954285}, {46.86218500137329,48.74661564826965}, {48.062798380851746,48.64929914474487}}),
mapFloor::triangle({{53.379178047180176,47.85220921039581}, {54.31949496269226,47.54675328731537}, {53.849244117736816,46.73902094364166}}),mapFloor::triangle({{50.04163384437561,44.72180902957916}, {46.3507205247879,45.898714661598206}, {47.95736372470856,48.5080748796463}}),mapFloor::triangle({{51.88826322555542,47.320106625556946}, {47.95736372470856,48.5080748796463}, {48.50795567035675,49.34563338756561}}),
mapFloor::triangle({{45.90473473072052,45.17439305782318}, {42.28043854236603,46.34822905063629}, {42.659807205200195,47.07562029361725}}),mapFloor::triangle({{49.52903091907501,44.00055408477783}, {45.90473473072052,45.17439305782318}, {46.3507205247879,45.898714661598206}}),mapFloor::triangle({{52.86875367164612,47.029563784599304}, {53.849244117736816,46.73902094364166}, {52.46635675430298,44.129398465156555}}),
mapFloor::triangle({{51.88826322555542,47.320106625556946}, {52.86875367164612,47.029563784599304}, {51.25399827957153,44.42560374736786}}),mapFloor::triangle({{52.43885517120361,48.15766513347626}, {53.379178047180176,47.85220921039581}, {52.86875367164612,47.029563784599304}}),mapFloor::triangle({{51.25399827957153,44.42560374736786}, {52.46635675430298,44.129398465156555}, {51.75027251243591,42.783528566360474}}),
mapFloor::triangle({{44.996535778045654,46.330517530441284}, {42.659807205200195,47.07562029361725}, {43.67576241493225,49.023592472076416}}),mapFloor::triangle({{46.3507205247879,45.898714661598206}, {44.996535778045654,46.330517530441284}, {46.12543284893036,48.27328622341156}}),mapFloor::triangle({{47.5450724363327,47.83846735954285}, {46.12543284893036,48.27328622341156}, {46.86218500137329,48.74661564826965}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
			}), {}, { 5000,5000 }, { Map<string, string>({
				pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
				}) });
		maps["BénouvilleTown1"] = mapInstance("BénouvilleTown1", MAP_TOWN1, { 50,50 }, List<mapObject>({
					mapObject("AT_GoToEstate", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 24, 5 }, false, List<mapFloor::triangle>({mapFloor::triangle({{30.819743871688843,1.0285344906151295}, {18.956512212753296,6.336745619773865}, {18.956512212753296,1.0285339318215847}}),mapFloor::triangle({{30.819743871688843,1.0285344906151295}, {30.819743871688843,6.336746364831924}, {18.956512212753296,6.336745619773865}}),}), Map<string, string>({
													pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go to Estate"}),
													pair<string, string>({"copy","AT_GoToEstate"}),
													pair<string, string>({"areaTransition","EstateOutside1"}),
													pair<string, string>({"direction","STAND_BACK"}),
													pair<string, string>({"playerPosX","24"}),
													pair<string, string>({"playerPosY","95"}),
													pair<string, string>({"audio","STONESTEPS"}),
													pair<string, string>({"x","0"}),
													pair<string, string>({"y","0"}),
													pair<string, string>({"h","50"}),
													pair<string, string>({"w","50"}),
													pair<string, string>("colour", "WHITE"),
													pair<string, string>("uniqueID", mapPopupTextID),
													pair<string, string>("anchorStyle", "TOPLEFT"),
													pair<string, string>("shadowColour", "BLACK"),
													pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
													pair<string, string>({"format","LightText_20"}),
					})),
					mapObject("AT_LeaveTown", true, false, false, "","0",0,0,"1","1.0","CENTRE",{64, 98}, false, List<mapFloor::triangle>({mapFloor::triangle({{65.90241193771362,98.22542667388916}, {58.13485383987427,100.4989743232727}, {66.02039933204651,100.25829076766968}}),mapFloor::triangle({{65.90241193771362,98.22542667388916}, {58.016860485076904,98.46611618995667}, {58.13485383987427,100.4989743232727}}),}), Map<string, string>({
													pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave Bénouville"}),
													pair<string, string>({"copy","AT_LeaveTown"}),
													pair<string, string>({"areaTransition","RoadToBénouville"}),
													pair<string, string>({"direction","STAND_FRONT"}),
													pair<string, string>({"playerPosX","53"}),
													pair<string, string>({"playerPosY","5"}),
													pair<string, string>({"audio","STONESTEPS"}),
													pair<string, string>({"x","0"}),
													pair<string, string>({"y","0"}),
													pair<string, string>({"h","50"}),
													pair<string, string>({"w","50"}),
													pair<string, string>("colour", "WHITE"),
													pair<string, string>("uniqueID", mapPopupTextID),
													pair<string, string>("anchorStyle", "TOPLEFT"),
													pair<string, string>("shadowColour", "BLACK"),
													pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
													pair<string, string>({"format","LightText_20"}),
														})),
					mapObject("AT_DoorToTavern", true, false, false, "","0",0,0,"1","1.0","CENTRE",{43, 58}, false, List<mapFloor::triangle>({mapFloor::triangle({{43.5824990272522,58.83595943450928}, {39.62002992630005,54.028284549713135}, {39.86047804355621,59.01541709899902}}),mapFloor::triangle({{43.5824990272522,58.83595943450928}, {43.34205090999603,53.84882688522339}, {39.62002992630005,54.028284549713135}}),}), Map<string, string>({
													pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Tavern"}),
													pair<string, string>({"copy","AT_DoorToTavern"}),
													pair<string, string>({"areaTransition","Tavern1"}),
													pair<string, string>({"direction","STAND_BACK"}),
													pair<string, string>({"playerPosX","50"}),
													pair<string, string>({"playerPosY","56"}),
													pair<string, string>({"audio","NICEDOOR1"}),
													pair<string, string>({"x","0"}),
													pair<string, string>({"y","-15"}),
													pair<string, string>({"h","50"}),
													pair<string, string>({"w","50"}),
													pair<string, string>("colour", "WHITE"),
													pair<string, string>("uniqueID", mapPopupTextID),
													pair<string, string>("anchorStyle", "TOPLEFT"),
													pair<string, string>("shadowColour", "BLACK"),
													pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
													pair<string, string>({"format","LightText_20"}),
														})),
					mapObject("AT_DoorToIso", true, false, false, "","0",0,0,"1","1.0","CENTRE",{55, 51}, false, List<mapFloor::triangle>({mapFloor::triangle({{55.41817545890808,50.60643553733826}, {54.307717084884644,52.09433436393738}, {55.41817545890808,52.09433436393738}}),mapFloor::triangle({{55.41817545890808,50.60643553733826}, {54.307717084884644,50.60643553733826}, {54.307717084884644,52.09433436393738}}),}), Map<string, string>({
													pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Iso"}),
													pair<string, string>({"copy","AT_DoorToIso"}),
													pair<string, string>({"areaTransition","IsoHouse"}),
													pair<string, string>({"direction","STAND_RIGHT"}),
													pair<string, string>({"playerPosX","47"}),
													pair<string, string>({"playerPosY","46"}),
													pair<string, string>({"audio","NICEDOOR1"}),
													pair<string, string>({"x","0"}),
													pair<string, string>({"y","0"}),
													pair<string, string>({"h","50"}),
													pair<string, string>({"w","50"}),
													pair<string, string>("colour", "WHITE"),
													pair<string, string>("uniqueID", mapPopupTextID),
													pair<string, string>("anchorStyle", "TOPLEFT"),
													pair<string, string>("shadowColour", "BLACK"),
													pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
													pair<string, string>({"format","LightText_20"}),
														})),
					mapObject("AT_DoorToSmithy", true, false, false, "","0",0,0,"1","1.0","CENTRE",{20, 60}, false, List<mapFloor::triangle>({mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.555029809474945,61.3682746887207}, {22.816142439842224,60.81589460372925}}),mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.662133395671844,63.6304497718811}, {20.555029809474945,61.3682746887207}}),}), Map<string, string>({
													pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Smithy"}),
													pair<string, string>({"copy","AT_DoorToSmithy"}),
													pair<string, string>({"areaTransition","SmithyInside"}),
													pair<string, string>({"direction","STAND_BACK"}),
													pair<string, string>({"playerPosX","49"}),
													pair<string, string>({"playerPosY","59"}),
													pair<string, string>({"audio","SHOPDOOR1"}),
													pair<string, string>({"x","0"}),
													pair<string, string>({"y","0"}),
													pair<string, string>({"h","50"}),
													pair<string, string>({"w","50"}),
													pair<string, string>("colour", "WHITE"),
													pair<string, string>("uniqueID", mapPopupTextID),
													pair<string, string>("anchorStyle", "TOPLEFT"),
													pair<string, string>("shadowColour", "BLACK"),
													pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
													pair<string, string>({"format","LightText_20"}),
														})),
				mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
				mapObject("PeasantF1", true, true, false, imageLookup.getSequenceAsString("PEASANTF1","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{42, 19}, false, List<mapFloor::triangle>({mapFloor::triangle({{43.42213273048401,17.592549324035645}, {40.98268151283264,21.704578399658203}, {44.179099798202515,21.084311604499817}}),mapFloor::triangle({{43.42213273048401,17.592549324035645}, {40.225714445114136,18.21281760931015}, {40.98268151283264,21.704578399658203}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Beggar"}),
												pair<string, string>({"copy","PeasantF1"}),
												pair<string, string>({"cutscene","BeggarF"}),
												pair<string, string>({"x","5"}),
												pair<string, string>({"y","-7"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("ShadowPeasantF1", false, true, false, imageLookup.getSequenceAsString("Shadow PEASANTF1","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{42, 19}, false, {}, {}),
					mapObject("TownGuardF2", true, true, false, imageLookup.getSequenceAsString("GUARD1F","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{69, 99}, false, List<mapFloor::triangle>({}), Map<string, string>()),
					mapObject("Shadow TownGuardF2", true, true, false, imageLookup.getSequenceAsString("Shadow GUARD1F","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"1.0","1.0","CENTRE",{69, 99}, false, List<mapFloor::triangle>({}), Map<string, string>()),
					mapObject("TownGuardM2", true, true, false, imageLookup.getSequenceAsString("GUARD1M","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{72, 98}, false, List<mapFloor::triangle>({}), Map<string, string>()),
					mapObject("Shadow TownGuardM2", true, true, false, imageLookup.getSequenceAsString("Shadow GUARD1M","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"1.0","1.0","CENTRE",{72, 98}, false, List<mapFloor::triangle>({}), Map<string, string>()),
					mapObject("TownGuardF1", true, true, false, imageLookup.getSequenceAsString("GUARD1F","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{45, 25}, false, List<mapFloor::triangle>({}), Map<string, string>()),
					mapObject("TownGuardF1Shadow", false, true, false, imageLookup.getSequenceAsString("Shadow GUARD1F","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{45, 25}, false, {}, {}),
					mapObject("TownGuardM1", true, true, false, imageLookup.getSequenceAsString("GUARD1M","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{39.5, 25.5}, false, List<mapFloor::triangle>({mapFloor::triangle({{41.35875403881073,24.685853719711304}, {39.43250775337219,28.882494568824768}, {42.34980344772339,28.518861532211304}}),mapFloor::triangle({{45.040708780288696,24.21427220106125}, {43.37426722049713,28.496503829956055}, {46.3937908411026,27.996671199798584}}),mapFloor::triangle({{41.35875403881073,24.685853719711304}, {38.425999879837036,25.01887083053589}, {39.43250775337219,28.882494568824768}}), mapFloor::triangle({{45.040708780288696,24.21427220106125}, {42.47890114784241,24.653708934783936}, {43.37426722049713,28.496503829956055}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to TownGuards1"}),
												pair<string, string>({"copy","TownGuardM1"}),
												pair<string, string>({"cutscene","TownGuards1"}),
												pair<string, string>({"x","4"}),
												pair<string, string>({"y","-7"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("TownGuardM1Shadow", false, true, false, imageLookup.getSequenceAsString("Shadow GUARD1M","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{39.5, 25.5}, false, {}, {}),
					mapObject("MerchantNPC3", true, true, false, imageLookup.getSequenceAsString("ELDERLYF2", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 36.8, 96.7 }, false, List<mapFloor::triangle>({mapFloor::triangle({{38.54956924915314,94.56130266189575}, {35.565540194511414,98.0098307132721}, {38.54956924915314,98.0098307132721}}),mapFloor::triangle({{38.54956924915314,94.56130266189575}, {35.565540194511414,94.56130266189575}, {35.565540194511414,98.0098307132721}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to MerchantNPC3"}),
												pair<string, string>({"copy","MerchantNPC3"}),
												pair<string, string>({"x","5"}),
												pair<string, string>({"y","-5"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("Merchant", "1"),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}) })),
					mapObject("MerchantNPC3Shadow", false, true, false, imageLookup.getSequenceAsString("ELDERLYF2SHADOW","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{36.8, 96.7}, false, {},{}),
					mapObject("OldWoman1", true, true, false, imageLookup.getSequenceAsString("ELDERLYF1","STAND_FRONT"),"1",1000,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{27.7, 83.8}, false, List<mapFloor::triangle>({mapFloor::triangle({{29.181161522865295,82.40866661071777}, {25.04141330718994,86.26223802566528}, {29.181161522865295,86.26223802566528}}),mapFloor::triangle({{29.181161522865295,82.40866661071777}, {25.04141330718994,82.40866661071777}, {25.04141330718994,86.26223802566528}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to OldWoman1"}),
												pair<string, string>({"copy","OldWoman1"}),
												pair<string, string>({"cutscene","OldWoman1"}),
												pair<string, string>({"x","2"}),
												pair<string, string>({"y","-7"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("OldWoman1Shadow", false, true, false, imageLookup.getSequenceAsString("ELDERLYF1SHADOW","STAND_FRONT"),"1",1000,imageLookup.layerDefaults["PLAYER"],"0.5","1.0","CENTRE",{27.7, 83.79}, false, {}, {}),
					mapObject("Fog1", false, true, true, imageLookup.getSequenceAsString("CityFog1","STAND_FRONT"),"1",200,imageLookup.layerDefaults["ENVIRONMENT"] + 1,"0.5","1.0","CENTRE",{0, 0}, false, {}, {}),
					mapObject("MerchantNPC2", true, true, false, imageLookup.getSequenceAsString("NPCMAN2", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 41.5, 92 }, false, List<mapFloor::triangle>({mapFloor::triangle({{42.0989990234375,93.16661357879639}, {38.614171743392944,95.978844165802}, {42.0989990234375,96.03456854820251}}),mapFloor::triangle({{42.0989990234375,93.16661357879639}, {38.614171743392944,93.11088919639587}, {38.614171743392944,95.978844165802}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to MerchantNPC2"}),
												pair<string, string>({"copy","MerchantNPC2"}),
												pair<string, string>({"x","5"}),
												pair<string, string>({"y","-5"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("Merchant", "1"),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}) })),
					mapObject("MerchantNPC2Shadow", false, true, false, imageLookup.getSequenceAsString("NPCMAN2SHADOW","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{41.5, 92}, false, {},{}),
					mapObject("MerchantNPC1", true, true, false, imageLookup.getSequenceAsString("NPCMAN1", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 33, 93 }, false, List<mapFloor::triangle>({mapFloor::triangle({{35.52044630050659,93.16661357879639}, {32.0356160402298,95.978844165802}, {35.52044630050659,96.03456854820251}}),mapFloor::triangle({{35.52044630050659,93.16661357879639}, {32.0356160402298,93.11088919639587}, {32.0356160402298,95.978844165802}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to MerchantNPC1"}),
												pair<string, string>({"copy","MerchantNPC1"}),
												pair<string, string>({"x","-20"}),
												pair<string, string>({"y","-5"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("Merchant", "1"),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}) })),
					mapObject("MerchantNPC1Shadow", false, true, false, imageLookup.getSequenceAsString("NPCMAN1SHADOW","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{33, 93}, false, {},{}),
					mapObject("GRAVEDIGGER1", true, true, false, imageLookup.getSequenceAsString("Gravedigger","STAND_FRONT"),"1",550,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{13, 73}, false, List<mapFloor::triangle>({mapFloor::triangle({{17.683838307857513,72.1723198890686}, {12.929876148700714,75.78386068344116}, {17.683838307857513,76.07642412185669}}),mapFloor::triangle({{17.683838307857513,72.1723198890686}, {12.929876148700714,72.1723198890686}, {12.929876148700714,75.78386068344116}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Gravedigger"}),
												pair<string, string>({"copy","GRAVEDIGGER1"}),
												pair<string, string>({"cutscene","GraveDigger"}),
												pair<string, string>({"x","-5"}),
												pair<string, string>({"y","-7"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("GRAVEDIGGER1SHADOW", false, true, false, imageLookup.getSequenceAsString("Shadow Gravedigger","STAND_FRONT"),"1",550,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{13, 73}, false, {},{}),
					mapObject("GRAVE5", true, false, false, "","0",0,0,"","","",{16, 76}, false, List<mapFloor::triangle>({mapFloor::triangle({{17.943866550922394,75.58833956718445}, {13.055244088172913,78.94938588142395}, {18.583104014396667,78.0000388622284}}),mapFloor::triangle({{17.943866550922394,75.58833956718445}, {12.664280831813812,76.53768062591553}, {13.055244088172913,78.94938588142395}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
												pair<string, string>({"copy","GRAVE5"}),
												pair<string, string>({"cutscene","Grave5"}),
												pair<string, string>({"x","2"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("GRAVE4", true, false, false, "","0",0,0,"","","",{8, 82}, false, List<mapFloor::triangle>({mapFloor::triangle({{11.5057572722435,81.94552659988403}, {7.157927751541138,83.7360680103302}, {11.5057572722435,83.7360680103302}}),mapFloor::triangle({{11.5057572722435,81.94552659988403}, {7.157927751541138,81.94552659988403}, {7.157927751541138,83.7360680103302}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
												pair<string, string>({"copy","GRAVE4"}),
												pair<string, string>({"cutscene","Grave4"}),
												pair<string, string>({"x","-5"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("GRAVE3", true, false, false, "","0",0,0,"","","",{8, 80}, false, List<mapFloor::triangle>({mapFloor::triangle({{11.5057572722435,79.9683690071106}, {7.157927751541138,81.75891041755676}, {11.5057572722435,81.75891041755676}}),mapFloor::triangle({{11.5057572722435,79.9683690071106}, {7.157927751541138,79.9683690071106}, {7.157927751541138,81.75891041755676}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
												pair<string, string>({"copy","GRAVE3"}),
												pair<string, string>({"cutscene","Grave3"}),
												pair<string, string>({"x","-5"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("GRAVE2", true, false, false, "","0",0,0,"","","",{8, 78}, false, List<mapFloor::triangle>({mapFloor::triangle({{12.003235518932343,77.97845602035522}, {7.655405253171921,79.76899743080139}, {12.003235518932343,79.76899743080139}}),mapFloor::triangle({{12.003235518932343,77.97845602035522}, {7.655405253171921,77.97845602035522}, {7.655405253171921,79.76899743080139}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
												pair<string, string>({"copy","GRAVE2"}),
												pair<string, string>({"cutscene","Grave2"}),
												pair<string, string>({"x","-5"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("GRAVE1", true, false, false, "","0",0,0,"","","",{7.7, 76.7}, false, List<mapFloor::triangle>({mapFloor::triangle({{12.003235518932343,75.9587824344635}, {7.655405253171921,77.96206474304199}, {12.003235518932343,77.96206474304199}}),mapFloor::triangle({{12.003235518932343,75.9587824344635}, {7.655405253171921,75.9587824344635}, {7.655405253171921,77.96206474304199}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Pray at Grave"}),
												pair<string, string>({"copy","GRAVE1"}),
												pair<string, string>({"cutscene","Grave1"}),
												pair<string, string>({"x","-5"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("Lamp1", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",19,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{46.5, 51.8}, false, {}, {}),
					mapObject("Lamp2", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{45.1, 51.7}, false, {}, {}),
					mapObject("Lamp3", false, true, false, imageLookup.getSequenceAsString("CITYLAMP3","STAND_FRONT"),"1",21,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{48.8, 51.9}, false, {}, {}),
					mapObject("Lamp4", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{50.9, 52}, false, {}, {}),
					mapObject("Lamp5", false, true, false, imageLookup.getSequenceAsString("CITYLAMP3","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{39.1, 56.1}, false, {}, {}),
					mapObject("Lamp6", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{34.1, 56.0}, false, {}, {}),
					mapObject("Lamp7", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{29.3, 56.5}, false, {}, {}),
					mapObject("Lamp8", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{19.2,60.1}, false, {}, {}),
					mapObject("Lamp9", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{7.1,69.1}, false, {}, {}),
					mapObject("Lamp10", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{27.7,78.8}, false, {}, {}),
					mapObject("Lamp11", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{68.4,94.6}, false, {}, {}),
					mapObject("Lamp12", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{61.1,56.2}, false, {}, {}),
					mapObject("Lamp12.5", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{43.1, 39.9}, false, {}, {}),
					mapObject("Lamp13", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{46.1, 40}, false, {}, {}),
					mapObject("Lamp14", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{45.6, 39.7}, false, {}, {}),
					mapObject("Lamp15", false, true, false, imageLookup.getSequenceAsString("CITYLAMP3","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{47.5, 39.3}, false, {}, {}),
					mapObject("Lamp16", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{49.8, 38.7}, false, {}, {}),
					mapObject("Lamp17", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{49.8, 38.7}, false, {}, {}),
					mapObject("Lamp18", false, true, false, imageLookup.getSequenceAsString("CITYLAMP3","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{39, 18.3}, false, {}, {}),
					mapObject("Lamp19", false, true, false, imageLookup.getSequenceAsString("CITYLAMP","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{41.5, 18.1}, false, {}, {}),
					mapObject("Lamp20", false, true, false, imageLookup.getSequenceAsString("CITYLAMP2","STAND_FRONT"),"1",22,imageLookup.layerDefaults["ENVIRONMENT"],"0.2","1.5","CENTRE",{43.3, 17.7}, false, {}, {}),
					mapObject("Lamp21", false, true, false, imageLookup.getSequenceAsString("LampLight1","STAND_FRONT"),"1",50,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","1.0","CENTRE",{17.5,2.5}, false, {}, {}),
					mapObject("Flag1", false, true, false, imageLookup.getSequenceAsString("FLAG1","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","2.0","CENTRE",{31.5,89.3}, false, {}, {}),
					mapObject("Flag2", false, true, false, imageLookup.getSequenceAsString("FLAG2","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","2.0","CENTRE",{42.5,89.5}, false, {}, {}),
					mapObject("CAT1", true, true, false, imageLookup.getSequenceAsString("CAT1","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"1.0","0.5","CENTRE",{52.75, 53.5}, false, List<mapFloor::triangle>({mapFloor::triangle({{54.775017499923706,52.226608991622925}, {52.116209268569946,55.789148807525635}, {54.775017499923706,55.789148807525635}}),mapFloor::triangle({{54.775017499923706,52.226608991622925}, {52.116209268569946,52.226608991622925}, {52.116209268569946,55.789148807525635}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to the Cat"}),
												pair<string, string>({"copy","CAT1"}),
												pair<string, string>({"cutscene","Cat"}),
												pair<string, string>({"x","-5"}),
												pair<string, string>({"y","-7"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("CAT1SHADOW", false, true, false, imageLookup.getSequenceAsString("CAT1SHADOW1","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 2,"1.0","0.5","CENTRE",{52.75, 53.5}, false, {},{}),
					mapObject("DOG1", true, true, false, imageLookup.getSequenceAsString("DOG1", "STAND_FRONT"), "1", 400, imageLookup.layerDefaults["PLAYER"] - 1, "1.0", "1", "CENTRE", { 11, 73.5 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{12.794099748134613,72.1723198890686}, {10.228399932384491,75.74518918991089}, {12.794099748134613,75.74518918991089}}),mapFloor::triangle({{12.794099748134613,72.1723198890686}, {10.228399932384491,72.1723198890686}, {10.228399932384491,75.74518918991089}}), }), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to the Dog"}),
												pair<string, string>({"copy","DOG1"}),
												pair<string, string>({"cutscene","Dog"}),
												pair<string, string>({"x","-5"}),
												pair<string, string>({"y","-7"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"})})),
					mapObject("DOG1SHADOW", false, true, false, imageLookup.getSequenceAsString("DOG1SHADOW","STAND_FRONT"),"1",400,imageLookup.layerDefaults["PLAYER"] - 2,"1.0","1","CENTRE",{11, 73.499}, false, {},{}),
					mapObject("TownTop", false, true, false, imageLookup.getSequenceAsString("TownTop","STAND_FRONT"),"0",0,imageLookup.layerDefaults["ENVIRONMENT"] - 1,"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
					mapObject("Town1Fence", false, true, false, imageLookup.getSequenceAsString("Town1Fence","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{11.4, 86.2}, false, {}, {}),
					}), List<mapFloor>({
						mapFloor("Trans", List<mapFloor::triangle>({mapFloor::triangle({{36.06741428375244,61.61784529685974}, {45.34463584423065,70.40689587593079}, {29.188668727874756,67.28110909461975}}),mapFloor::triangle({{23.434598743915558,63.77654671669006}, {29.32136058807373,66.01842045783997}, {15.57660847902298,67.97674298286438}}),mapFloor::triangle({{29.32136058807373,66.01842045783997}, {15.604294836521149,70.79758048057556}, {15.57660847902298,67.97674298286438}}),
			mapFloor::triangle({{-0.16089975833892822,15.54003357887268}, {34.60811674594879,15.542873740196228}, {37.569910287857056,11.209874600172043}}),mapFloor::triangle({{38.46757113933563,20.722617208957672}, {34.60811674594879,15.542873740196228}, {35.50577759742737,25.055617094039917}}),mapFloor::triangle({{41.55943989753723,8.371678739786148}, {47.83017337322235,9.50634628534317}, {42.341578006744385,12.52337098121643}}),
			mapFloor::triangle({{45.34463584423065,70.40689587593079}, {29.736092686653137,74.48803782463074}, {29.188668727874756,67.28110909461975}}),mapFloor::triangle({{56.818270683288574,63.53752017021179}, {66.09548926353455,72.32657074928284}, {50.21376013755798,67.82958507537842}}),mapFloor::triangle({{50.21376013755798,67.82958507537842}, {63.3530855178833,77.26289629936218}, {47.47135937213898,72.76591062545776}}),
			mapFloor::triangle({{63.3530855178833,77.26289629936218}, {47.47135937213898,87.50168085098267}, {47.47135937213898,72.76591062545776}}),mapFloor::triangle({{29.32136058807373,66.01842045783997}, {30.352306365966797,68.6963140964508}, {15.604294836521149,70.79758048057556}}),mapFloor::triangle({{-0.16089975833892822,15.54003357887268}, {-3.122694045305252,19.873031973838806}, {34.60811674594879,15.542873740196228}}),
			mapFloor::triangle({{38.46757113933563,20.722617208957672}, {37.569910287857056,11.209874600172043}, {34.60811674594879,15.542873740196228}}),mapFloor::triangle({{41.55943989753723,8.371678739786148}, {45.8541601896286,5.354653298854828}, {47.83017337322235,9.50634628534317}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {42.07055866718292,87.00407147407532}, {29.736092686653137,74.48803782463074}}),
			mapFloor::triangle({{50.21376013755798,67.82958507537842}, {66.09548926353455,72.32657074928284}, {63.3530855178833,77.26289629936218}}),mapFloor::triangle({{63.3530855178833,77.26289629936218}, {63.3530855178833,91.99866652488708}, {47.47135937213898,87.50168085098267}}),}), false, Map<string, string>({{"trans","1"}})),
						mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.54521369934082,60.92062592506409}, {55.698639154434204,60.87729334831238}}),mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.307164669036865,54.441070556640625}, {52.470725774765015,60.76837778091431}}),mapFloor::triangle({{52.307164669036865,54.441070556640625}, {55.33130764961243,50.12750029563904}, {52.176278829574585,50.17045736312866}}),
		mapFloor::triangle({{13.686150312423706,76.88034772872925}, {17.43103861808777,75.66371560096741}, {17.56890118122101,76.05867385864258}}),mapFloor::triangle({{9.941238909959793,78.42307090759277}, {13.686150312423706,76.88034772872925}, {14.22465443611145,78.42307686805725}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {13.39118778705597,76.03532671928406}, {13.686150312423706,76.88034772872925}}),
		mapFloor::triangle({{16.15099161863327,84.20861959457397}, {10.064070671796799,84.92844104766846}, {10.180553793907166,84.20861959457397}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {28.136953711509705,83.36310982704163}, {28.338921070098877,84.2086136341095}}),mapFloor::triangle({{40.47485888004303,89.06092643737793}, {28.92724573612213,86.10114455223083}, {41.99438989162445,87.10901141166687}}),
		mapFloor::triangle({{28.92724573612213,86.10114455223083}, {28.338921070098877,84.2086136341095}, {29.184192419052124,84.2086136341095}}),mapFloor::triangle({{41.99438989162445,87.10901141166687}, {45.81670165061951,90.5623197555542}, {40.43306112289429,88.83872032165527}}),mapFloor::triangle({{10.02856194972992,79.23346757888794}, {14.22465443611145,78.42307686805725}, {14.440201222896576,79.04058694839478}}),
		mapFloor::triangle({{53.61122488975525,94.16074752807617}, {58.36923122406006,92.49657392501831}, {61.028093099594116,94.16074752807617}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {10.180553793907166,84.20861959457397}, {9.978587925434113,83.3631157875061}}),mapFloor::triangle({{21.20542675256729,86.10115051269531}, {28.338921070098877,84.2086136341095}, {28.139975666999817,86.10114455223083}}),
		mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,91.30561947822571}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{51.74022316932678,92.94909238815308}, {47.37803041934967,88.8326108455658}, {54.53476905822754,90.83240032196045}}),mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,86.10115051269531}, {26.234343647956848,86.10115051269531}}),
		mapFloor::triangle({{52.675724029541016,93.55491995811462}, {54.53476905822754,90.83240032196045}, {58.36923122406006,92.49657392501831}}),mapFloor::triangle({{57.62477517127991,96.7599093914032}, {61.028093099594116,94.16074752807617}, {64.7293746471405,96.7599093914032}}),mapFloor::triangle({{59.35288071632385,99.9901294708252}, {64.7293746471405,96.7599093914032}, {64.7293746471405,99.9901294708252}}),
		mapFloor::triangle({{13.39118778705597,76.03532671928406}, {9.973200410604477,75.59431195259094}, {13.463491201400757,74.69298839569092}}),mapFloor::triangle({{17.43103861808777,75.66371560096741}, {13.463491201400757,74.69298839569092}, {16.953782737255096,73.79165887832642}}),mapFloor::triangle({{12.89408802986145,73.65986108779907}, {14.676167070865631,72.58003950119019}, {15.814974904060364,73.1858491897583}}),
		mapFloor::triangle({{12.89408802986145,73.65986108779907}, {9.973200410604477,75.59431195259094}, {9.973200410604477,74.13387298583984}}),mapFloor::triangle({{12.324683368206024,72.6267397403717}, {9.973200410604477,74.13387298583984}, {9.973200410604477,72.67343997955322}}),mapFloor::triangle({{13.463491201400757,74.69298839569092}, {15.814974904060364,73.1858491897583}, {16.953782737255096,73.79165887832642}}),
		mapFloor::triangle({{8.0686554312706,73.3474850654602}, {6.317555904388428,74.13387298583984}, {6.317555904388428,72.67343997955322}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {9.28720310330391,73.3474850654602}, {9.973200410604477,72.67343997955322}}),mapFloor::triangle({{9.28720235824585,74.13387298583984}, {8.0686554312706,73.3474850654602}, {9.28720310330391,73.3474850654602}}),
		mapFloor::triangle({{10.32191589474678,76.88034772872925}, {9.941238909959793,78.42307090759277}, {8.754055947065353,77.9451847076416}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {10.088422894477844,81.11355304718018}, {15.086840093135834,80.89309930801392}}),mapFloor::triangle({{10.088422894477844,81.11355304718018}, {14.655748009681702,79.65809106826782}, {15.086840093135834,80.89309930801392}}),
		mapFloor::triangle({{9.978587925434113,83.3631157875061}, {15.517932176589966,82.12810754776001}, {15.949025750160217,83.3631157875061}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {10.02856194972992,79.23346757888794}, {14.440201222896576,79.04058694839478}}),mapFloor::triangle({{17.204593122005463,86.66783571243286}, {21.20542675256729,86.10115051269531}, {21.20542675256729,86.66783571243286}}),
		mapFloor::triangle({{17.204593122005463,91.30561947822571}, {9.905359148979187,86.66783571243286}, {17.204593122005463,86.66783571243286}}),mapFloor::triangle({{27.62833833694458,88.05306553840637}, {26.234343647956848,86.10115051269531}, {28.92724573612213,86.10114455223083}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {21.20542675256729,98.65127205848694}, {21.20542675256729,95.42139768600464}}),
		mapFloor::triangle({{24.609489738941193,91.30561947822571}, {21.20542675256729,95.42139768600464}, {21.20542675256729,91.30561947822571}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {31.748607754707336,97.42332100868225}, {24.609489738941193,98.65127205848694}}),mapFloor::triangle({{31.748607754707336,97.42332100868225}, {32.05464780330658,94.19344663619995}, {32.14395046234131,97.42332100868225}}),
		mapFloor::triangle({{32.14395046234131,97.42332100868225}, {33.30938220024109,94.19344663619995}, {33.30938220024109,97.42332100868225}}),mapFloor::triangle({{32.05464780330658,94.19344663619995}, {33.30938220024109,93.16425919532776}, {33.30938220024109,94.19344663619995}}),mapFloor::triangle({{33.30938220024109,93.16425919532776}, {42.084380984306335,94.19344663619995}, {33.30938220024109,94.19344663619995}}),
		mapFloor::triangle({{33.30938220024109,94.19344663619995}, {42.084380984306335,97.42332100868225}, {33.30938220024109,97.42332100868225}}),mapFloor::triangle({{41.99438989162445,87.10901141166687}, {50.72827339172363,72.13049530982971}, {47.37803041934967,88.8326108455658}}),mapFloor::triangle({{51.75238847732544,58.21966528892517}, {45.356035232543945,56.528520584106445}, {51.60804986953735,56.528520584106445}}),
		mapFloor::triangle({{45.356035232543945,56.528520584106445}, {50.93286633491516,46.826037764549255}, {51.60804986953735,56.528520584106445}}),mapFloor::triangle({{48.96253943443298,38.96656632423401}, {41.62573218345642,36.34674251079559}, {48.30576181411743,36.34674251079559}}),mapFloor::triangle({{41.62573218345642,36.34674251079559}, {44.15292143821716,21.22199535369873}, {48.30576181411743,36.34674251079559}}),
		mapFloor::triangle({{34.93525981903076,8.944147825241089}, {40.346384048461914,6.569939851760864}, {41.436946392059326,8.740557730197906}}),mapFloor::triangle({{34.45371687412262,7.009121775627136}, {37.78216540813446,5.662957951426506}, {38.22864294052124,6.951345503330231}}),mapFloor::triangle({{37.69223392009735,22.020843625068665}, {42.149168252944946,12.013623863458633}, {44.15292143821716,21.22199535369873}}),
		mapFloor::triangle({{17.539416253566742,10.173983126878738}, {10.580939054489136,14.448870718479156}, {10.186772793531418,10.693792998790741}}),mapFloor::triangle({{17.226578295230865,7.878036797046661}, {10.186772793531418,10.693792998790741}, {9.94524136185646,8.392805606126785}}),mapFloor::triangle({{35.72110831737518,12.671534717082977}, {41.436946392059326,8.740557730197906}, {42.149168252944946,12.013623863458633}}),
		mapFloor::triangle({{2.6035303249955177,15.305614471435547}, {-0.058932637330144644,11.836475133895874}, {2.5048965588212013,11.550536751747131}}),mapFloor::triangle({{9.181087464094162,14.599210023880005}, {5.063920095562935,11.265134066343307}, {8.838780224323273,10.84413155913353}}),mapFloor::triangle({{10.580939054489136,14.448870718479156}, {8.838780224323273,10.84413155913353}, {10.186772793531418,10.693792998790741}}),
		mapFloor::triangle({{8.838780224323273,10.84413155913353}, {5.360473692417145,7.395584881305695}, {8.542226254940033,7.395584881305695}}),mapFloor::triangle({{5.261003226041794,15.020212531089783}, {2.5048965588212013,11.550536751747131}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{2.5048965588212013,11.550536751747131}, {3.8800302892923355,7.334820926189423}, {5.063920095562935,11.265134066343307}}),
		mapFloor::triangle({{17.983587086200714,7.824518531560898}, {17.539416253566742,10.173983126878738}, {17.226578295230865,7.878036797046661}}),mapFloor::triangle({{18.30383837223053,10.119940340518951}, {18.049949407577515,13.920833170413971}, {17.539416253566742,10.173983126878738}}),mapFloor::triangle({{9.94524136185646,8.392805606126785}, {17.024531960487366,7.054728269577026}, {17.226578295230865,7.878036797046661}}),
		mapFloor::triangle({{19.813525676727295,10.013210028409958}, {18.826469779014587,13.865935802459717}, {18.30383837223053,10.119940340518951}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {18.30383837223053,10.119940340518951}, {17.983587086200714,7.824518531560898}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {23.916561901569366,7.4050746858119965}, {25.25157630443573,7.3106929659843445}}),
		mapFloor::triangle({{25.400224328041077,9.61824655532837}, {24.62996244430542,13.455647230148315}, {24.016912281513214,9.716042876243591}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {17.983587086200714,6.906026601791382}, {19.478632509708405,6.800331175327301}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {23.641256988048553,6.506045907735825}, {23.916561901569366,7.4050746858119965}}),
		mapFloor::triangle({{23.916561901569366,7.4050746858119965}, {19.813525676727295,10.013210028409958}, {19.478632509708405,7.718823105096817}}),mapFloor::triangle({{24.016912281513214,9.716042876243591}, {20.360049605369568,13.7575164437294}, {19.813525676727295,10.013210028409958}}),mapFloor::triangle({{23.641256988048553,6.506045907735825}, {19.478632509708405,2.204103022813797}, {23.641256988048553,1.9098177552223206}}),
		mapFloor::triangle({{34.93525981903076,8.944147825241089}, {31.293559074401855,12.98454999923706}, {30.576688051223755,9.252285957336426}}),mapFloor::triangle({{34.45371687412262,6.660129129886627}, {30.576688051223755,9.252285957336426}, {30.247318744659424,6.957508623600006}}),mapFloor::triangle({{30.576688051223755,9.252285957336426}, {26.035165786743164,13.356302678585052}, {25.400224328041077,9.61824655532837}}),
		mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.400224328041077,9.61824655532837}, {25.25157630443573,7.3106929659843445}}),mapFloor::triangle({{30.129551887512207,5.877119302749634}, {25.383812189102173,1.6636453568935394}, {29.63748276233673,1.3629233464598656}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.277110934257507,6.220173090696335}, {30.129551887512207,5.877119302749634}}),
		mapFloor::triangle({{34.00723934173584,5.720734223723412}, {29.75088655948639,7.119341939687729}, {29.30440902709961,5.830954387784004}}),mapFloor::triangle({{42.48851537704468,8.46017301082611}, {43.88707876205444,5.625864863395691}, {45.2946662902832,7.711953669786453}}),mapFloor::triangle({{41.436946392059326,8.740557730197906}, {41.397953033447266,6.289554387331009}, {42.48851537704468,8.46017301082611}}),
		mapFloor::triangle({{41.397953033447266,6.289554387331009}, {43.4712290763855,4.633805900812149}, {43.88707876205444,5.625864863395691}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {51.958876848220825,60.639023780822754}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{45.53675353527069,60.98302602767944}, {51.75238847732544,58.21966528892517}, {51.958876848220825,60.639023780822754}}),
		mapFloor::triangle({{41.767215728759766,61.17992401123047}, {39.820969104766846,58.655405044555664}, {41.59303903579712,58.55809450149536}}),mapFloor::triangle({{41.59303903579712,58.55809450149536}, {39.820969104766846,58.016884326934814}, {41.623473167419434,57.53249526023865}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {43.199437856674194,56.56546354293823}, {45.356035232543945,56.528520584106445}}),
		mapFloor::triangle({{45.17846703529358,58.36119055747986}, {43.63028407096863,61.08260750770569}, {43.36511194705963,58.46077799797058}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.623473167419434,57.53249526023865}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.767215728759766,61.17992401123047}, {41.59303903579712,58.55809450149536}}),
		mapFloor::triangle({{41.623473167419434,57.53249526023865}, {41.67589247226715,55.451756715774536}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {43.63028407096863,61.08260750770569}, {45.53675353527069,60.98302602767944}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {40.02484083175659,69.40388083457947}, {34.70504581928253,68.40086579322815}}),
		mapFloor::triangle({{39.90415036678314,61.27724051475525}, {34.70504581928253,68.40086579322815}, {29.38525080680847,67.39785075187683}}),mapFloor::triangle({{29.38525080680847,67.39785075187683}, {28.205865621566772,61.695897579193115}, {39.90415036678314,61.27724051475525}}),mapFloor::triangle({{20.650091767311096,69.33240294456482}, {17.685143649578094,64.39388394355774}, {20.662133395671844,63.6304497718811}}),
		mapFloor::triangle({{23.242035508155823,68.62598657608032}, {20.662133395671844,63.6304497718811}, {23.416775465011597,62.924033403396606}}),mapFloor::triangle({{27.748265862464905,67.39785075187683}, {23.416775465011597,62.924033403396606}, {28.205865621566772,61.695897579193115}}),mapFloor::triangle({{23.143833875656128,61.77178621292114}, {20.527951419353485,61.07513904571533}, {22.772544622421265,60.49951910972595}}),
		mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.585645735263824,62.40820288658142}, {23.143833875656128,61.77178621292114}}),mapFloor::triangle({{51.958876848220825,60.639023780822754}, {57.256513833999634,71.34671211242676}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{56.5263569355011,61.679607629776}, {56.64258599281311,60.85101366043091}, {56.4550518989563,60.7355535030365}}),
		mapFloor::triangle({{57.37825632095337,67.95763969421387}, {58.750998973846436,70.36827802658081}, {58.1642746925354,67.64544248580933}}),mapFloor::triangle({{58.1642746925354,67.64544248580933}, {59.65898036956787,69.77382302284241}, {59.01072025299072,67.23819971084595}}),mapFloor::triangle({{59.01072025299072,67.23819971084595}, {60.56696176528931,69.179368019104}, {59.75006818771362,66.61676168441772}}),
		mapFloor::triangle({{59.75006818771362,66.61676168441772}, {61.47494316101074,68.5849130153656}, {60.76385974884033,66.20283126831055}}),mapFloor::triangle({{60.76385974884033,66.20283126831055}, {62.38292455673218,67.9904580116272}, {61.504554748535156,65.94322919845581}}),mapFloor::triangle({{61.504554748535156,65.94322919845581}, {63.29090595245361,67.39600896835327}, {62.26264238357544,65.54231643676758}}),
		mapFloor::triangle({{62.26264238357544,65.54231643676758}, {64.19888734817505,66.80155992507935}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {60.23549437522888,61.15959882736206}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {60.588109493255615,61.661553382873535}, {61.7917001247406,63.37488889694214}}),
		mapFloor::triangle({{60.23549437522888,61.15959882736206}, {60.206109285354614,61.83493137359619}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{59.90486145019531,61.291855573654175}, {59.74782109260559,61.84876561164856}, {60.206109285354614,61.83493137359619}}),mapFloor::triangle({{59.4904899597168,61.24899387359619}, {59.34169888496399,61.43867373466492}, {59.74782109260559,61.84876561164856}}),
		mapFloor::triangle({{59.13337469100952,60.74079871177673}, {58.11628699302673,61.49156093597412}, {59.34169888496399,61.43867373466492}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {58.11628699302673,61.49156093597412}, {57.876962423324585,60.74079871177673}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {56.99082016944885,62.199777364730835}, {57.714200019836426,62.16696500778198}}),
		mapFloor::triangle({{56.8189263343811,61.40207052230835}, {56.7365825176239,61.759012937545776}, {56.99082016944885,62.199777364730835}}),mapFloor::triangle({{56.76974058151245,64.90197777748108}, {56.7365825176239,61.759012937545776}, {56.5263569355011,61.679607629776}}),mapFloor::triangle({{61.09514832496643,60.5864942073822}, {60.90986728668213,61.08654737472534}, {61.11719012260437,60.87304353713989}}),
		mapFloor::triangle({{62.91089653968811,61.24071478843689}, {61.778461933135986,61.2257182598114}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{57.69909620285034,71.05695605278015}, {57.01313018798828,68.12434196472168}, {57.256513833999634,71.34671211242676}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {56.76974058151245,64.90197777748108}, {56.891435384750366,66.51315689086914}}),
		mapFloor::triangle({{64.19888734817505,66.80155992507935}, {63.70786428451538,64.5002543926239}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {61.7917001247406,63.37488889694214}, {62.39349842071533,64.23155665397644}}),mapFloor::triangle({{60.588109493255615,61.661553382873535}, {61.23437285423279,63.688623905181885}, {61.7917001247406,63.37488889694214}}),
		mapFloor::triangle({{62.39349842071533,64.23155665397644}, {62.26264238357544,65.54231643676758}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{60.206109285354614,61.83493137359619}, {60.62619090080261,63.895994424819946}, {61.23437285423279,63.688623905181885}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {62.26264238357544,65.54231643676758}, {61.748504638671875,64.61547017097473}}),
		mapFloor::triangle({{59.74782109260559,61.84876561164856}, {60.0527822971344,63.82075548171997}, {60.62619090080261,63.895994424819946}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {60.76385974884033,66.20283126831055}, {61.504554748535156,65.94322919845581}}),mapFloor::triangle({{59.34169888496399,61.43867373466492}, {58.93318057060242,64.05416131019592}, {60.0527822971344,63.82075548171997}}),
		mapFloor::triangle({{59.34162139892578,65.33546447753906}, {60.76385974884033,66.20283126831055}, {60.40831804275513,65.01179337501526}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {58.93318057060242,64.05416131019592}, {58.11628699302673,61.49156093597412}}),mapFloor::triangle({{58.68659019470215,65.97039103507996}, {59.75006818771362,66.61676168441772}, {59.34162139892578,65.33546447753906}}),
		mapFloor::triangle({{57.714200019836426,62.16696500778198}, {57.57754445075989,64.92261290550232}, {58.362460136413574,64.70258235931396}}),mapFloor::triangle({{57.87091255187988,66.28402471542358}, {59.01072025299072,67.23819971084595}, {58.68659019470215,65.97039103507996}}),mapFloor::triangle({{56.99082016944885,62.199777364730835}, {57.057422399520874,64.85832929611206}, {57.57754445075989,64.92261290550232}}),
		mapFloor::triangle({{57.21783638000488,66.40798449516296}, {58.1642746925354,67.64544248580933}, {57.87091255187988,66.28402471542358}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {61.778461933135986,61.2257182598114}, {61.11719012260437,60.87304353713989}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {64.62193131446838,63.38502764701843}, {63.200199604034424,62.30537295341492}}),
		mapFloor::triangle({{65.106862783432,63.38502764701843}, {63.70786428451538,64.5002543926239}, {65.106862783432,66.20710492134094}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {63.200199604034424,62.30537295341492}, {61.778461933135986,61.2257182598114}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.21783638000488,66.40798449516296}, {57.87091255187988,66.28402471542358}}),
		mapFloor::triangle({{57.57754445075989,64.92261290550232}, {58.68659019470215,65.97039103507996}, {58.362460136413574,64.70258235931396}}),mapFloor::triangle({{58.362460136413574,64.70258235931396}, {59.34162139892578,65.33546447753906}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{60.0527822971344,63.82075548171997}, {59.34162139892578,65.33546447753906}, {60.40831804275513,65.01179337501526}}),
		mapFloor::triangle({{60.62619090080261,63.895994424819946}, {60.40831804275513,65.01179337501526}, {61.06537580490112,64.91961479187012}}),mapFloor::triangle({{61.23437285423279,63.688623905181885}, {61.06537580490112,64.91961479187012}, {61.748504638671875,64.61547017097473}}),mapFloor::triangle({{61.7917001247406,63.37488889694214}, {61.748504638671875,64.61547017097473}, {62.39349842071533,64.23155665397644}}),
		mapFloor::triangle({{63.70786428451538,64.5002543926239}, {62.39349842071533,64.23155665397644}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {56.891435384750366,66.51315689086914}, {57.01313018798828,68.12434196472168}}),mapFloor::triangle({{64.62193131446838,62.04128861427307}, {63.200199604034424,62.30537295341492}, {64.62193131446838,63.38502764701843}}),
		mapFloor::triangle({{62.91089653968811,61.24071478843689}, {64.62808847427368,61.03021502494812}, {63.03298473358154,60.283881425857544}}),mapFloor::triangle({{45.00240981578827,46.826037764549255}, {49.619314074516296,41.586390137672424}, {50.93286633491516,46.826037764549255}}),mapFloor::triangle({{44.412678480148315,42.49590039253235}, {48.96253943443298,38.96656632423401}, {49.619314074516296,41.586390137672424}}),
		mapFloor::triangle({{42.149168252944946,12.013623863458633}, {42.48851537704468,8.46017301082611}, {44.150835275650024,11.273302137851715}}),mapFloor::triangle({{44.150835275650024,11.273302137851715}, {45.2946662902832,7.711953669786453}, {46.090564131736755,10.248332470655441}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {18.17968785762787,78.09324860572815}, {18.538518249988556,78.84608507156372}}),
		mapFloor::triangle({{15.949025750160217,83.3631157875061}, {19.973839819431305,81.85744285583496}, {20.69150060415268,83.3631157875061}}),mapFloor::triangle({{15.086840093135834,80.89309930801392}, {18.538518249988556,78.84608507156372}, {19.25617903470993,80.35176396369934}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {19.25617903470993,80.35176396369934}, {19.973839819431305,81.85744285583496}}),
		mapFloor::triangle({{20.69150060415268,83.3631157875061}, {16.15099161863327,84.20861959457397}, {15.949025750160217,83.3631157875061}}),mapFloor::triangle({{14.440201222896576,79.04058694839478}, {17.82085746526718,77.34041213989258}, {18.17968785762787,78.09324860572815}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {16.46295189857483,84.92844104766846}, {16.15099161863327,84.20861959457397}}),
		mapFloor::triangle({{21.20542675256729,91.30561947822571}, {17.204593122005463,86.66783571243286}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{43.31970810890198,91.10780358314514}, {43.396878242492676,89.28470611572266}, {46.31889760494232,90.03100395202637}}),mapFloor::triangle({{27.62833833694458,87.53054738044739}, {24.609489738941193,90.78310132026672}, {25.388413667678833,87.0474636554718}}),
		mapFloor::triangle({{30.673453211784363,87.76944875717163}, {28.789418935775757,90.86838364601135}, {27.62833833694458,87.53054738044739}}),mapFloor::triangle({{42.08822846412659,91.76114797592163}, {40.47485888004303,88.53840827941895}, {43.396878242492676,89.28470611572266}}),mapFloor::triangle({{39.47590887546539,91.0868763923645}, {31.772249937057495,87.6875102519989}, {40.47485888004303,88.37026357650757}}),
		mapFloor::triangle({{46.55798375606537,90.55352210998535}, {43.23611855506897,92.87440180778503}, {43.360134959220886,89.93344306945801}}),mapFloor::triangle({{31.772249937057495,87.6875102519989}, {31.322526931762695,90.79188704490662}, {30.673453211784363,87.60130405426025}}),mapFloor::triangle({{39.47590887546539,91.0868763923645}, {32.28745460510254,91.10910892486572}, {32.23657011985779,90.824955701828}}),
		mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.470725774765015,60.76837778091431}, {52.54521369934082,60.92062592506409}}),mapFloor::triangle({{55.73093891143799,60.606199502944946}, {55.56737780570984,54.31789755821228}, {52.307164669036865,54.441070556640625}}),mapFloor::triangle({{52.307164669036865,54.441070556640625}, {55.56737780570984,54.31789755821228}, {55.33130764961243,50.12750029563904}}),
		mapFloor::triangle({{13.686150312423706,76.88034772872925}, {13.39118778705597,76.03532671928406}, {17.43103861808777,75.66371560096741}}),mapFloor::triangle({{9.941238909959793,78.42307090759277}, {10.32191589474678,76.88034772872925}, {13.686150312423706,76.88034772872925}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {8.154606074094772,76.03532671928406}, {13.39118778705597,76.03532671928406}}),
		mapFloor::triangle({{16.15099161863327,84.20861959457397}, {16.46295189857483,84.92844104766846}, {10.064070671796799,84.92844104766846}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {20.69150060415268,83.3631157875061}, {28.136953711509705,83.36310982704163}}),mapFloor::triangle({{40.47485888004303,89.06092643737793}, {27.62833833694458,88.05306553840637}, {28.92724573612213,86.10114455223083}}),
		mapFloor::triangle({{28.92724573612213,86.10114455223083}, {28.139975666999817,86.10114455223083}, {28.338921070098877,84.2086136341095}}),mapFloor::triangle({{41.99438989162445,87.10901141166687}, {47.37803041934967,88.8326108455658}, {45.81670165061951,90.5623197555542}}),mapFloor::triangle({{10.02856194972992,79.23346757888794}, {9.941238909959793,78.42307090759277}, {14.22465443611145,78.42307686805725}}),
		mapFloor::triangle({{53.61122488975525,94.16074752807617}, {52.675724029541016,93.55491995811462}, {58.36923122406006,92.49657392501831}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {16.15099161863327,84.20861959457397}, {10.180553793907166,84.20861959457397}}),mapFloor::triangle({{21.20542675256729,86.10115051269531}, {20.893466472625732,84.20861959457397}, {28.338921070098877,84.2086136341095}}),
		mapFloor::triangle({{25.388413667678833,87.56998181343079}, {24.609489738941193,91.30561947822571}, {21.20542675256729,91.30561947822571}}),mapFloor::triangle({{51.74022316932678,92.94909238815308}, {43.25721859931946,92.51015782356262}, {47.37803041934967,88.8326108455658}}),mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,86.66783571243286}, {21.20542675256729,86.10115051269531}}),
		mapFloor::triangle({{52.675724029541016,93.55491995811462}, {51.74022316932678,92.94909238815308}, {54.53476905822754,90.83240032196045}}),mapFloor::triangle({{57.62477517127991,96.7599093914032}, {53.61122488975525,94.16074752807617}, {61.028093099594116,94.16074752807617}}),mapFloor::triangle({{59.35288071632385,99.9901294708252}, {57.62477517127991,96.7599093914032}, {64.7293746471405,96.7599093914032}}),
		mapFloor::triangle({{13.39118778705597,76.03532671928406}, {8.154606074094772,76.03532671928406}, {9.973200410604477,75.59431195259094}}),mapFloor::triangle({{17.43103861808777,75.66371560096741}, {13.39118778705597,76.03532671928406}, {13.463491201400757,74.69298839569092}}),mapFloor::triangle({{12.89408802986145,73.65986108779907}, {12.324683368206024,72.6267397403717}, {14.676167070865631,72.58003950119019}}),
		mapFloor::triangle({{12.89408802986145,73.65986108779907}, {13.463491201400757,74.69298839569092}, {9.973200410604477,75.59431195259094}}),mapFloor::triangle({{12.324683368206024,72.6267397403717}, {12.89408802986145,73.65986108779907}, {9.973200410604477,74.13387298583984}}),mapFloor::triangle({{13.463491201400757,74.69298839569092}, {12.89408802986145,73.65986108779907}, {15.814974904060364,73.1858491897583}}),
		mapFloor::triangle({{8.0686554312706,73.3474850654602}, {8.0686554312706,74.13387298583984}, {6.317555904388428,74.13387298583984}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {9.28720235824585,74.13387298583984}, {9.28720310330391,73.3474850654602}}),mapFloor::triangle({{9.28720235824585,74.13387298583984}, {8.0686554312706,74.13387298583984}, {8.0686554312706,73.3474850654602}}),
		mapFloor::triangle({{15.517932176589966,82.12810754776001}, {9.216467291116714,82.12810754776001}, {10.088422894477844,81.11355304718018}}),mapFloor::triangle({{10.088422894477844,81.11355304718018}, {9.491559863090515,80.04385828971863}, {14.655748009681702,79.65809106826782}}),mapFloor::triangle({{9.978587925434113,83.3631157875061}, {9.216467291116714,82.12810754776001}, {15.517932176589966,82.12810754776001}}),
		mapFloor::triangle({{14.655748009681702,79.65809106826782}, {9.491559863090515,80.04385828971863}, {10.02856194972992,79.23346757888794}}),mapFloor::triangle({{17.204593122005463,86.66783571243286}, {17.120999097824097,84.92844104766846}, {21.20542675256729,86.10115051269531}}),mapFloor::triangle({{17.204593122005463,91.30561947822571}, {9.905359148979187,91.30561947822571}, {9.905359148979187,86.66783571243286}}),
		mapFloor::triangle({{27.62833833694458,88.05306553840637}, {25.388413667678833,87.56998181343079}, {26.234343647956848,86.10115051269531}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {24.609489738941193,98.65127205848694}, {21.20542675256729,98.65127205848694}}),mapFloor::triangle({{24.609489738941193,91.30561947822571}, {24.609489738941193,95.42139768600464}, {21.20542675256729,95.42139768600464}}),
		mapFloor::triangle({{24.609489738941193,95.42139768600464}, {31.748607754707336,94.19344663619995}, {31.748607754707336,97.42332100868225}}),mapFloor::triangle({{31.748607754707336,97.42332100868225}, {31.748607754707336,94.19344663619995}, {32.05464780330658,94.19344663619995}}),mapFloor::triangle({{32.14395046234131,97.42332100868225}, {32.05464780330658,94.19344663619995}, {33.30938220024109,94.19344663619995}}),
		mapFloor::triangle({{32.05464780330658,94.19344663619995}, {32.05464780330658,93.16425919532776}, {33.30938220024109,93.16425919532776}}),mapFloor::triangle({{33.30938220024109,93.16425919532776}, {42.084380984306335,93.16425919532776}, {42.084380984306335,94.19344663619995}}),mapFloor::triangle({{33.30938220024109,94.19344663619995}, {42.084380984306335,94.19344663619995}, {42.084380984306335,97.42332100868225}}),
		mapFloor::triangle({{41.99438989162445,87.10901141166687}, {45.34463584423065,70.40689587593079}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{51.75238847732544,58.21966528892517}, {45.17846703529358,58.36119055747986}, {45.356035232543945,56.528520584106445}}),mapFloor::triangle({{45.356035232543945,56.528520584106445}, {45.00240981578827,46.826037764549255}, {50.93286633491516,46.826037764549255}}),
		mapFloor::triangle({{48.96253943443298,38.96656632423401}, {42.99716353416443,42.59541928768158}, {41.62573218345642,36.34674251079559}}),mapFloor::triangle({{41.62573218345642,36.34674251079559}, {37.69223392009735,22.020843625068665}, {44.15292143821716,21.22199535369873}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {34.45371687412262,6.660129129886627}, {40.346384048461914,6.569939851760864}}),
		mapFloor::triangle({{34.45371687412262,7.009121775627136}, {34.00723934173584,5.720734223723412}, {37.78216540813446,5.662957951426506}}),mapFloor::triangle({{37.69223392009735,22.020843625068665}, {35.72110831737518,12.671534717082977}, {42.149168252944946,12.013623863458633}}),mapFloor::triangle({{17.539416253566742,10.173983126878738}, {18.049949407577515,13.920833170413971}, {10.580939054489136,14.448870718479156}}),
		mapFloor::triangle({{17.226578295230865,7.878036797046661}, {17.539416253566742,10.173983126878738}, {10.186772793531418,10.693792998790741}}),mapFloor::triangle({{35.72110831737518,12.671534717082977}, {34.93525981903076,8.944147825241089}, {41.436946392059326,8.740557730197906}}),mapFloor::triangle({{2.6035303249955177,15.305614471435547}, {-0.05893282359465957,15.59155285358429}, {-0.058932637330144644,11.836475133895874}}),
		mapFloor::triangle({{9.181087464094162,14.599210023880005}, {5.261003226041794,15.020212531089783}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{10.580939054489136,14.448870718479156}, {9.181087464094162,14.599210023880005}, {8.838780224323273,10.84413155913353}}),mapFloor::triangle({{8.838780224323273,10.84413155913353}, {5.063920095562935,11.265134066343307}, {5.360473692417145,7.395584881305695}}),
		mapFloor::triangle({{5.261003226041794,15.020212531089783}, {2.6035303249955177,15.305614471435547}, {2.5048965588212013,11.550536751747131}}),mapFloor::triangle({{2.5048965588212013,11.550536751747131}, {2.3879943415522575,7.385682314634323}, {3.8800302892923355,7.334820926189423}}),mapFloor::triangle({{17.983587086200714,7.824518531560898}, {18.30383837223053,10.119940340518951}, {17.539416253566742,10.173983126878738}}),
		mapFloor::triangle({{18.30383837223053,10.119940340518951}, {18.826469779014587,13.865935802459717}, {18.049949407577515,13.920833170413971}}),mapFloor::triangle({{9.94524136185646,8.392805606126785}, {9.743194282054901,7.305775582790375}, {17.024531960487366,7.054728269577026}}),mapFloor::triangle({{19.813525676727295,10.013210028409958}, {20.360049605369568,13.7575164437294}, {18.826469779014587,13.865935802459717}}),
		mapFloor::triangle({{19.478632509708405,7.718823105096817}, {19.813525676727295,10.013210028409958}, {18.30383837223053,10.119940340518951}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {24.016912281513214,9.716042876243591}, {23.916561901569366,7.4050746858119965}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {26.035165786743164,13.356302678585052}, {24.62996244430542,13.455647230148315}}),
		mapFloor::triangle({{19.478632509708405,7.718823105096817}, {17.983587086200714,7.824518531560898}, {17.983587086200714,6.906026601791382}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {19.478632509708405,6.800331175327301}, {23.641256988048553,6.506045907735825}}),mapFloor::triangle({{23.916561901569366,7.4050746858119965}, {24.016912281513214,9.716042876243591}, {19.813525676727295,10.013210028409958}}),
		mapFloor::triangle({{24.016912281513214,9.716042876243591}, {24.62996244430542,13.455647230148315}, {20.360049605369568,13.7575164437294}}),mapFloor::triangle({{23.641256988048553,6.506045907735825}, {19.478632509708405,6.800331175327301}, {19.478632509708405,2.204103022813797}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {35.72110831737518,12.671534717082977}, {31.293559074401855,12.98454999923706}}),
		mapFloor::triangle({{34.45371687412262,6.660129129886627}, {34.93525981903076,8.944147825241089}, {30.576688051223755,9.252285957336426}}),mapFloor::triangle({{30.576688051223755,9.252285957336426}, {31.293559074401855,12.98454999923706}, {26.035165786743164,13.356302678585052}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {30.576688051223755,9.252285957336426}, {25.400224328041077,9.61824655532837}}),
		mapFloor::triangle({{30.129551887512207,5.877119302749634}, {25.277110934257507,6.220173090696335}, {25.383812189102173,1.6636453568935394}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.25157630443573,7.3106929659843445}, {25.277110934257507,6.220173090696335}}),mapFloor::triangle({{34.00723934173584,5.720734223723412}, {34.45371687412262,7.009121775627136}, {29.75088655948639,7.119341939687729}}),
		mapFloor::triangle({{42.48851537704468,8.46017301082611}, {41.397953033447266,6.289554387331009}, {43.88707876205444,5.625864863395691}}),mapFloor::triangle({{41.436946392059326,8.740557730197906}, {40.346384048461914,6.569939851760864}, {41.397953033447266,6.289554387331009}}),mapFloor::triangle({{41.397953033447266,6.289554387331009}, {41.23776853084564,5.181708186864853}, {43.4712290763855,4.633805900812149}}),
		mapFloor::triangle({{45.34463584423065,70.40689587593079}, {45.53675353527069,60.98302602767944}, {51.958876848220825,60.639023780822754}}),mapFloor::triangle({{45.53675353527069,60.98302602767944}, {45.17846703529358,58.36119055747986}, {51.75238847732544,58.21966528892517}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {39.90415036678314,61.27724051475525}, {39.820969104766846,58.655405044555664}}),
		mapFloor::triangle({{41.59303903579712,58.55809450149536}, {39.820969104766846,58.655405044555664}, {39.820969104766846,58.016884326934814}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {43.36511194705963,58.46077799797058}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {45.53675353527069,60.98302602767944}, {43.63028407096863,61.08260750770569}}),
		mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.59303903579712,58.55809450149536}, {41.623473167419434,57.53249526023865}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {43.63028407096863,61.08260750770569}, {41.767215728759766,61.17992401123047}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {40.02484083175659,69.40388083457947}, {43.63028407096863,61.08260750770569}}),
		mapFloor::triangle({{41.767215728759766,61.17992401123047}, {43.63028407096863,61.08260750770569}, {40.02484083175659,69.40388083457947}}),mapFloor::triangle({{39.90415036678314,61.27724051475525}, {41.767215728759766,61.17992401123047}, {34.70504581928253,68.40086579322815}}),mapFloor::triangle({{29.38525080680847,67.39785075187683}, {27.748265862464905,67.39785075187683}, {28.205865621566772,61.695897579193115}}),
		mapFloor::triangle({{20.650091767311096,69.33240294456482}, {17.84893423318863,70.09583711624146}, {17.685143649578094,64.39388394355774}}),mapFloor::triangle({{23.242035508155823,68.62598657608032}, {20.650091767311096,69.33240294456482}, {20.662133395671844,63.6304497718811}}),mapFloor::triangle({{27.748265862464905,67.39785075187683}, {23.242035508155823,68.62598657608032}, {23.416775465011597,62.924033403396606}}),
		mapFloor::triangle({{23.143833875656128,61.77178621292114}, {20.585645735263824,62.40820288658142}, {20.527951419353485,61.07513904571533}}),mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.662133395671844,63.6304497718811}, {20.585645735263824,62.40820288658142}}),mapFloor::triangle({{51.958876848220825,60.639023780822754}, {56.4550518989563,60.7355535030365}, {57.256513833999634,71.34671211242676}}),
		mapFloor::triangle({{56.5263569355011,61.679607629776}, {56.7365825176239,61.759012937545776}, {56.64258599281311,60.85101366043091}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {57.69909620285034,71.05695605278015}, {58.750998973846436,70.36827802658081}}),mapFloor::triangle({{58.1642746925354,67.64544248580933}, {58.750998973846436,70.36827802658081}, {59.65898036956787,69.77382302284241}}),
		mapFloor::triangle({{59.01072025299072,67.23819971084595}, {59.65898036956787,69.77382302284241}, {60.56696176528931,69.179368019104}}),mapFloor::triangle({{59.75006818771362,66.61676168441772}, {60.56696176528931,69.179368019104}, {61.47494316101074,68.5849130153656}}),mapFloor::triangle({{60.76385974884033,66.20283126831055}, {61.47494316101074,68.5849130153656}, {62.38292455673218,67.9904580116272}}),
		mapFloor::triangle({{61.504554748535156,65.94322919845581}, {62.38292455673218,67.9904580116272}, {63.29090595245361,67.39600896835327}}),mapFloor::triangle({{62.26264238357544,65.54231643676758}, {63.29090595245361,67.39600896835327}, {64.19888734817505,66.80155992507935}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {60.50000190734863,60.5864942073822}, {60.23549437522888,61.15959882736206}}),
		mapFloor::triangle({{62.308865785598755,62.79339790344238}, {60.90986728668213,61.08654737472534}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{60.23549437522888,61.15959882736206}, {59.90486145019531,61.291855573654175}, {60.206109285354614,61.83493137359619}}),mapFloor::triangle({{59.90486145019531,61.291855573654175}, {59.4904899597168,61.24899387359619}, {59.74782109260559,61.84876561164856}}),
		mapFloor::triangle({{59.4904899597168,61.24899387359619}, {59.13337469100952,60.74079871177673}, {59.34169888496399,61.43867373466492}}),mapFloor::triangle({{59.13337469100952,60.74079871177673}, {57.876962423324585,60.74079871177673}, {58.11628699302673,61.49156093597412}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {57.714200019836426,62.16696500778198}, {58.11628699302673,61.49156093597412}}),
		mapFloor::triangle({{57.5242817401886,61.42411231994629}, {56.8189263343811,61.40207052230835}, {56.99082016944885,62.199777364730835}}),mapFloor::triangle({{56.8189263343811,61.40207052230835}, {56.64258599281311,60.85101366043091}, {56.7365825176239,61.759012937545776}}),mapFloor::triangle({{56.76974058151245,64.90197777748108}, {57.057422399520874,64.85832929611206}, {56.7365825176239,61.759012937545776}}),
		mapFloor::triangle({{61.09514832496643,60.5864942073822}, {60.50000190734863,60.5864942073822}, {60.90986728668213,61.08654737472534}}),mapFloor::triangle({{62.91089653968811,61.24071478843689}, {62.21930980682373,60.91712713241577}, {61.778461933135986,61.2257182598114}}),mapFloor::triangle({{57.69909620285034,71.05695605278015}, {57.37825632095337,67.95763969421387}, {57.01313018798828,68.12434196472168}}),
		mapFloor::triangle({{57.21783638000488,66.40798449516296}, {57.057422399520874,64.85832929611206}, {56.76974058151245,64.90197777748108}}),mapFloor::triangle({{64.19888734817505,66.80155992507935}, {65.106862783432,66.20710492134094}, {63.70786428451538,64.5002543926239}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {62.308865785598755,62.79339790344238}, {61.7917001247406,63.37488889694214}}),
		mapFloor::triangle({{60.588109493255615,61.661553382873535}, {60.206109285354614,61.83493137359619}, {61.23437285423279,63.688623905181885}}),mapFloor::triangle({{62.39349842071533,64.23155665397644}, {61.748504638671875,64.61547017097473}, {62.26264238357544,65.54231643676758}}),mapFloor::triangle({{60.206109285354614,61.83493137359619}, {59.74782109260559,61.84876561164856}, {60.62619090080261,63.895994424819946}}),
		mapFloor::triangle({{61.06537580490112,64.91961479187012}, {61.504554748535156,65.94322919845581}, {62.26264238357544,65.54231643676758}}),mapFloor::triangle({{59.74782109260559,61.84876561164856}, {59.34169888496399,61.43867373466492}, {60.0527822971344,63.82075548171997}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {60.40831804275513,65.01179337501526}, {60.76385974884033,66.20283126831055}}),
		mapFloor::triangle({{59.34169888496399,61.43867373466492}, {58.11628699302673,61.49156093597412}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{59.34162139892578,65.33546447753906}, {59.75006818771362,66.61676168441772}, {60.76385974884033,66.20283126831055}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {58.362460136413574,64.70258235931396}, {58.93318057060242,64.05416131019592}}),
		mapFloor::triangle({{58.68659019470215,65.97039103507996}, {59.01072025299072,67.23819971084595}, {59.75006818771362,66.61676168441772}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {56.99082016944885,62.199777364730835}, {57.57754445075989,64.92261290550232}}),mapFloor::triangle({{57.87091255187988,66.28402471542358}, {58.1642746925354,67.64544248580933}, {59.01072025299072,67.23819971084595}}),
		mapFloor::triangle({{56.99082016944885,62.199777364730835}, {56.7365825176239,61.759012937545776}, {57.057422399520874,64.85832929611206}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {57.37825632095337,67.95763969421387}, {58.1642746925354,67.64544248580933}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {62.308865785598755,62.79339790344238}, {61.778461933135986,61.2257182598114}}),
		mapFloor::triangle({{63.00836801528931,63.64682912826538}, {63.70786428451538,64.5002543926239}, {64.62193131446838,63.38502764701843}}),mapFloor::triangle({{65.106862783432,63.38502764701843}, {64.62193131446838,63.38502764701843}, {63.70786428451538,64.5002543926239}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {63.00836801528931,63.64682912826538}, {63.200199604034424,62.30537295341492}}),
		mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.057422399520874,64.85832929611206}, {57.21783638000488,66.40798449516296}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.87091255187988,66.28402471542358}, {58.68659019470215,65.97039103507996}}),mapFloor::triangle({{58.362460136413574,64.70258235931396}, {58.68659019470215,65.97039103507996}, {59.34162139892578,65.33546447753906}}),
		mapFloor::triangle({{60.0527822971344,63.82075548171997}, {58.93318057060242,64.05416131019592}, {59.34162139892578,65.33546447753906}}),mapFloor::triangle({{60.62619090080261,63.895994424819946}, {60.0527822971344,63.82075548171997}, {60.40831804275513,65.01179337501526}}),mapFloor::triangle({{61.23437285423279,63.688623905181885}, {60.62619090080261,63.895994424819946}, {61.06537580490112,64.91961479187012}}),
		mapFloor::triangle({{61.7917001247406,63.37488889694214}, {61.23437285423279,63.688623905181885}, {61.748504638671875,64.61547017097473}}),mapFloor::triangle({{63.70786428451538,64.5002543926239}, {63.00836801528931,63.64682912826538}, {62.39349842071533,64.23155665397644}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {57.21783638000488,66.40798449516296}, {56.891435384750366,66.51315689086914}}),
		mapFloor::triangle({{64.62193131446838,62.04128861427307}, {62.91089653968811,61.24071478843689}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{62.91089653968811,61.24071478843689}, {64.62193131446838,62.04128861427307}, {64.62808847427368,61.03021502494812}}),mapFloor::triangle({{45.00240981578827,46.826037764549255}, {44.412678480148315,42.49590039253235}, {49.619314074516296,41.586390137672424}}),
		mapFloor::triangle({{44.412678480148315,42.49590039253235}, {42.99716353416443,42.59541928768158}, {48.96253943443298,38.96656632423401}}),mapFloor::triangle({{42.149168252944946,12.013623863458633}, {41.436946392059326,8.740557730197906}, {42.48851537704468,8.46017301082611}}),mapFloor::triangle({{44.150835275650024,11.273302137851715}, {42.48851537704468,8.46017301082611}, {45.2946662902832,7.711953669786453}}),
		mapFloor::triangle({{14.655748009681702,79.65809106826782}, {14.440201222896576,79.04058694839478}, {18.17968785762787,78.09324860572815}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {15.517932176589966,82.12810754776001}, {19.973839819431305,81.85744285583496}}),mapFloor::triangle({{15.086840093135834,80.89309930801392}, {14.655748009681702,79.65809106826782}, {18.538518249988556,78.84608507156372}}),
		mapFloor::triangle({{15.517932176589966,82.12810754776001}, {15.086840093135834,80.89309930801392}, {19.25617903470993,80.35176396369934}}),mapFloor::triangle({{20.69150060415268,83.3631157875061}, {20.893466472625732,84.20861959457397}, {16.15099161863327,84.20861959457397}}),mapFloor::triangle({{14.440201222896576,79.04058694839478}, {14.22465443611145,78.42307686805725}, {17.82085746526718,77.34041213989258}}),
		mapFloor::triangle({{20.893466472625732,84.20861959457397}, {21.20542675256729,86.10115051269531}, {16.46295189857483,84.92844104766846}}),mapFloor::triangle({{21.20542675256729,91.30561947822571}, {17.204593122005463,91.30561947822571}, {17.204593122005463,86.66783571243286}}),mapFloor::triangle({{43.31970810890198,91.10780358314514}, {42.573365569114685,90.84309339523315}, {43.396878242492676,89.28470611572266}}),
		mapFloor::triangle({{27.62833833694458,87.53054738044739}, {28.789418935775757,90.86838364601135}, {24.609489738941193,90.78310132026672}}),mapFloor::triangle({{30.673453211784363,87.76944875717163}, {31.322526931762695,90.960031747818}, {28.789418935775757,90.86838364601135}}),mapFloor::triangle({{42.08822846412659,91.76114797592163}, {39.47590887546539,91.25502109527588}, {40.47485888004303,88.53840827941895}}),
		mapFloor::triangle({{39.47590887546539,91.0868763923645}, {32.23657011985779,90.824955701828}, {31.772249937057495,87.6875102519989}}),mapFloor::triangle({{46.55798375606537,90.55352210998535}, {47.16920554637909,92.1568751335144}, {43.23611855506897,92.87440180778503}}),mapFloor::triangle({{31.772249937057495,87.6875102519989}, {32.23657011985779,90.824955701828}, {31.322526931762695,90.79188704490662}}),
		mapFloor::triangle({{39.47590887546539,91.0868763923645}, {39.526793360710144,91.37102961540222}, {32.28745460510254,91.10910892486572}}),}), true, {}),
						mapFloor("Stone", List<mapFloor::triangle>({ mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.54521369934082,60.92062592506409}, {55.698639154434204,60.87729334831238}}),mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.307164669036865,54.441070556640625}, {52.470725774765015,60.76837778091431}}),mapFloor::triangle({{52.307164669036865,54.441070556640625}, {55.33130764961243,50.12750029563904}, {52.176278829574585,50.17045736312866}}),
			mapFloor::triangle({{20.893466472625732,84.20861959457397}, {28.136953711509705,83.36310982704163}, {28.338921070098877,84.2086136341095}}),mapFloor::triangle({{40.47485888004303,89.06092643737793}, {28.92724573612213,86.10114455223083}, {41.99438989162445,87.10901141166687}}),mapFloor::triangle({{28.92724573612213,86.10114455223083}, {28.338921070098877,84.2086136341095}, {29.184192419052124,84.2086136341095}}),
			mapFloor::triangle({{41.99438989162445,87.10901141166687}, {45.81670165061951,90.5623197555542}, {40.43306112289429,88.83872032165527}}),mapFloor::triangle({{53.61122488975525,94.16074752807617}, {58.36923122406006,92.49657392501831}, {61.028093099594116,94.16074752807617}}),mapFloor::triangle({{21.20542675256729,86.10115051269531}, {28.338921070098877,84.2086136341095}, {28.139975666999817,86.10114455223083}}),
			mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,91.30561947822571}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{51.74022316932678,92.94909238815308}, {47.37803041934967,88.8326108455658}, {54.53476905822754,90.83240032196045}}),mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,86.10115051269531}, {26.234343647956848,86.10115051269531}}),
			mapFloor::triangle({{39.47590887546539,91.25502109527588}, {31.322526931762695,91.07525944709778}, {31.322526931762695,90.960031747818}}),mapFloor::triangle({{52.675724029541016,93.55491995811462}, {54.53476905822754,90.83240032196045}, {58.36923122406006,92.49657392501831}}),mapFloor::triangle({{57.62477517127991,96.7599093914032}, {61.028093099594116,94.16074752807617}, {64.7293746471405,96.7599093914032}}),
			mapFloor::triangle({{59.35288071632385,99.9901294708252}, {64.7293746471405,96.7599093914032}, {65.64862132072449,99.9901294708252}}),mapFloor::triangle({{11.588496714830399,73.87173771858215}, {14.676167070865631,72.58003950119019}, {15.814974904060364,73.1858491897583}}),mapFloor::triangle({{12.05345094203949,75.05711317062378}, {15.814974904060364,73.1858491897583}, {17.30959415435791,75.18734335899353}}),
			mapFloor::triangle({{17.204593122005463,91.30561947822571}, {9.905359148979187,86.66783571243286}, {17.204593122005463,86.66783571243286}}),mapFloor::triangle({{27.62833833694458,88.05306553840637}, {26.234343647956848,86.10115051269531}, {28.92724573612213,86.10114455223083}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {21.20542675256729,98.65127205848694}, {21.20542675256729,95.42139768600464}}),
			mapFloor::triangle({{24.609489738941193,91.30561947822571}, {21.20542675256729,95.42139768600464}, {21.20542675256729,91.30561947822571}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {31.748607754707336,97.42332100868225}, {24.609489738941193,98.65127205848694}}),mapFloor::triangle({{31.748607754707336,97.42332100868225}, {32.05464780330658,94.19344663619995}, {32.14395046234131,97.42332100868225}}),
			mapFloor::triangle({{41.99438989162445,87.10901141166687}, {50.72827339172363,72.13049530982971}, {47.37803041934967,88.8326108455658}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {51.60804986953735,56.528520584106445}, {51.75238847732544,58.21966528892517}}),mapFloor::triangle({{44.92802321910858,56.528520584106445}, {50.93286633491516,46.826037764549255}, {51.60804986953735,56.528520584106445}}),
			mapFloor::triangle({{48.96253943443298,38.96656632423401}, {41.62573218345642,36.34674251079559}, {48.30576181411743,36.34674251079559}}),mapFloor::triangle({{41.62573218345642,36.34674251079559}, {44.15292143821716,21.22199535369873}, {48.30576181411743,36.34674251079559}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {40.346384048461914,6.569939851760864}, {41.436946392059326,8.740557730197906}}),
			mapFloor::triangle({{34.45371687412262,7.009121775627136}, {37.78216540813446,5.662957951426506}, {38.22864294052124,6.951345503330231}}),mapFloor::triangle({{37.69223392009735,22.020843625068665}, {42.149168252944946,12.013623863458633}, {44.15292143821716,21.22199535369873}}),mapFloor::triangle({{17.539416253566742,10.173983126878738}, {10.580939054489136,14.448870718479156}, {10.186772793531418,10.693792998790741}}),
			mapFloor::triangle({{17.226578295230865,7.878036797046661}, {10.186772793531418,10.693792998790741}, {9.94524136185646,8.392805606126785}}),mapFloor::triangle({{35.72110831737518,12.671534717082977}, {41.436946392059326,8.740557730197906}, {42.149168252944946,12.013623863458633}}),mapFloor::triangle({{2.6035303249955177,15.305614471435547}, {-0.058932637330144644,11.836475133895874}, {2.5048965588212013,11.550536751747131}}),
			mapFloor::triangle({{9.181087464094162,14.599210023880005}, {5.063920095562935,11.265134066343307}, {8.838780224323273,10.84413155913353}}),mapFloor::triangle({{10.580939054489136,14.448870718479156}, {8.838780224323273,10.84413155913353}, {10.186772793531418,10.693792998790741}}),mapFloor::triangle({{8.838780224323273,10.84413155913353}, {5.360473692417145,7.395584881305695}, {8.542226254940033,7.395584881305695}}),
			mapFloor::triangle({{5.261003226041794,15.020212531089783}, {2.5048965588212013,11.550536751747131}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{2.5048965588212013,11.550536751747131}, {3.8800302892923355,7.334820926189423}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{17.983587086200714,7.824518531560898}, {17.539416253566742,10.173983126878738}, {17.226578295230865,7.878036797046661}}),
			mapFloor::triangle({{18.30383837223053,10.119940340518951}, {18.049949407577515,13.920833170413971}, {17.539416253566742,10.173983126878738}}),mapFloor::triangle({{9.94524136185646,8.392805606126785}, {17.024531960487366,7.054728269577026}, {17.226578295230865,7.878036797046661}}),mapFloor::triangle({{19.813525676727295,10.013210028409958}, {18.826469779014587,13.865935802459717}, {18.30383837223053,10.119940340518951}}),
			mapFloor::triangle({{19.478632509708405,7.718823105096817}, {18.30383837223053,10.119940340518951}, {17.983587086200714,7.824518531560898}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {23.916561901569366,7.4050746858119965}, {25.25157630443573,7.3106929659843445}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {24.62996244430542,13.455647230148315}, {24.016912281513214,9.716042876243591}}),
			mapFloor::triangle({{19.478632509708405,7.718823105096817}, {17.983587086200714,6.906026601791382}, {19.478632509708405,6.800331175327301}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {23.641256988048553,6.506045907735825}, {23.916561901569366,7.4050746858119965}}),mapFloor::triangle({{23.916561901569366,7.4050746858119965}, {19.813525676727295,10.013210028409958}, {19.478632509708405,7.718823105096817}}),
			mapFloor::triangle({{24.016912281513214,9.716042876243591}, {20.360049605369568,13.7575164437294}, {19.813525676727295,10.013210028409958}}),mapFloor::triangle({{23.641256988048553,6.506045907735825}, {19.478632509708405,2.204103022813797}, {23.641256988048553,1.9098177552223206}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {31.293559074401855,12.98454999923706}, {30.576688051223755,9.252285957336426}}),
			mapFloor::triangle({{34.45371687412262,6.660129129886627}, {30.576688051223755,9.252285957336426}, {30.247318744659424,6.957508623600006}}),mapFloor::triangle({{30.576688051223755,9.252285957336426}, {26.035165786743164,13.356302678585052}, {25.400224328041077,9.61824655532837}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.400224328041077,9.61824655532837}, {25.25157630443573,7.3106929659843445}}),
			mapFloor::triangle({{30.129551887512207,5.877119302749634}, {25.383812189102173,1.6636453568935394}, {29.63748276233673,1.3629233464598656}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.277110934257507,6.220173090696335}, {30.129551887512207,5.877119302749634}}),mapFloor::triangle({{34.00723934173584,5.720734223723412}, {29.75088655948639,7.119341939687729}, {29.30440902709961,5.830954387784004}}),
			mapFloor::triangle({{42.48851537704468,8.46017301082611}, {43.88707876205444,5.625864863395691}, {45.2946662902832,7.711953669786453}}),mapFloor::triangle({{41.436946392059326,8.740557730197906}, {41.397953033447266,6.289554387331009}, {42.48851537704468,8.46017301082611}}),mapFloor::triangle({{41.397953033447266,6.289554387331009}, {43.4712290763855,4.633805900812149}, {43.88707876205444,5.625864863395691}}),
			mapFloor::triangle({{45.34463584423065,70.40689587593079}, {51.958876848220825,60.639023780822754}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{45.53675353527069,60.98302602767944}, {51.75238847732544,58.21966528892517}, {51.958876848220825,60.639023780822754}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {39.820969104766846,58.655405044555664}, {41.59303903579712,58.55809450149536}}),
			mapFloor::triangle({{41.59303903579712,58.55809450149536}, {39.820969104766846,58.016884326934814}, {41.623473167419434,57.53249526023865}}),mapFloor::triangle({{44.92802321910858,56.528520584106445}, {43.36511194705963,58.46077799797058}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {43.63028407096863,61.08260750770569}, {43.36511194705963,58.46077799797058}}),
			mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.623473167419434,57.53249526023865}, {43.199437856674194,56.56546354293823}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.767215728759766,61.17992401123047}, {41.59303903579712,58.55809450149536}}),mapFloor::triangle({{41.623473167419434,57.53249526023865}, {41.67589247226715,55.451756715774536}, {43.199437856674194,56.56546354293823}}),
			mapFloor::triangle({{45.34463584423065,70.40689587593079}, {43.63028407096863,61.08260750770569}, {45.53675353527069,60.98302602767944}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {40.02484083175659,69.40388083457947}, {34.70504581928253,68.40086579322815}}),mapFloor::triangle({{39.90415036678314,61.27724051475525}, {34.70504581928253,68.40086579322815}, {29.38525080680847,67.39785075187683}}),
			mapFloor::triangle({{29.38525080680847,67.39785075187683}, {28.205865621566772,61.695897579193115}, {39.90415036678314,61.27724051475525}}),mapFloor::triangle({{20.650091767311096,69.33240294456482}, {17.685143649578094,64.39388394355774}, {20.662133395671844,63.6304497718811}}),mapFloor::triangle({{23.242035508155823,68.62598657608032}, {20.662133395671844,63.6304497718811}, {23.416775465011597,62.924033403396606}}),
			mapFloor::triangle({{27.748265862464905,67.39785075187683}, {23.416775465011597,62.924033403396606}, {28.205865621566772,61.695897579193115}}),mapFloor::triangle({{23.416775465011597,61.7017924785614}, {20.662133395671844,59.18592810630798}, {23.416775465011597,58.479511737823486}}),mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.662133395671844,62.40820288658142}, {23.416775465011597,61.7017924785614}}),
			mapFloor::triangle({{23.416775465011597,58.479511737823486}, {27.07330882549286,60.94868183135986}, {23.416775465011597,61.7017924785614}}),mapFloor::triangle({{51.958876848220825,60.639023780822754}, {57.256513833999634,71.34671211242676}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{56.5263569355011,61.679607629776}, {56.64258599281311,60.85101366043091}, {56.4550518989563,60.7355535030365}}),
			mapFloor::triangle({{57.37825632095337,67.95763969421387}, {58.750998973846436,70.36827802658081}, {58.1642746925354,67.64544248580933}}),mapFloor::triangle({{58.1642746925354,67.64544248580933}, {59.65898036956787,69.77382302284241}, {59.01072025299072,67.23819971084595}}),mapFloor::triangle({{59.01072025299072,67.23819971084595}, {60.56696176528931,69.179368019104}, {59.75006818771362,66.61676168441772}}),
			mapFloor::triangle({{59.75006818771362,66.61676168441772}, {61.47494316101074,68.5849130153656}, {60.76385974884033,66.20283126831055}}),mapFloor::triangle({{60.76385974884033,66.20283126831055}, {62.38292455673218,67.9904580116272}, {61.504554748535156,65.94322919845581}}),mapFloor::triangle({{61.504554748535156,65.94322919845581}, {63.29090595245361,67.39600896835327}, {62.26264238357544,65.54231643676758}}),
			mapFloor::triangle({{62.26264238357544,65.54231643676758}, {64.19888734817505,66.80155992507935}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {60.23549437522888,61.15959882736206}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {60.588109493255615,61.661553382873535}, {61.7917001247406,63.37488889694214}}),
			mapFloor::triangle({{60.23549437522888,61.15959882736206}, {60.206109285354614,61.83493137359619}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{59.90486145019531,61.291855573654175}, {59.74782109260559,61.84876561164856}, {60.206109285354614,61.83493137359619}}),mapFloor::triangle({{59.4904899597168,61.24899387359619}, {59.34169888496399,61.43867373466492}, {59.74782109260559,61.84876561164856}}),
			mapFloor::triangle({{59.13337469100952,60.74079871177673}, {58.11628699302673,61.49156093597412}, {59.34169888496399,61.43867373466492}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {58.11628699302673,61.49156093597412}, {57.876962423324585,60.74079871177673}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {56.99082016944885,62.199777364730835}, {57.714200019836426,62.16696500778198}}),
			mapFloor::triangle({{56.8189263343811,61.40207052230835}, {56.7365825176239,61.759012937545776}, {56.99082016944885,62.199777364730835}}),mapFloor::triangle({{56.76974058151245,64.90197777748108}, {56.7365825176239,61.759012937545776}, {56.5263569355011,61.679607629776}}),mapFloor::triangle({{61.09514832496643,60.5864942073822}, {60.90986728668213,61.08654737472534}, {61.11719012260437,60.87304353713989}}),
			mapFloor::triangle({{62.91089653968811,61.24071478843689}, {61.778461933135986,61.2257182598114}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{57.69909620285034,71.05695605278015}, {57.01313018798828,68.12434196472168}, {57.256513833999634,71.34671211242676}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {56.76974058151245,64.90197777748108}, {56.891435384750366,66.51315689086914}}),
			mapFloor::triangle({{64.19888734817505,66.80155992507935}, {63.70786428451538,64.5002543926239}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {61.7917001247406,63.37488889694214}, {62.39349842071533,64.23155665397644}}),mapFloor::triangle({{60.588109493255615,61.661553382873535}, {61.23437285423279,63.688623905181885}, {61.7917001247406,63.37488889694214}}),
			mapFloor::triangle({{62.39349842071533,64.23155665397644}, {62.26264238357544,65.54231643676758}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{60.206109285354614,61.83493137359619}, {60.62619090080261,63.895994424819946}, {61.23437285423279,63.688623905181885}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {62.26264238357544,65.54231643676758}, {61.748504638671875,64.61547017097473}}),
			mapFloor::triangle({{59.74782109260559,61.84876561164856}, {60.0527822971344,63.82075548171997}, {60.62619090080261,63.895994424819946}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {60.76385974884033,66.20283126831055}, {61.504554748535156,65.94322919845581}}),mapFloor::triangle({{59.34169888496399,61.43867373466492}, {58.93318057060242,64.05416131019592}, {60.0527822971344,63.82075548171997}}),
			mapFloor::triangle({{59.34162139892578,65.33546447753906}, {60.76385974884033,66.20283126831055}, {60.40831804275513,65.01179337501526}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {58.93318057060242,64.05416131019592}, {58.11628699302673,61.49156093597412}}),mapFloor::triangle({{58.68659019470215,65.97039103507996}, {59.75006818771362,66.61676168441772}, {59.34162139892578,65.33546447753906}}),
			mapFloor::triangle({{57.714200019836426,62.16696500778198}, {57.57754445075989,64.92261290550232}, {58.362460136413574,64.70258235931396}}),mapFloor::triangle({{57.87091255187988,66.28402471542358}, {59.01072025299072,67.23819971084595}, {58.68659019470215,65.97039103507996}}),mapFloor::triangle({{56.99082016944885,62.199777364730835}, {57.057422399520874,64.85832929611206}, {57.57754445075989,64.92261290550232}}),
			mapFloor::triangle({{57.21783638000488,66.40798449516296}, {58.1642746925354,67.64544248580933}, {57.87091255187988,66.28402471542358}}),mapFloor::triangle({{60.90986728668213,61.08654737472534}, {61.778461933135986,61.2257182598114}, {61.11719012260437,60.87304353713989}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {64.62193131446838,63.38502764701843}, {63.200199604034424,62.30537295341492}}),
			mapFloor::triangle({{65.106862783432,63.38502764701843}, {63.70786428451538,64.5002543926239}, {65.106862783432,66.20710492134094}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {63.200199604034424,62.30537295341492}, {61.778461933135986,61.2257182598114}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.21783638000488,66.40798449516296}, {57.87091255187988,66.28402471542358}}),
			mapFloor::triangle({{57.57754445075989,64.92261290550232}, {58.68659019470215,65.97039103507996}, {58.362460136413574,64.70258235931396}}),mapFloor::triangle({{58.362460136413574,64.70258235931396}, {59.34162139892578,65.33546447753906}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{60.0527822971344,63.82075548171997}, {59.34162139892578,65.33546447753906}, {60.40831804275513,65.01179337501526}}),
			mapFloor::triangle({{60.62619090080261,63.895994424819946}, {60.40831804275513,65.01179337501526}, {61.06537580490112,64.91961479187012}}),mapFloor::triangle({{61.23437285423279,63.688623905181885}, {61.06537580490112,64.91961479187012}, {61.748504638671875,64.61547017097473}}),mapFloor::triangle({{61.7917001247406,63.37488889694214}, {61.748504638671875,64.61547017097473}, {62.39349842071533,64.23155665397644}}),
			mapFloor::triangle({{63.70786428451538,64.5002543926239}, {62.39349842071533,64.23155665397644}, {62.995290756225586,65.08822441101074}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {56.891435384750366,66.51315689086914}, {57.01313018798828,68.12434196472168}}),mapFloor::triangle({{64.62193131446838,62.04128861427307}, {63.200199604034424,62.30537295341492}, {64.62193131446838,63.38502764701843}}),
			mapFloor::triangle({{45.00240981578827,46.826037764549255}, {49.619314074516296,41.586390137672424}, {50.93286633491516,46.826037764549255}}),mapFloor::triangle({{44.412678480148315,42.49590039253235}, {48.96253943443298,38.96656632423401}, {49.619314074516296,41.586390137672424}}),mapFloor::triangle({{42.149168252944946,12.013623863458633}, {42.48851537704468,8.46017301082611}, {44.150835275650024,11.273302137851715}}),
			mapFloor::triangle({{44.150835275650024,11.273302137851715}, {45.2946662902832,7.711953669786453}, {46.090564131736755,10.248332470655441}}),mapFloor::triangle({{21.20542675256729,91.30561947822571}, {17.204593122005463,86.66783571243286}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{43.31970810890198,91.10780358314514}, {43.396878242492676,89.28470611572266}, {46.31889760494232,90.03100395202637}}),
			mapFloor::triangle({{27.62833833694458,87.53054738044739}, {24.609489738941193,90.78310132026672}, {25.388413667678833,87.0474636554718}}),mapFloor::triangle({{30.673453211784363,87.76944875717163}, {28.789418935775757,90.86838364601135}, {27.62833833694458,87.53054738044739}}),mapFloor::triangle({{42.08822846412659,91.76114797592163}, {40.47485888004303,88.53840827941895}, {43.396878242492676,89.28470611572266}}),
			mapFloor::triangle({{40.47485888004303,88.53840827941895}, {31.322526931762695,90.960031747818}, {30.673453211784363,87.76944875717163}}),mapFloor::triangle({{46.55798375606537,90.55352210998535}, {43.23611855506897,92.87440180778503}, {43.360134959220886,89.93344306945801}}),mapFloor::triangle({{17.30959415435791,75.18734335899353}, {17.136812210083008,73.74606132507324}, {17.79475510120392,74.62713122367859}}),
			mapFloor::triangle({{55.73093891143799,60.606199502944946}, {52.470725774765015,60.76837778091431}, {52.54521369934082,60.92062592506409}}),mapFloor::triangle({{55.73093891143799,60.606199502944946}, {55.56737780570984,54.31789755821228}, {52.307164669036865,54.441070556640625}}),mapFloor::triangle({{52.307164669036865,54.441070556640625}, {55.56737780570984,54.31789755821228}, {55.33130764961243,50.12750029563904}}),
			mapFloor::triangle({{20.893466472625732,84.20861959457397}, {20.69150060415268,83.3631157875061}, {28.136953711509705,83.36310982704163}}),mapFloor::triangle({{40.47485888004303,89.06092643737793}, {27.62833833694458,88.05306553840637}, {28.92724573612213,86.10114455223083}}),mapFloor::triangle({{28.92724573612213,86.10114455223083}, {28.139975666999817,86.10114455223083}, {28.338921070098877,84.2086136341095}}),
			mapFloor::triangle({{41.99438989162445,87.10901141166687}, {47.37803041934967,88.8326108455658}, {45.81670165061951,90.5623197555542}}),mapFloor::triangle({{53.61122488975525,94.16074752807617}, {52.675724029541016,93.55491995811462}, {58.36923122406006,92.49657392501831}}),mapFloor::triangle({{21.20542675256729,86.10115051269531}, {20.893466472625732,84.20861959457397}, {28.338921070098877,84.2086136341095}}),
			mapFloor::triangle({{25.388413667678833,87.56998181343079}, {24.609489738941193,91.30561947822571}, {21.20542675256729,91.30561947822571}}),mapFloor::triangle({{51.74022316932678,92.94909238815308}, {43.25721859931946,92.51015782356262}, {47.37803041934967,88.8326108455658}}),mapFloor::triangle({{25.388413667678833,87.56998181343079}, {21.20542675256729,86.66783571243286}, {21.20542675256729,86.10115051269531}}),
			mapFloor::triangle({{39.47590887546539,91.25502109527588}, {39.47590887546539,91.37024879455566}, {31.322526931762695,91.07525944709778}}),mapFloor::triangle({{52.675724029541016,93.55491995811462}, {51.74022316932678,92.94909238815308}, {54.53476905822754,90.83240032196045}}),mapFloor::triangle({{57.62477517127991,96.7599093914032}, {53.61122488975525,94.16074752807617}, {61.028093099594116,94.16074752807617}}),
			mapFloor::triangle({{59.35288071632385,99.9901294708252}, {57.62477517127991,96.7599093914032}, {64.7293746471405,96.7599093914032}}),mapFloor::triangle({{11.588496714830399,73.87173771858215}, {11.37804388999939,72.64554500579834}, {14.676167070865631,72.58003950119019}}),mapFloor::triangle({{12.05345094203949,75.05711317062378}, {11.588496714830399,73.87173771858215}, {15.814974904060364,73.1858491897583}}),
			mapFloor::triangle({{17.204593122005463,91.30561947822571}, {9.905359148979187,91.30561947822571}, {9.905359148979187,86.66783571243286}}),mapFloor::triangle({{27.62833833694458,88.05306553840637}, {25.388413667678833,87.56998181343079}, {26.234343647956848,86.10115051269531}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {24.609489738941193,98.65127205848694}, {21.20542675256729,98.65127205848694}}),
			mapFloor::triangle({{24.609489738941193,91.30561947822571}, {24.609489738941193,95.42139768600464}, {21.20542675256729,95.42139768600464}}),mapFloor::triangle({{24.609489738941193,95.42139768600464}, {31.748607754707336,94.19344663619995}, {31.748607754707336,97.42332100868225}}),mapFloor::triangle({{31.748607754707336,97.42332100868225}, {31.748607754707336,94.19344663619995}, {32.05464780330658,94.19344663619995}}),
			mapFloor::triangle({{41.99438989162445,87.10901141166687}, {45.34463584423065,70.40689587593079}, {50.72827339172363,72.13049530982971}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {44.92802321910858,56.528520584106445}, {51.60804986953735,56.528520584106445}}),mapFloor::triangle({{44.92802321910858,56.528520584106445}, {45.00240981578827,46.826037764549255}, {50.93286633491516,46.826037764549255}}),
			mapFloor::triangle({{48.96253943443298,38.96656632423401}, {42.99716353416443,42.59541928768158}, {41.62573218345642,36.34674251079559}}),mapFloor::triangle({{41.62573218345642,36.34674251079559}, {37.69223392009735,22.020843625068665}, {44.15292143821716,21.22199535369873}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {34.45371687412262,6.660129129886627}, {40.346384048461914,6.569939851760864}}),
			mapFloor::triangle({{34.45371687412262,7.009121775627136}, {34.00723934173584,5.720734223723412}, {37.78216540813446,5.662957951426506}}),mapFloor::triangle({{37.69223392009735,22.020843625068665}, {35.72110831737518,12.671534717082977}, {42.149168252944946,12.013623863458633}}),mapFloor::triangle({{17.539416253566742,10.173983126878738}, {18.049949407577515,13.920833170413971}, {10.580939054489136,14.448870718479156}}),
			mapFloor::triangle({{17.226578295230865,7.878036797046661}, {17.539416253566742,10.173983126878738}, {10.186772793531418,10.693792998790741}}),mapFloor::triangle({{35.72110831737518,12.671534717082977}, {34.93525981903076,8.944147825241089}, {41.436946392059326,8.740557730197906}}),mapFloor::triangle({{2.6035303249955177,15.305614471435547}, {-0.05893282359465957,15.59155285358429}, {-0.058932637330144644,11.836475133895874}}),
			mapFloor::triangle({{9.181087464094162,14.599210023880005}, {5.261003226041794,15.020212531089783}, {5.063920095562935,11.265134066343307}}),mapFloor::triangle({{10.580939054489136,14.448870718479156}, {9.181087464094162,14.599210023880005}, {8.838780224323273,10.84413155913353}}),mapFloor::triangle({{8.838780224323273,10.84413155913353}, {5.063920095562935,11.265134066343307}, {5.360473692417145,7.395584881305695}}),
			mapFloor::triangle({{5.261003226041794,15.020212531089783}, {2.6035303249955177,15.305614471435547}, {2.5048965588212013,11.550536751747131}}),mapFloor::triangle({{2.5048965588212013,11.550536751747131}, {2.3879943415522575,7.385682314634323}, {3.8800302892923355,7.334820926189423}}),mapFloor::triangle({{17.983587086200714,7.824518531560898}, {18.30383837223053,10.119940340518951}, {17.539416253566742,10.173983126878738}}),
			mapFloor::triangle({{18.30383837223053,10.119940340518951}, {18.826469779014587,13.865935802459717}, {18.049949407577515,13.920833170413971}}),mapFloor::triangle({{9.94524136185646,8.392805606126785}, {9.743194282054901,7.305775582790375}, {17.024531960487366,7.054728269577026}}),mapFloor::triangle({{19.813525676727295,10.013210028409958}, {20.360049605369568,13.7575164437294}, {18.826469779014587,13.865935802459717}}),
			mapFloor::triangle({{19.478632509708405,7.718823105096817}, {19.813525676727295,10.013210028409958}, {18.30383837223053,10.119940340518951}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {24.016912281513214,9.716042876243591}, {23.916561901569366,7.4050746858119965}}),mapFloor::triangle({{25.400224328041077,9.61824655532837}, {26.035165786743164,13.356302678585052}, {24.62996244430542,13.455647230148315}}),
			mapFloor::triangle({{19.478632509708405,7.718823105096817}, {17.983587086200714,7.824518531560898}, {17.983587086200714,6.906026601791382}}),mapFloor::triangle({{19.478632509708405,7.718823105096817}, {19.478632509708405,6.800331175327301}, {23.641256988048553,6.506045907735825}}),mapFloor::triangle({{23.916561901569366,7.4050746858119965}, {24.016912281513214,9.716042876243591}, {19.813525676727295,10.013210028409958}}),
			mapFloor::triangle({{24.016912281513214,9.716042876243591}, {24.62996244430542,13.455647230148315}, {20.360049605369568,13.7575164437294}}),mapFloor::triangle({{23.641256988048553,6.506045907735825}, {19.478632509708405,6.800331175327301}, {19.478632509708405,2.204103022813797}}),mapFloor::triangle({{34.93525981903076,8.944147825241089}, {35.72110831737518,12.671534717082977}, {31.293559074401855,12.98454999923706}}),
			mapFloor::triangle({{34.45371687412262,6.660129129886627}, {34.93525981903076,8.944147825241089}, {30.576688051223755,9.252285957336426}}),mapFloor::triangle({{30.576688051223755,9.252285957336426}, {31.293559074401855,12.98454999923706}, {26.035165786743164,13.356302678585052}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {30.576688051223755,9.252285957336426}, {25.400224328041077,9.61824655532837}}),
			mapFloor::triangle({{30.129551887512207,5.877119302749634}, {25.277110934257507,6.220173090696335}, {25.383812189102173,1.6636453568935394}}),mapFloor::triangle({{30.247318744659424,6.957508623600006}, {25.25157630443573,7.3106929659843445}, {25.277110934257507,6.220173090696335}}),mapFloor::triangle({{34.00723934173584,5.720734223723412}, {34.45371687412262,7.009121775627136}, {29.75088655948639,7.119341939687729}}),
			mapFloor::triangle({{42.48851537704468,8.46017301082611}, {41.397953033447266,6.289554387331009}, {43.88707876205444,5.625864863395691}}),mapFloor::triangle({{41.436946392059326,8.740557730197906}, {40.346384048461914,6.569939851760864}, {41.397953033447266,6.289554387331009}}),mapFloor::triangle({{41.397953033447266,6.289554387331009}, {41.23776853084564,5.181708186864853}, {43.4712290763855,4.633805900812149}}),
			mapFloor::triangle({{45.34463584423065,70.40689587593079}, {45.53675353527069,60.98302602767944}, {51.958876848220825,60.639023780822754}}),mapFloor::triangle({{45.53675353527069,60.98302602767944}, {45.17846703529358,58.36119055747986}, {51.75238847732544,58.21966528892517}}),mapFloor::triangle({{41.767215728759766,61.17992401123047}, {39.90415036678314,61.27724051475525}, {39.820969104766846,58.655405044555664}}),
			mapFloor::triangle({{41.59303903579712,58.55809450149536}, {39.820969104766846,58.655405044555664}, {39.820969104766846,58.016884326934814}}),mapFloor::triangle({{44.92802321910858,56.528520584106445}, {45.17846703529358,58.36119055747986}, {43.36511194705963,58.46077799797058}}),mapFloor::triangle({{45.17846703529358,58.36119055747986}, {45.53675353527069,60.98302602767944}, {43.63028407096863,61.08260750770569}}),
			mapFloor::triangle({{43.36511194705963,58.46077799797058}, {41.59303903579712,58.55809450149536}, {41.623473167419434,57.53249526023865}}),mapFloor::triangle({{43.36511194705963,58.46077799797058}, {43.63028407096863,61.08260750770569}, {41.767215728759766,61.17992401123047}}),mapFloor::triangle({{45.34463584423065,70.40689587593079}, {40.02484083175659,69.40388083457947}, {43.63028407096863,61.08260750770569}}),
			mapFloor::triangle({{41.767215728759766,61.17992401123047}, {43.63028407096863,61.08260750770569}, {40.02484083175659,69.40388083457947}}),mapFloor::triangle({{39.90415036678314,61.27724051475525}, {41.767215728759766,61.17992401123047}, {34.70504581928253,68.40086579322815}}),mapFloor::triangle({{29.38525080680847,67.39785075187683}, {27.748265862464905,67.39785075187683}, {28.205865621566772,61.695897579193115}}),
			mapFloor::triangle({{20.650091767311096,69.33240294456482}, {17.84893423318863,70.09583711624146}, {17.685143649578094,64.39388394355774}}),mapFloor::triangle({{23.242035508155823,68.62598657608032}, {20.650091767311096,69.33240294456482}, {20.662133395671844,63.6304497718811}}),mapFloor::triangle({{27.748265862464905,67.39785075187683}, {23.242035508155823,68.62598657608032}, {23.416775465011597,62.924033403396606}}),
			mapFloor::triangle({{23.416775465011597,61.7017924785614}, {20.662133395671844,62.40820288658142}, {20.662133395671844,59.18592810630798}}),mapFloor::triangle({{23.416775465011597,62.924033403396606}, {20.662133395671844,63.6304497718811}, {20.662133395671844,62.40820288658142}}),mapFloor::triangle({{23.416775465011597,58.479511737823486}, {25.787505507469177,57.928454875946045}, {27.07330882549286,60.94868183135986}}),
			mapFloor::triangle({{51.958876848220825,60.639023780822754}, {56.4550518989563,60.7355535030365}, {57.256513833999634,71.34671211242676}}),mapFloor::triangle({{56.5263569355011,61.679607629776}, {56.7365825176239,61.759012937545776}, {56.64258599281311,60.85101366043091}}),mapFloor::triangle({{57.37825632095337,67.95763969421387}, {57.69909620285034,71.05695605278015}, {58.750998973846436,70.36827802658081}}),
			mapFloor::triangle({{58.1642746925354,67.64544248580933}, {58.750998973846436,70.36827802658081}, {59.65898036956787,69.77382302284241}}),mapFloor::triangle({{59.01072025299072,67.23819971084595}, {59.65898036956787,69.77382302284241}, {60.56696176528931,69.179368019104}}),mapFloor::triangle({{59.75006818771362,66.61676168441772}, {60.56696176528931,69.179368019104}, {61.47494316101074,68.5849130153656}}),
			mapFloor::triangle({{60.76385974884033,66.20283126831055}, {61.47494316101074,68.5849130153656}, {62.38292455673218,67.9904580116272}}),mapFloor::triangle({{61.504554748535156,65.94322919845581}, {62.38292455673218,67.9904580116272}, {63.29090595245361,67.39600896835327}}),mapFloor::triangle({{62.26264238357544,65.54231643676758}, {63.29090595245361,67.39600896835327}, {64.19888734817505,66.80155992507935}}),
			mapFloor::triangle({{60.90986728668213,61.08654737472534}, {60.50000190734863,60.5864942073822}, {60.23549437522888,61.15959882736206}}),mapFloor::triangle({{62.308865785598755,62.79339790344238}, {60.90986728668213,61.08654737472534}, {60.588109493255615,61.661553382873535}}),mapFloor::triangle({{60.23549437522888,61.15959882736206}, {59.90486145019531,61.291855573654175}, {60.206109285354614,61.83493137359619}}),
			mapFloor::triangle({{59.90486145019531,61.291855573654175}, {59.4904899597168,61.24899387359619}, {59.74782109260559,61.84876561164856}}),mapFloor::triangle({{59.4904899597168,61.24899387359619}, {59.13337469100952,60.74079871177673}, {59.34169888496399,61.43867373466492}}),mapFloor::triangle({{59.13337469100952,60.74079871177673}, {57.876962423324585,60.74079871177673}, {58.11628699302673,61.49156093597412}}),
			mapFloor::triangle({{57.5242817401886,61.42411231994629}, {57.714200019836426,62.16696500778198}, {58.11628699302673,61.49156093597412}}),mapFloor::triangle({{57.5242817401886,61.42411231994629}, {56.8189263343811,61.40207052230835}, {56.99082016944885,62.199777364730835}}),mapFloor::triangle({{56.8189263343811,61.40207052230835}, {56.64258599281311,60.85101366043091}, {56.7365825176239,61.759012937545776}}),
			mapFloor::triangle({{56.76974058151245,64.90197777748108}, {57.057422399520874,64.85832929611206}, {56.7365825176239,61.759012937545776}}),mapFloor::triangle({{61.09514832496643,60.5864942073822}, {60.50000190734863,60.5864942073822}, {60.90986728668213,61.08654737472534}}),mapFloor::triangle({{62.91089653968811,61.24071478843689}, {62.21930980682373,60.91712713241577}, {61.778461933135986,61.2257182598114}}),
			mapFloor::triangle({{57.69909620285034,71.05695605278015}, {57.37825632095337,67.95763969421387}, {57.01313018798828,68.12434196472168}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {57.057422399520874,64.85832929611206}, {56.76974058151245,64.90197777748108}}),mapFloor::triangle({{64.19888734817505,66.80155992507935}, {65.106862783432,66.20710492134094}, {63.70786428451538,64.5002543926239}}),
			mapFloor::triangle({{63.00836801528931,63.64682912826538}, {62.308865785598755,62.79339790344238}, {61.7917001247406,63.37488889694214}}),mapFloor::triangle({{60.588109493255615,61.661553382873535}, {60.206109285354614,61.83493137359619}, {61.23437285423279,63.688623905181885}}),mapFloor::triangle({{62.39349842071533,64.23155665397644}, {61.748504638671875,64.61547017097473}, {62.26264238357544,65.54231643676758}}),
			mapFloor::triangle({{60.206109285354614,61.83493137359619}, {59.74782109260559,61.84876561164856}, {60.62619090080261,63.895994424819946}}),mapFloor::triangle({{61.06537580490112,64.91961479187012}, {61.504554748535156,65.94322919845581}, {62.26264238357544,65.54231643676758}}),mapFloor::triangle({{59.74782109260559,61.84876561164856}, {59.34169888496399,61.43867373466492}, {60.0527822971344,63.82075548171997}}),
			mapFloor::triangle({{61.06537580490112,64.91961479187012}, {60.40831804275513,65.01179337501526}, {60.76385974884033,66.20283126831055}}),mapFloor::triangle({{59.34169888496399,61.43867373466492}, {58.11628699302673,61.49156093597412}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{59.34162139892578,65.33546447753906}, {59.75006818771362,66.61676168441772}, {60.76385974884033,66.20283126831055}}),
			mapFloor::triangle({{57.714200019836426,62.16696500778198}, {58.362460136413574,64.70258235931396}, {58.93318057060242,64.05416131019592}}),mapFloor::triangle({{58.68659019470215,65.97039103507996}, {59.01072025299072,67.23819971084595}, {59.75006818771362,66.61676168441772}}),mapFloor::triangle({{57.714200019836426,62.16696500778198}, {56.99082016944885,62.199777364730835}, {57.57754445075989,64.92261290550232}}),
			mapFloor::triangle({{57.87091255187988,66.28402471542358}, {58.1642746925354,67.64544248580933}, {59.01072025299072,67.23819971084595}}),mapFloor::triangle({{56.99082016944885,62.199777364730835}, {56.7365825176239,61.759012937545776}, {57.057422399520874,64.85832929611206}}),mapFloor::triangle({{57.21783638000488,66.40798449516296}, {57.37825632095337,67.95763969421387}, {58.1642746925354,67.64544248580933}}),
			mapFloor::triangle({{60.90986728668213,61.08654737472534}, {62.308865785598755,62.79339790344238}, {61.778461933135986,61.2257182598114}}),mapFloor::triangle({{63.00836801528931,63.64682912826538}, {63.70786428451538,64.5002543926239}, {64.62193131446838,63.38502764701843}}),mapFloor::triangle({{65.106862783432,63.38502764701843}, {64.62193131446838,63.38502764701843}, {63.70786428451538,64.5002543926239}}),
			mapFloor::triangle({{62.308865785598755,62.79339790344238}, {63.00836801528931,63.64682912826538}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.057422399520874,64.85832929611206}, {57.21783638000488,66.40798449516296}}),mapFloor::triangle({{57.57754445075989,64.92261290550232}, {57.87091255187988,66.28402471542358}, {58.68659019470215,65.97039103507996}}),
			mapFloor::triangle({{58.362460136413574,64.70258235931396}, {58.68659019470215,65.97039103507996}, {59.34162139892578,65.33546447753906}}),mapFloor::triangle({{60.0527822971344,63.82075548171997}, {58.93318057060242,64.05416131019592}, {59.34162139892578,65.33546447753906}}),mapFloor::triangle({{60.62619090080261,63.895994424819946}, {60.0527822971344,63.82075548171997}, {60.40831804275513,65.01179337501526}}),
			mapFloor::triangle({{61.23437285423279,63.688623905181885}, {60.62619090080261,63.895994424819946}, {61.06537580490112,64.91961479187012}}),mapFloor::triangle({{61.7917001247406,63.37488889694214}, {61.23437285423279,63.688623905181885}, {61.748504638671875,64.61547017097473}}),mapFloor::triangle({{63.70786428451538,64.5002543926239}, {63.00836801528931,63.64682912826538}, {62.39349842071533,64.23155665397644}}),
			mapFloor::triangle({{57.37825632095337,67.95763969421387}, {57.21783638000488,66.40798449516296}, {56.891435384750366,66.51315689086914}}),mapFloor::triangle({{64.62193131446838,62.04128861427307}, {62.91089653968811,61.24071478843689}, {63.200199604034424,62.30537295341492}}),mapFloor::triangle({{45.00240981578827,46.826037764549255}, {44.412678480148315,42.49590039253235}, {49.619314074516296,41.586390137672424}}),
			mapFloor::triangle({{44.412678480148315,42.49590039253235}, {42.99716353416443,42.59541928768158}, {48.96253943443298,38.96656632423401}}),mapFloor::triangle({{42.149168252944946,12.013623863458633}, {41.436946392059326,8.740557730197906}, {42.48851537704468,8.46017301082611}}),mapFloor::triangle({{44.150835275650024,11.273302137851715}, {42.48851537704468,8.46017301082611}, {45.2946662902832,7.711953669786453}}),
			mapFloor::triangle({{21.20542675256729,91.30561947822571}, {17.204593122005463,91.30561947822571}, {17.204593122005463,86.66783571243286}}),mapFloor::triangle({{43.31970810890198,91.10780358314514}, {42.573365569114685,90.84309339523315}, {43.396878242492676,89.28470611572266}}),mapFloor::triangle({{27.62833833694458,87.53054738044739}, {28.789418935775757,90.86838364601135}, {24.609489738941193,90.78310132026672}}),
			mapFloor::triangle({{30.673453211784363,87.76944875717163}, {31.322526931762695,90.960031747818}, {28.789418935775757,90.86838364601135}}),mapFloor::triangle({{42.08822846412659,91.76114797592163}, {39.47590887546539,91.25502109527588}, {40.47485888004303,88.53840827941895}}),mapFloor::triangle({{40.47485888004303,88.53840827941895}, {39.47590887546539,91.25502109527588}, {31.322526931762695,90.960031747818}}),
			mapFloor::triangle({{46.55798375606537,90.55352210998535}, {47.16920554637909,92.1568751335144}, {43.23611855506897,92.87440180778503}}),mapFloor::triangle({{17.30959415435791,75.18734335899353}, {15.814974904060364,73.1858491897583}, {17.136812210083008,73.74606132507324}}), }), false, Map<string, string>({ pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "STONE"}) })),
						mapFloor("Grass", List<mapFloor::triangle>({ mapFloor::triangle({{13.686150312423706,76.88034772872925}, {17.43103861808777,75.66371560096741}, {17.56890118122101,76.05867385864258}}),mapFloor::triangle({{9.941238909959793,78.42307090759277}, {13.686150312423706,76.88034772872925}, {14.22465443611145,78.42307686805725}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {13.39118778705597,76.03532671928406}, {13.686150312423706,76.88034772872925}}),
			mapFloor::triangle({{16.15099161863327,84.20861959457397}, {10.064070671796799,84.92844104766846}, {10.180553793907166,84.20861959457397}}),mapFloor::triangle({{10.02856194972992,79.23346757888794}, {14.22465443611145,78.42307686805725}, {14.440201222896576,79.04058694839478}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {10.180553793907166,84.20861959457397}, {9.978587925434113,83.3631157875061}}),
			mapFloor::triangle({{13.39118778705597,76.03532671928406}, {9.973200410604477,75.59431195259094}, {12.05345094203949,75.05711317062378}}),mapFloor::triangle({{13.39118778705597,76.03532671928406}, {17.30959415435791,75.18734335899353}, {17.43103861808777,75.66371560096741}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {12.05345094203949,75.05711317062378}, {9.973200410604477,75.59431195259094}}),
			mapFloor::triangle({{9.973200410604477,72.67343997955322}, {11.588496714830399,73.87173771858215}, {9.973200410604477,74.13387298583984}}),mapFloor::triangle({{8.0686554312706,73.3474850654602}, {6.317555904388428,74.13387298583984}, {6.317555904388428,72.67343997955322}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {9.28720310330391,73.3474850654602}, {9.973200410604477,72.67343997955322}}),
			mapFloor::triangle({{9.28720235824585,74.13387298583984}, {8.0686554312706,73.3474850654602}, {9.28720310330391,73.3474850654602}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {9.941238909959793,78.42307090759277}, {8.754055947065353,77.9451847076416}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {10.088422894477844,81.11355304718018}, {15.086840093135834,80.89309930801392}}),
			mapFloor::triangle({{10.088422894477844,81.11355304718018}, {14.655748009681702,79.65809106826782}, {15.086840093135834,80.89309930801392}}),mapFloor::triangle({{9.978587925434113,83.3631157875061}, {15.517932176589966,82.12810754776001}, {15.949025750160217,83.3631157875061}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {10.02856194972992,79.23346757888794}, {14.440201222896576,79.04058694839478}}),
			mapFloor::triangle({{17.204593122005463,86.66783571243286}, {21.20542675256729,86.10115051269531}, {21.20542675256729,86.66783571243286}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {18.17968785762787,78.09324860572815}, {18.538518249988556,78.84608507156372}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {19.973839819431305,81.85744285583496}, {20.69150060415268,83.3631157875061}}),
			mapFloor::triangle({{15.086840093135834,80.89309930801392}, {18.538518249988556,78.84608507156372}, {19.25617903470993,80.35176396369934}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {19.25617903470993,80.35176396369934}, {19.973839819431305,81.85744285583496}}),mapFloor::triangle({{20.69150060415268,83.3631157875061}, {16.15099161863327,84.20861959457397}, {15.949025750160217,83.3631157875061}}),
			mapFloor::triangle({{14.440201222896576,79.04058694839478}, {17.82085746526718,77.34041213989258}, {18.17968785762787,78.09324860572815}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {16.46295189857483,84.92844104766846}, {16.15099161863327,84.20861959457397}}),mapFloor::triangle({{13.686150312423706,76.88034772872925}, {13.39118778705597,76.03532671928406}, {17.43103861808777,75.66371560096741}}),
			mapFloor::triangle({{9.941238909959793,78.42307090759277}, {10.32191589474678,76.88034772872925}, {13.686150312423706,76.88034772872925}}),mapFloor::triangle({{10.32191589474678,76.88034772872925}, {8.154606074094772,76.03532671928406}, {13.39118778705597,76.03532671928406}}),mapFloor::triangle({{16.15099161863327,84.20861959457397}, {16.46295189857483,84.92844104766846}, {10.064070671796799,84.92844104766846}}),
			mapFloor::triangle({{10.02856194972992,79.23346757888794}, {9.941238909959793,78.42307090759277}, {14.22465443611145,78.42307686805725}}),mapFloor::triangle({{15.949025750160217,83.3631157875061}, {16.15099161863327,84.20861959457397}, {10.180553793907166,84.20861959457397}}),mapFloor::triangle({{13.39118778705597,76.03532671928406}, {8.154606074094772,76.03532671928406}, {9.973200410604477,75.59431195259094}}),
			mapFloor::triangle({{13.39118778705597,76.03532671928406}, {12.05345094203949,75.05711317062378}, {17.30959415435791,75.18734335899353}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {11.588496714830399,73.87173771858215}, {12.05345094203949,75.05711317062378}}),mapFloor::triangle({{9.973200410604477,72.67343997955322}, {11.37804388999939,72.64554500579834}, {11.588496714830399,73.87173771858215}}),
			mapFloor::triangle({{8.0686554312706,73.3474850654602}, {8.0686554312706,74.13387298583984}, {6.317555904388428,74.13387298583984}}),mapFloor::triangle({{9.973200410604477,74.13387298583984}, {9.28720235824585,74.13387298583984}, {9.28720310330391,73.3474850654602}}),mapFloor::triangle({{9.28720235824585,74.13387298583984}, {8.0686554312706,74.13387298583984}, {8.0686554312706,73.3474850654602}}),
			mapFloor::triangle({{15.517932176589966,82.12810754776001}, {9.216467291116714,82.12810754776001}, {10.088422894477844,81.11355304718018}}),mapFloor::triangle({{10.088422894477844,81.11355304718018}, {9.491559863090515,80.04385828971863}, {14.655748009681702,79.65809106826782}}),mapFloor::triangle({{9.978587925434113,83.3631157875061}, {9.216467291116714,82.12810754776001}, {15.517932176589966,82.12810754776001}}),
			mapFloor::triangle({{14.655748009681702,79.65809106826782}, {9.491559863090515,80.04385828971863}, {10.02856194972992,79.23346757888794}}),mapFloor::triangle({{17.204593122005463,86.66783571243286}, {17.120999097824097,84.92844104766846}, {21.20542675256729,86.10115051269531}}),mapFloor::triangle({{14.655748009681702,79.65809106826782}, {14.440201222896576,79.04058694839478}, {18.17968785762787,78.09324860572815}}),
			mapFloor::triangle({{15.949025750160217,83.3631157875061}, {15.517932176589966,82.12810754776001}, {19.973839819431305,81.85744285583496}}),mapFloor::triangle({{15.086840093135834,80.89309930801392}, {14.655748009681702,79.65809106826782}, {18.538518249988556,78.84608507156372}}),mapFloor::triangle({{15.517932176589966,82.12810754776001}, {15.086840093135834,80.89309930801392}, {19.25617903470993,80.35176396369934}}),
			mapFloor::triangle({{20.69150060415268,83.3631157875061}, {20.893466472625732,84.20861959457397}, {16.15099161863327,84.20861959457397}}),mapFloor::triangle({{14.440201222896576,79.04058694839478}, {14.22465443611145,78.42307686805725}, {17.82085746526718,77.34041213989258}}),mapFloor::triangle({{20.893466472625732,84.20861959457397}, {21.20542675256729,86.10115051269531}, {16.46295189857483,84.92844104766846}}), }), false, Map<string, string>({ pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "GRASS"}) })),
						mapFloor("Wood", List<mapFloor::triangle>({ mapFloor::triangle({{32.14395046234131,97.42332100868225}, {42.084380984306335,93.16425919532776}, {42.084380984306335,97.42332100868225}}),mapFloor::triangle({{62.91089653968811,61.24071478843689}, {64.63928818702698,59.19190049171448}, {63.254958391189575,58.54418873786926}}),mapFloor::triangle({{32.14395046234131,97.42332100868225}, {32.05464780330658,93.16425919532776}, {42.084380984306335,93.16425919532776}}),
						mapFloor::triangle({{62.91089653968811,61.24071478843689}, {64.62193131446838,62.04128861427307}, {64.63928818702698,59.19190049171448}}), }), false, Map<string, string>({ pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "WOOD"}) })),
						}), {}, { 5000, 5000 }, { Map<string, string>({
							pair<string,string>({"song1",to_string(TOWN_WAV_1) + " " + "MusicVolume"}),
							pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
					}) });
		maps["Tavern1"] = mapInstance("Tavern1", EMPTYMAP, { 50,50 }, List<mapObject>({
							mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
							mapObject("AT_Upstairs", true, false, false, "","0",0,0,"1","1.0","CENTRE",{44, 50}, false, List<mapFloor::triangle>({mapFloor::triangle({{44.866180419921875,51.85549855232239}, {43.43647360801697,52.36383080482483}, {43.921685218811035,51.41664147377014}}),mapFloor::triangle({{43.43647360801697,52.36383080482483}, {44.23273801803589,53.86872887611389}, {42.841193079948425,53.20360064506531}}),mapFloor::triangle({{44.866180419921875,51.85549855232239}, {44.82801854610443,53.02895903587341}, {43.43647360801697,52.36383080482483}}),
mapFloor::triangle({{43.43647360801697,52.36383080482483}, {44.82801854610443,53.02895903587341}, {44.23273801803589,53.86872887611389}}),}), Map<string, string>({
																	pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Upstairs"}),
																	pair<string, string>({"copy","AT_Upstairs"}),
																	pair<string, string>({"areaTransition","TavernUpstairsDay"}),
																	pair<string, string>({"direction","STAND_BACK"}),
																	pair<string, string>({"playerPosX","57"}),
																	pair<string, string>({"playerPosY","52"}),
																	pair<string, string>({"audio","WOODENSTAIRS"}),
																	pair<string, string>({"x","0"}),
																	pair<string, string>({"y","0"}),
																	pair<string, string>({"h","50"}),
																	pair<string, string>({"w","50"}),
																	pair<string, string>("colour", "WHITE"),
																	pair<string, string>("uniqueID", mapPopupTextID),
																	pair<string, string>("anchorStyle", "TOPLEFT"),
																	pair<string, string>("shadowColour", "BLACK"),
																	pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																	pair<string, string>("don'tLoadIfNot", "OudinDefeated"),
																	pair<string, string>({"format","LightText_20"}),
																		})),
							mapObject("Hernando Pizarro", true, true, false, imageLookup.getSequenceAsString("Hernando Pizarro","TAVERN_WAIT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{56, 48.5}, false, List<mapFloor::triangle>({mapFloor::triangle({{54.85587120056152,49.13839101791382}, {55.50876259803772,51.55308246612549}, {56.750309467315674,49.848872423172}}),mapFloor::triangle({{54.85587120056152,49.13839101791382}, {53.76160144805908,50.5159854888916}, {55.50876259803772,51.55308246612549}}),}), Map<string, string>({
														pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Hernando Pizarro"}),
														pair<string, string>({"copy","Hernando Pizarro"}),
														pair<string, string>({"cutscene","TavernHernando1"}),
														pair<string, string>({"x","4"}),
														pair<string, string>({"y","-5"}),
														pair<string, string>({"h","50"}),
														pair<string, string>({"w","50"}),
														pair<string, string>("colour", "WHITE"),
														pair<string, string>("uniqueID", mapPopupTextID),
														pair<string, string>("don'tLoadIfPlayer", "Hernando Pizarro"),
														pair<string, string>("anchorStyle", "TOPLEFT"),
														pair<string, string>("shadowColour", "BLACK"),
														pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
														pair<string, string>({"format","LightText_20"})})),
								mapObject("Gihat al-Din Jaqmaq", true, true, false, imageLookup.getSequenceAsString("Gihat al-Din Jaqmaq","TAVERN_WAIT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{54.5, 51}, false, List<mapFloor::triangle>({mapFloor::triangle({{53.69369983673096,50.6539523601532}, {54.89156246185303,53.317856788635254}, {55.93475103378296,51.88592076301575}}),mapFloor::triangle({{53.69369983673096,50.6539523601532}, {52.196645736694336,52.538615465164185}, {54.89156246185303,53.317856788635254}}),}), Map<string, string>({
														pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Gihat al-Din Jaqmaq"}),
														pair<string, string>({"copy","Gihat al-Din Jaqmaq"}),
														pair<string, string>({"cutscene","TavernGihat1"}),
														pair<string, string>({"x","4"}),
														pair<string, string>({"y","-5"}),
														pair<string, string>({"h","50"}),
														pair<string, string>({"w","50"}),
														pair<string, string>("colour", "WHITE"),
														pair<string, string>("uniqueID", mapPopupTextID),
														pair<string, string>("anchorStyle", "TOPLEFT"),
														pair<string, string>("shadowColour", "BLACK"),
														pair<string, string>("don'tLoadIfPlayer", "Gihat al-Din Jaqmaq"),
														pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
														pair<string, string>({"format","LightText_20"})})),
								mapObject("Olyver Sumner", true, true, false, imageLookup.getSequenceAsString("Olyver Sumner","TAVERN_WAIT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{50, 48.8}, false, List<mapFloor::triangle>({mapFloor::triangle({{49.690064787864685,49.232131242752075}, {50.1770555973053,51.677531003952026}, {51.40126347541809,49.997130036354065}}),mapFloor::triangle({{49.690064787864685,49.232131242752075}, {48.65306317806244,50.53762197494507}, {50.1770555973053,51.677531003952026}}),}), Map<string, string>({
														pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Olyver Sumner"}),
														pair<string, string>({"copy","Olyver Sumner"}),
														pair<string, string>({"cutscene","TavernOlyver1"}),
														pair<string, string>({"x","-17"}),
														pair<string, string>({"y","-5"}),
														pair<string, string>({"h","50"}),
														pair<string, string>({"w","50"}),
														pair<string, string>("colour", "WHITE"),
														pair<string, string>("uniqueID", mapPopupTextID),
														pair<string, string>("anchorStyle", "TOPLEFT"),
														pair<string, string>("shadowColour", "BLACK"),
														pair<string, string>("don'tLoadIfPlayer", "Olyver Sumner"),
														pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
														pair<string, string>({"format","LightText_20"})})),
								mapObject("Tianshun Song", true, true, false, imageLookup.getSequenceAsString("Tianshun Song","TAVERN_WAIT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{53.4, 46.7}, false, List<mapFloor::triangle>({mapFloor::triangle({{53.75606417655945,47.804465889930725}, {51.4295756816864,50.14353394508362}, {53.19821238517761,50.67419409751892}}),mapFloor::triangle({{53.75606417655945,47.804465889930725}, {51.983314752578735,46.987324953079224}, {51.4295756816864,50.14353394508362}}),}), Map<string, string>({
														pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Tianshun Song"}),
														pair<string, string>({"copy","Tianshun Song"}),
														pair<string, string>({"cutscene","TavernTianshun1"}),
														pair<string, string>({"x","-17"}),
														pair<string, string>({"y","-5"}),
														pair<string, string>({"h","50"}),
														pair<string, string>({"w","50"}),
														pair<string, string>("colour", "WHITE"),
														pair<string, string>("uniqueID", mapPopupTextID),
														pair<string, string>("anchorStyle", "TOPLEFT"),
														pair<string, string>("shadowColour", "BLACK"),
														pair<string, string>("don'tLoadIfPlayer", "Tianshun Song"),
														pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
														pair<string, string>({"format","LightText_20"})})),
								mapObject("Angela Fleuret", true, true, false, imageLookup.getSequenceAsString("Angela Fleuret","TAVERN_WAIT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{48, 51}, false, List<mapFloor::triangle>({mapFloor::triangle({{47.35563099384308,52.509766817092896}, {48.2275515794754,54.15087938308716}, {48.571962118148804,51.74139738082886}}),mapFloor::triangle({{48.2275515794754,54.15087938308716}, {49.37558174133301,51.93273425102234}, {48.571962118148804,51.74139738082886}}),mapFloor::triangle({{47.35563099384308,52.509766817092896}, {47.02636897563934,53.65135669708252}, {48.2275515794754,54.15087938308716}}),
				mapFloor::triangle({{48.2275515794754,54.15087938308716}, {49.03117120265961,54.34221625328064}, {49.37558174133301,51.93273425102234}}),}), Map<string, string>({
														pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Angela Fleuret"}),
														pair<string, string>({"copy","Angela Fleuret"}),
														pair<string, string>({"cutscene","TavernAngela1"}),
														pair<string, string>({"x","-17"}),
														pair<string, string>({"y","-5"}),
														pair<string, string>({"h","50"}),
														pair<string, string>({"w","50"}),
														pair<string, string>("colour", "WHITE"),
														pair<string, string>("uniqueID", mapPopupTextID),
														pair<string, string>("anchorStyle", "TOPLEFT"),
														pair<string, string>("shadowColour", "BLACK"),
														pair<string, string>("don'tLoadIfPlayer", "Angela Fleuret"),
														pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
														pair<string, string>({"format","LightText_20"})})),
								mapObject("Father Michelet Cutscene", true, false, false, "","0",0,0,"0","0","",{50, 53}, false, List<mapFloor::triangle>({mapFloor::triangle({{51.54890418052673,58.0529510974884}, {50.794535875320435,55.240654945373535}, {49.33348894119263,57.070523500442505}}),mapFloor::triangle({{51.43198370933533,56.87977075576782}, {50.575172901153564,53.68557572364807}, {48.915717005729675,55.76393008232117}}),mapFloor::triangle({{51.54890418052673,58.0529510974884}, {53.00514101982117,56.22187852859497}, {50.794535875320435,55.240654945373535}}),
				mapFloor::triangle({{51.43198370933533,56.87977075576782}, {53.085970878601074,54.80004549026489}, {50.575172901153564,53.68557572364807}}),}), Map<string, string>({
														pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Father Michelet"}),
														pair<string, string>({"copy","Father Michelet Cutscene"}),
														pair<string, string>({"cutscene","TavernMichelet1"}),
														pair<string, string>({"x","-10"}),
														pair<string, string>({"y","-7"}),
														pair<string, string>({"h","50"}),
														pair<string, string>({"w","50"}),
														pair<string, string>("colour", "WHITE"),
														pair<string, string>("uniqueID", mapPopupTextID),
														pair<string, string>("anchorStyle", "TOPLEFT"),
														pair<string, string>("shadowColour", "BLACK"),
														pair<string, string>("don'tLoadIf", "IntroFinished"),
														pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
														pair<string, string>({"format","LightText_20"})})),
									mapObject("Fireplace", false, true, false, imageLookup.getSequenceAsString("Fireplace2","STAND_FRONT"),"1",20,imageLookup.layerDefaults["MAP"] + 2,"1.0","1.0","CENTRE",{55.7, 48.5}, false, {}, {}),
									mapObject("Tavern1AsObject1", false, true, false, imageLookup.getSequenceAsString("TAVERN1_1","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
									mapObject("Table", false, true, false, imageLookup.getSequenceAsString("TAVERN1_4","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"],"1","1.0","CENTRE",{53.7, 48.9}, false, {}, {}),
									mapObject("Tavern1AsObject2", false, true, false, imageLookup.getSequenceAsString("TAVERN1_2","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
									mapObject("Tavern1AsObject3", false, true, false, imageLookup.getSequenceAsString("TAVERN1_3","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"] + 2,"0.2","1.0","CENTRE",{50, 50}, false, {}, {}),
									mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{52, 56}, false, List<mapFloor::triangle>({mapFloor::triangle({{51.54890418052673,58.0529510974884}, {50.794535875320435,55.240654945373535}, {49.33348894119263,57.070523500442505}}),mapFloor::triangle({{51.54890418052673,58.0529510974884}, {53.00514101982117,56.22187852859497}, {50.794535875320435,55.240654945373535}}),}), Map<string, string>({
																	pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave Tavern"}),
																	pair<string, string>({"copy","AT_Door"}),
																	pair<string, string>({"areaTransition","BénouvilleTown1"}),
																	pair<string, string>({"direction","STAND_FRONT"}),
																	pair<string, string>({"playerPosX","43"}),
																	pair<string, string>({"playerPosY","58"}),
																	pair<string, string>({"audio","NICEDOOR1"}),
																	pair<string, string>({"x","0"}),
																	pair<string, string>({"y","0"}),
																	pair<string, string>({"h","50"}),
																	pair<string, string>({"w","50"}),
																	pair<string, string>("colour", "WHITE"),
																	pair<string, string>("uniqueID", mapPopupTextID),
																	pair<string, string>("anchorStyle", "TOPLEFT"),
																	pair<string, string>("shadowColour", "BLACK"),
																	pair<string, string>("don'tLoadIfNot", "IntroFinished"),
																	pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																	pair<string, string>({"format","LightText_20"}),
																		})),
							}), List<mapFloor>({
					mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{45.80773115158081,55.23442625999451}, {43.52834224700928,52.217042446136475}, {42.25945472717285,53.74298095703125}}),mapFloor::triangle({{51.78239941596985,57.75935649871826}, {51.420509815216064,55.51648139953613}, {50.175940990448,57.07051157951355}}),mapFloor::triangle({{50.175940990448,57.07051157951355}, {47.05921411514282,53.70413064956665}, {45.80773115158081,55.23442625999451}}),
			mapFloor::triangle({{51.420509815216064,55.51648139953613}, {50.562429428100586,49.26672577857971}, {47.05921411514282,53.70413064956665}}),mapFloor::triangle({{54.80759143829346,51.210397481918335}, {55.250173807144165,48.847419023513794}, {53.683775663375854,50.73707699775696}}),mapFloor::triangle({{52.27267146110535,50.05027651786804}, {50.97469687461853,47.59702384471893}, {50.365859270095825,49.536800384521484}}),
			mapFloor::triangle({{50.97469687461853,47.59702384471893}, {50.05497336387634,46.827346086502075}, {49.763306975364685,47.14358448982239}}),mapFloor::triangle({{44.866180419921875,51.85549855232239}, {43.43647360801697,52.36383080482483}, {43.921685218811035,51.41664147377014}}),mapFloor::triangle({{52.332907915115356,56.570613384246826}, {52.022868394851685,53.92199754714966}, {50.277793407440186,55.861955881118774}}),
			mapFloor::triangle({{45.80773115158081,55.23442625999451}, {47.05921411514282,53.70413064956665}, {43.52834224700928,52.217042446136475}}),mapFloor::triangle({{51.78239941596985,57.75935649871826}, {52.54425406455994,56.81350231170654}, {51.420509815216064,55.51648139953613}}),mapFloor::triangle({{50.175940990448,57.07051157951355}, {51.420509815216064,55.51648139953613}, {47.05921411514282,53.70413064956665}}),
			mapFloor::triangle({{51.420509815216064,55.51648139953613}, {54.9092173576355,51.097458600997925}, {50.562429428100586,49.26672577857971}}),mapFloor::triangle({{54.80759143829346,51.210397481918335}, {56.59042000770569,49.41188991069794}, {55.250173807144165,48.847419023513794}}),mapFloor::triangle({{52.27267146110535,50.05027651786804}, {52.75932550430298,47.75756299495697}, {50.97469687461853,47.59702384471893}}),
			mapFloor::triangle({{50.97469687461853,47.59702384471893}, {52.75932550430298,47.75756299495697}, {50.05497336387634,46.827346086502075}}),mapFloor::triangle({{44.866180419921875,51.85549855232239}, {44.82801854610443,53.02895903587341}, {43.43647360801697,52.36383080482483}}),}),true,{}),
					mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{45.80773115158081,55.23442625999451}, {43.52834224700928,52.217042446136475}, {42.25945472717285,53.74298095703125}}),mapFloor::triangle({{51.78239941596985,57.75935649871826}, {51.420509815216064,55.51648139953613}, {50.175940990448,57.07051157951355}}),mapFloor::triangle({{50.175940990448,57.07051157951355}, {47.05921411514282,53.70413064956665}, {45.80773115158081,55.23442625999451}}),
			mapFloor::triangle({{51.420509815216064,55.51648139953613}, {50.562429428100586,49.26672577857971}, {47.05921411514282,53.70413064956665}}),mapFloor::triangle({{54.80759143829346,51.210397481918335}, {55.250173807144165,48.847419023513794}, {53.683775663375854,50.73707699775696}}),mapFloor::triangle({{52.27267146110535,50.05027651786804}, {50.97469687461853,47.59702384471893}, {50.365859270095825,49.536800384521484}}),
			mapFloor::triangle({{50.97469687461853,47.59702384471893}, {50.05497336387634,46.827346086502075}, {49.763306975364685,47.14358448982239}}),mapFloor::triangle({{44.866180419921875,51.85549855232239}, {43.43647360801697,52.36383080482483}, {43.921685218811035,51.41664147377014}}),mapFloor::triangle({{52.660661935806274,56.37573003768921}, {52.3506224155426,53.72711420059204}, {50.6055474281311,55.66707253456116}}),
			mapFloor::triangle({{45.80773115158081,55.23442625999451}, {47.05921411514282,53.70413064956665}, {43.52834224700928,52.217042446136475}}),mapFloor::triangle({{51.78239941596985,57.75935649871826}, {53.00514101982117,56.22187852859497}, {51.420509815216064,55.51648139953613}}),mapFloor::triangle({{50.175940990448,57.07051157951355}, {51.420509815216064,55.51648139953613}, {47.05921411514282,53.70413064956665}}),
			mapFloor::triangle({{51.420509815216064,55.51648139953613}, {54.9092173576355,51.097458600997925}, {50.562429428100586,49.26672577857971}}),mapFloor::triangle({{54.80759143829346,51.210397481918335}, {56.59042000770569,49.41188991069794}, {55.250173807144165,48.847419023513794}}),mapFloor::triangle({{52.27267146110535,50.05027651786804}, {52.75932550430298,47.75756299495697}, {50.97469687461853,47.59702384471893}}),
			mapFloor::triangle({{50.97469687461853,47.59702384471893}, {52.75932550430298,47.75756299495697}, {50.05497336387634,46.827346086502075}}),mapFloor::triangle({{44.866180419921875,51.85549855232239}, {44.82801854610443,53.02895903587341}, {43.43647360801697,52.36383080482483}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
								}), {}, { 5000,5000 }, { Map<string, string>({
								pair<string,string>({"song2",to_string(TOWN_WAV_1) + " " + "MusicVolume"}),
								pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
								pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
								}) });
		maps["IsoHouse"] = mapInstance("IsoHouse", EMPTYMAP, { 48,50 }, List<mapObject>({
						mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
						mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{46, 46}, false, List<mapFloor::triangle>({mapFloor::triangle({{46.41692042350769,45.48998177051544}, {48.017191886901855,47.58803844451904}, {46.41692042350769,47.58803844451904}}),mapFloor::triangle({{46.41692042350769,45.48998177051544}, {48.017191886901855,45.48998177051544}, {48.017191886901855,47.58803844451904}}),}), Map<string, string>({
														pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave House"}),
														pair<string, string>({"copy","AT_Door"}),
														pair<string, string>({"areaTransition","BénouvilleTown1"}),
														pair<string, string>({"direction","STAND_LEFT"}),
														pair<string, string>({"playerPosX","55"}),
														pair<string, string>({"playerPosY","51"}),
														pair<string, string>({"audio","NICEDOOR1"}),
														pair<string, string>({"x","0"}),
														pair<string, string>({"y","0"}),
														pair<string, string>({"h","50"}),
														pair<string, string>({"w","50"}),
														pair<string, string>("colour", "WHITE"),
														pair<string, string>("uniqueID", mapPopupTextID),
														pair<string, string>("anchorStyle", "TOPLEFT"),
														pair<string, string>("shadowColour", "BLACK"),
														pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
														pair<string, string>({"format","LightText_20"}),
															})),
						mapObject("IsoHouseAsObject", false, true, false, imageLookup.getSequenceAsString("ISOHOUSE1","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
						mapObject("IsoHouseAsObject2", false, true, false, imageLookup.getSequenceAsString("ISOHOUSE2","STAND_FRONT"),"0",0,imageLookup.layerDefaults["ENVIRONMENT"] + 2,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
						mapObject("IsobellaMerchant", true, true, false, imageLookup.getSequenceAsString("EMPTYOBJECT","STAND_FRONT"), "0", 0, 0, "0", "0","", {47, 55}, false, List<mapFloor::triangle>({mapFloor::triangle({{47.89861440658569,54.704225063323975}, {49.07795190811157,56.25041127204895}, {47.89861440658569,56.25041127204895}}),mapFloor::triangle({{47.89861440658569,54.704225063323975}, {49.07795190811157,54.704225063323975}, {49.07795190811157,56.25041127204895}}),}), Map<string, string>({
													pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_View Isobella"}),
													pair<string, string>({"copy","IsobellaMerchant"}),
													pair<string, string>({"x","-15"}),
													pair<string, string>({"y","0"}),
													pair<string, string>({"h","50"}),
													pair<string, string>({"w","50"}),
													pair<string, string>("Merchant", "1"),
													pair<string, string>("colour", "WHITE"),
													pair<string, string>("uniqueID", mapPopupTextID),
													pair<string, string>("anchorStyle", "TOPLEFT"),
													pair<string, string>("shadowColour", "BLACK"),
													pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
													pair<string, string>({"format","LightText_20"}) })),
						mapObject("Isobella", true, true, false, imageLookup.getSequenceAsString("Isobella de Vaines","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","0.9","CENTRE",{49.8, 53}, false, List<mapFloor::triangle>({mapFloor::triangle({{49.09331798553467,54.02823090553284}, {50.56154727935791,56.25041127204895}, {49.09331798553467,56.25041127204895}}),mapFloor::triangle({{49.09331798553467,54.02823090553284}, {50.56154727935791,54.02823090553284}, {50.56154727935791,56.25041127204895}}),}), Map<string, string>({
													pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Isobella"}),
													pair<string, string>({"copy","Isobella"}),
													pair<string, string>({"cutscene","Isobella"}),
													pair<string, string>({"x","2"}),
													pair<string, string>({"y","-7"}),
													pair<string, string>({"h","50"}),
													pair<string, string>({"w","50"}),
													pair<string, string>("colour", "WHITE"),
													pair<string, string>("uniqueID", mapPopupTextID),
													pair<string, string>("anchorStyle", "TOPLEFT"),
													pair<string, string>("shadowColour", "BLACK"),
													pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
													pair<string, string>({"format","LightText_20"})})),
						mapObject("ShadowIsobella", false, true, false, imageLookup.getSequenceAsString("Shadow Isobella de Vaines","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","0.9","CENTRE",{49.8, 53}, false, {},{}),
						}), List<mapFloor>({
								mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{51.14791393280029,45.70656716823578}, {46.577221155166626,47.115594148635864}, {51.14791393280029,47.115594148635864}}),mapFloor::triangle({{52.39313840866089,45.70656716823578}, {51.14791393280029,47.115594148635864}, {52.39313840866089,47.115594148635864}}),mapFloor::triangle({{51.14791393280029,47.115594148635864}, {52.39313840866089,56.25041127204895}, {52.39313840866089,47.115594148635864}}),
			mapFloor::triangle({{49.801453948020935,56.25041127204895}, {51.81728005409241,55.49086332321167}, {51.81728005409241,56.25041127204895}}),mapFloor::triangle({{49.801453948020935,55.49086332321167}, {48.009854555130005,55.658286809921265}, {48.009854555130005,54.9493670463562}}),mapFloor::triangle({{48.009854555130005,54.9493670463562}, {49.801453948020935,53.939396142959595}, {49.801453948020935,55.49086332321167}}),
			mapFloor::triangle({{49.801453948020935,53.939396142959595}, {50.302654504776,55.49086332321167}, {49.801453948020935,55.49086332321167}}),mapFloor::triangle({{49.73040521144867,51.68055295944214}, {47.50184714794159,53.10479998588562}, {49.71359670162201,54.99657392501831}}),mapFloor::triangle({{48.8520622253418,50.02935528755188}, {47.50184714794159,51.68055295944214}, {49.73040521144867,51.68055295944214}}),
			mapFloor::triangle({{49.73040521144867,51.68055295944214}, {49.86833930015564,50.02935528755188}, {48.8520622253418,50.02935528755188}}),mapFloor::triangle({{49.86833930015564,50.02935528755188}, {48.8520622253418,49.64827597141266}, {48.8520622253418,50.02935528755188}}),mapFloor::triangle({{51.14791393280029,45.70656716823578}, {46.577221155166626,45.70656716823578}, {46.577221155166626,47.115594148635864}}),
			mapFloor::triangle({{52.39313840866089,45.70656716823578}, {51.14791393280029,45.70656716823578}, {51.14791393280029,47.115594148635864}}),mapFloor::triangle({{51.14791393280029,47.115594148635864}, {51.14791393280029,56.25041127204895}, {52.39313840866089,56.25041127204895}}),mapFloor::triangle({{49.801453948020935,56.25041127204895}, {49.801453948020935,55.49086332321167}, {51.81728005409241,55.49086332321167}}),
			mapFloor::triangle({{49.801453948020935,55.49086332321167}, {49.801453948020935,56.25041127204895}, {48.009854555130005,55.658286809921265}}),mapFloor::triangle({{48.009854555130005,54.9493670463562}, {48.009854555130005,53.40504050254822}, {49.801453948020935,53.939396142959595}}),mapFloor::triangle({{49.801453948020935,53.939396142959595}, {50.302654504776,53.939396142959595}, {50.302654504776,55.49086332321167}}),
			mapFloor::triangle({{49.73040521144867,51.68055295944214}, {47.50184714794159,51.68055295944214}, {47.50184714794159,53.10479998588562}}),mapFloor::triangle({{48.8520622253418,50.02935528755188}, {47.50184714794159,50.02935528755188}, {47.50184714794159,51.68055295944214}}),mapFloor::triangle({{49.73040521144867,51.68055295944214}, {49.86833930015564,51.68055295944214}, {49.86833930015564,50.02935528755188}}),
			mapFloor::triangle({{49.86833930015564,50.02935528755188}, {49.86833930015564,49.64827597141266}, {48.8520622253418,49.64827597141266}}),}),true,{}),
								mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{51.14791393280029,45.70656716823578}, {46.577221155166626,47.115594148635864}, {51.14791393280029,47.115594148635864}}),mapFloor::triangle({{52.39313840866089,45.70656716823578}, {51.14791393280029,47.115594148635864}, {52.39313840866089,47.115594148635864}}),mapFloor::triangle({{51.14791393280029,47.115594148635864}, {52.39313840866089,56.25041127204895}, {52.39313840866089,47.115594148635864}}),
			mapFloor::triangle({{49.801453948020935,56.25041127204895}, {51.81728005409241,55.49086332321167}, {51.81728005409241,56.25041127204895}}),mapFloor::triangle({{49.801453948020935,55.49086332321167}, {48.009854555130005,55.658286809921265}, {48.009854555130005,54.9493670463562}}),mapFloor::triangle({{48.009854555130005,54.9493670463562}, {49.801453948020935,53.939396142959595}, {49.801453948020935,55.49086332321167}}),
			mapFloor::triangle({{49.801453948020935,53.939396142959595}, {50.302654504776,55.49086332321167}, {49.801453948020935,55.49086332321167}}),mapFloor::triangle({{49.73040521144867,51.68055295944214}, {47.50184714794159,53.10479998588562}, {49.71359670162201,54.99657392501831}}),mapFloor::triangle({{48.8520622253418,50.02935528755188}, {47.50184714794159,51.68055295944214}, {49.73040521144867,51.68055295944214}}),
			mapFloor::triangle({{49.73040521144867,51.68055295944214}, {49.86833930015564,50.02935528755188}, {48.8520622253418,50.02935528755188}}),mapFloor::triangle({{49.86833930015564,50.02935528755188}, {48.8520622253418,49.64827597141266}, {48.8520622253418,50.02935528755188}}),mapFloor::triangle({{51.14791393280029,45.70656716823578}, {46.577221155166626,45.70656716823578}, {46.577221155166626,47.115594148635864}}),
			mapFloor::triangle({{52.39313840866089,45.70656716823578}, {51.14791393280029,45.70656716823578}, {51.14791393280029,47.115594148635864}}),mapFloor::triangle({{51.14791393280029,47.115594148635864}, {51.14791393280029,56.25041127204895}, {52.39313840866089,56.25041127204895}}),mapFloor::triangle({{49.801453948020935,56.25041127204895}, {49.801453948020935,55.49086332321167}, {51.81728005409241,55.49086332321167}}),
			mapFloor::triangle({{49.801453948020935,55.49086332321167}, {49.801453948020935,56.25041127204895}, {48.009854555130005,55.658286809921265}}),mapFloor::triangle({{48.009854555130005,54.9493670463562}, {48.009854555130005,53.40504050254822}, {49.801453948020935,53.939396142959595}}),mapFloor::triangle({{49.801453948020935,53.939396142959595}, {50.302654504776,53.939396142959595}, {50.302654504776,55.49086332321167}}),
			mapFloor::triangle({{49.73040521144867,51.68055295944214}, {47.50184714794159,51.68055295944214}, {47.50184714794159,53.10479998588562}}),mapFloor::triangle({{48.8520622253418,50.02935528755188}, {47.50184714794159,50.02935528755188}, {47.50184714794159,51.68055295944214}}),mapFloor::triangle({{49.73040521144867,51.68055295944214}, {49.86833930015564,51.68055295944214}, {49.86833930015564,50.02935528755188}}),
			mapFloor::triangle({{49.86833930015564,50.02935528755188}, {49.86833930015564,49.64827597141266}, {48.8520622253418,49.64827597141266}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
							}), {}, { 5000,5000 }, { Map<string, string>({
								pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
								pair<string,string>({"song2",to_string(CLOCK1_WAV) + " " + "AmbienceVolume"}),
								pair<string,string>({"song3",to_string(TOWN_WAV_1) + " " + "MusicVolume"}),
								pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
								}) });
		maps["SmithyInside"] = mapInstance("SmithyInside", EMPTYMAP, { 50,50 }, List<mapObject>({
									mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
									mapObject("BSMITHM", true, true, false, imageLookup.getSequenceAsString("BSMITHM", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 44, 47 }, false, List<mapFloor::triangle>({mapFloor::triangle({{42.71445572376251,49.696892499923706}, {45.711466670036316,47.12308645248413}, {46.2537944316864,49.27886724472046}}),mapFloor::triangle({{42.71445572376251,49.696892499923706}, {42.17788875102997,47.616103291511536}, {45.711466670036316,47.12308645248413}}),}), Map<string, string>({
																pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Blacksmith"}),
																pair<string, string>({"copy","BSMITHM"}),
																pair<string, string>({"x","5"}),
																pair<string, string>({"y","-5"}),
																pair<string, string>({"h","50"}),
																pair<string, string>({"w","50"}),
																pair<string, string>("Merchant", "1"),
																pair<string, string>("colour", "WHITE"),
																pair<string, string>("uniqueID", mapPopupTextID),
																pair<string, string>("anchorStyle", "TOPLEFT"),
																pair<string, string>("shadowColour", "BLACK"),
																pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																pair<string, string>({"format","LightText_20"}) })),
									mapObject("Shadow BSMITHM", false, true, false, imageLookup.getSequenceAsString("Shadow BSMITHM","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{44, 47}, false, {},{}),
									mapObject("BSMITHF", true, true, false, imageLookup.getSequenceAsString("BSMITHF", "STAND_FRONT"), "1", 500, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0","CENTRE", { 50, 42 }, false, List<mapFloor::triangle>({mapFloor::triangle({{48.24576377868652,44.805559515953064}, {51.242780685424805,42.23175346851349}, {51.78510546684265,44.38753426074982}}),mapFloor::triangle({{48.24576377868652,44.805559515953064}, {47.70919680595398,42.724770307540894}, {51.242780685424805,42.23175346851349}}),}), Map<string, string>({
																pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to Blacksmith"}),
																pair<string, string>({"copy","BSMITHF"}),
																pair<string, string>({"x","5"}),
																pair<string, string>({"y","-5"}),
																pair<string, string>({"h","50"}),
																pair<string, string>({"w","50"}),
																pair<string, string>("Merchant", "1"),
																pair<string, string>("colour", "WHITE"),
																pair<string, string>("uniqueID", mapPopupTextID),
																pair<string, string>("anchorStyle", "TOPLEFT"),
																pair<string, string>("shadowColour", "BLACK"),
																pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																pair<string, string>({"format","LightText_20"}) })),
									mapObject("Shadow BSMITHF", false, true, false, imageLookup.getSequenceAsString("Shadow BSMITHF","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{50, 42}, false, {},{}),
									mapObject("SmithyInsideAsObject", false, true, false, imageLookup.getSequenceAsString("SMITHYINSIDE","STAND_FRONT"),"0",0,imageLookup.layerDefaults["Map"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
									mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{50, 60}, false, List<mapFloor::triangle>({mapFloor::triangle({{48.45373034477234,60.88482737541199}, {51.3355553150177,58.81905555725098}, {51.9015371799469,60.41581630706787}}),mapFloor::triangle({{48.45373034477234,60.88482737541199}, {47.8946179151535,59.36296582221985}, {51.3355553150177,58.81905555725098}}),}), Map<string, string>({
																	pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave Smithy"}),
																	pair<string, string>({"copy","AT_Door"}),
																	pair<string, string>({"areaTransition","BénouvilleTown1"}),
																	pair<string, string>({"direction","STAND_FRONT"}),
																	pair<string, string>({"playerPosX","22"}),
																	pair<string, string>({"playerPosY","63"}),
																	pair<string, string>({"audio","SHOPDOOR2"}),
																	pair<string, string>({"x","0"}),
																	pair<string, string>({"y","0"}),
																	pair<string, string>({"h","50"}),
																	pair<string, string>({"w","50"}),
																	pair<string, string>("colour", "WHITE"),
																	pair<string, string>("uniqueID", mapPopupTextID),
																	pair<string, string>("anchorStyle", "TOPLEFT"),
																	pair<string, string>("shadowColour", "BLACK"),
																	pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																	pair<string, string>({"format","LightText_20"}),
																		})),
									}), List<mapFloor>({
									mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{44.26324963569641,61.39955520629883}, {46.67193591594696,57.91361927986145}, {47.514209151268005,60.93399524688721}}),mapFloor::triangle({{47.514209151268005,60.93399524688721}, {55.145782232284546,56.70009255409241}, {55.98805546760559,59.720468521118164}}),mapFloor::triangle({{55.145782232284546,56.70009255409241}, {47.12591469287872,57.16918706893921}, {55.145782232284546,56.020671129226685}}),
				mapFloor::triangle({{55.145782232284546,56.020671129226685}, {47.69083857536316,56.497544050216675}, {55.01830577850342,55.44818639755249}}),mapFloor::triangle({{55.145782232284546,56.020671129226685}, {55.57105541229248,55.36314845085144}, {55.69853186607361,55.935633182525635}}),mapFloor::triangle({{55.01830577850342,55.44818639755249}, {55.22027015686035,53.5888671875}, {55.57105541229248,55.36314845085144}}),
				mapFloor::triangle({{47.69083857536316,56.497544050216675}, {54.462140798568726,53.70331406593323}, {55.01830577850342,55.44818639755249}}),mapFloor::triangle({{46.454012393951416,45.06559669971466}, {51.37625336647034,42.13124215602875}, {51.948899030685425,44.27867531776428}}),mapFloor::triangle({{47.40384221076965,52.7351975440979}, {52.41432785987854,46.02402448654175}, {54.01214361190796,52.01582908630371}}),
				mapFloor::triangle({{46.7032790184021,46.841901540756226}, {44.278424978256226,52.997398376464844}, {42.04740822315216,47.53071665763855}}),mapFloor::triangle({{47.60114252567291,54.39493656158447}, {54.01214361190796,52.01582908630371}, {54.462140798568726,53.70331406593323}}),mapFloor::triangle({{41.90409481525421,53.16284894943237}, {42.273736000061035,50.68162679672241}, {42.7021324634552,53.11139225959778}}),
				mapFloor::triangle({{42.7021324634552,53.11139225959778}, {44.46992874145508,50.567638874053955}, {44.46992874145508,52.997398376464844}}),mapFloor::triangle({{43.448323011398315,49.14799928665161}, {42.273736000061035,50.68162679672241}, {42.02996790409088,49.14799928665161}}),mapFloor::triangle({{46.7032790184021,46.841901540756226}, {51.948899030685425,44.27867531776428}, {52.41432785987854,46.02402448654175}}),
				mapFloor::triangle({{47.40384221076965,52.7351975440979}, {44.756484031677246,54.16879057884216}, {44.278424978256226,52.997398376464844}}),mapFloor::triangle({{47.70389795303345,40.665894746780396}, {49.43193793296814,38.7274295091629}, {49.83611702919006,40.28853476047516}}),mapFloor::triangle({{47.8838324546814,40.665894746780396}, {46.0231751203537,43.46131682395935}, {45.449092984199524,41.04325473308563}}),
				mapFloor::triangle({{45.449092984199524,41.04325473308563}, {47.42230772972107,39.10478949546814}, {47.8838324546814,40.665894746780396}}),mapFloor::triangle({{43.050575256347656,43.28620433807373}, {39.78189826011658,45.269814133644104}, {39.42515254020691,43.867915868759155}}),mapFloor::triangle({{39.42515254020691,43.867915868759155}, {42.474937438964844,41.44938588142395}, {43.050575256347656,43.28620433807373}}),
				mapFloor::triangle({{43.050575256347656,43.28620433807373}, {43.41710805892944,41.32582247257233}, {43.992745876312256,43.16264092922211}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {43.34287345409393,44.67802047729492}, {43.050575256347656,43.28620433807373}}),mapFloor::triangle({{38.897791504859924,42.0167475938797}, {42.083701491355896,40.268293023109436}, {42.474937438964844,41.44938588142395}}),
				mapFloor::triangle({{46.14731967449188,42.88007318973541}, {44.255539774894714,44.5544570684433}, {43.992745876312256,43.16264092922211}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {45.57168185710907,41.04325473308563}, {46.14731967449188,42.88007318973541}}),mapFloor::triangle({{43.41710805892944,41.32582247257233}, {45.167502760887146,39.48214948177338}, {45.57168185710907,41.04325473308563}}),
				mapFloor::triangle({{44.26324963569641,61.39955520629883}, {43.420976400375366,58.37917923927307}, {46.67193591594696,57.91361927986145}}),mapFloor::triangle({{47.514209151268005,60.93399524688721}, {46.67193591594696,57.91361927986145}, {55.145782232284546,56.70009255409241}}),mapFloor::triangle({{55.145782232284546,56.70009255409241}, {46.67193591594696,57.91361927986145}, {47.12591469287872,57.16918706893921}}),
				mapFloor::triangle({{55.145782232284546,56.020671129226685}, {47.12591469287872,57.16918706893921}, {47.69083857536316,56.497544050216675}}),mapFloor::triangle({{55.145782232284546,56.020671129226685}, {55.01830577850342,55.44818639755249}, {55.57105541229248,55.36314845085144}}),mapFloor::triangle({{55.01830577850342,55.44818639755249}, {54.462140798568726,53.70331406593323}, {55.22027015686035,53.5888671875}}),
				mapFloor::triangle({{47.69083857536316,56.497544050216675}, {47.60114252567291,54.39493656158447}, {54.462140798568726,53.70331406593323}}),mapFloor::triangle({{46.454012393951416,45.06559669971466}, {46.14731967449188,42.88007318973541}, {51.37625336647034,42.13124215602875}}),mapFloor::triangle({{47.40384221076965,52.7351975440979}, {46.7032790184021,46.841901540756226}, {52.41432785987854,46.02402448654175}}),
				mapFloor::triangle({{46.7032790184021,46.841901540756226}, {47.40384221076965,52.7351975440979}, {44.278424978256226,52.997398376464844}}),mapFloor::triangle({{47.60114252567291,54.39493656158447}, {47.40384221076965,52.7351975440979}, {54.01214361190796,52.01582908630371}}),mapFloor::triangle({{41.90409481525421,53.16284894943237}, {41.28230810165405,50.733089447021484}, {42.273736000061035,50.68162679672241}}),
				mapFloor::triangle({{42.7021324634552,53.11139225959778}, {42.273736000061035,50.68162679672241}, {44.46992874145508,50.567638874053955}}),mapFloor::triangle({{43.448323011398315,49.14799928665161}, {44.46992874145508,50.567638874053955}, {42.273736000061035,50.68162679672241}}),mapFloor::triangle({{46.7032790184021,46.841901540756226}, {46.454012393951416,45.06559669971466}, {51.948899030685425,44.27867531776428}}),
				mapFloor::triangle({{47.40384221076965,52.7351975440979}, {47.545406222343445,53.92605662345886}, {44.756484031677246,54.16879057884216}}),mapFloor::triangle({{47.70389795303345,40.665894746780396}, {47.29971885681152,39.10478949546814}, {49.43193793296814,38.7274295091629}}),mapFloor::triangle({{47.8838324546814,40.665894746780396}, {48.79792928695679,43.03125739097595}, {46.0231751203537,43.46131682395935}}),
				mapFloor::triangle({{45.449092984199524,41.04325473308563}, {44.9875682592392,39.48214948177338}, {47.42230772972107,39.10478949546814}}),mapFloor::triangle({{43.050575256347656,43.28620433807373}, {43.34287345409393,44.67802047729492}, {39.78189826011658,45.269814133644104}}),mapFloor::triangle({{39.42515254020691,43.867915868759155}, {38.897791504859924,42.0167475938797}, {42.474937438964844,41.44938588142395}}),
				mapFloor::triangle({{43.050575256347656,43.28620433807373}, {42.474937438964844,41.44938588142395}, {43.41710805892944,41.32582247257233}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {44.255539774894714,44.5544570684433}, {43.34287345409393,44.67802047729492}}),mapFloor::triangle({{38.897791504859924,42.0167475938797}, {38.53760361671448,40.825873613357544}, {42.083701491355896,40.268293023109436}}),
				mapFloor::triangle({{46.14731967449188,42.88007318973541}, {46.34263217449188,44.271889328956604}, {44.255539774894714,44.5544570684433}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {43.41710805892944,41.32582247257233}, {45.57168185710907,41.04325473308563}}),mapFloor::triangle({{43.41710805892944,41.32582247257233}, {43.01292896270752,39.76471722126007}, {45.167502760887146,39.48214948177338}}),}),true,{}),
									mapFloor("WoodFloor", List<mapFloor::triangle>({mapFloor::triangle({{44.26324963569641,61.39955520629883}, {46.67193591594696,57.91361927986145}, {47.514209151268005,60.93399524688721}}),mapFloor::triangle({{47.514209151268005,60.93399524688721}, {55.145782232284546,56.70009255409241}, {55.98805546760559,59.720468521118164}}),mapFloor::triangle({{55.145782232284546,56.70009255409241}, {47.12591469287872,57.16918706893921}, {55.145782232284546,56.020671129226685}}),
				mapFloor::triangle({{55.145782232284546,56.020671129226685}, {47.69083857536316,56.497544050216675}, {55.01830577850342,55.44818639755249}}),mapFloor::triangle({{55.145782232284546,56.020671129226685}, {55.57105541229248,55.36314845085144}, {55.69853186607361,55.935633182525635}}),mapFloor::triangle({{55.01830577850342,55.44818639755249}, {55.22027015686035,53.5888671875}, {55.57105541229248,55.36314845085144}}),
				mapFloor::triangle({{47.69083857536316,56.497544050216675}, {54.462140798568726,53.70331406593323}, {55.01830577850342,55.44818639755249}}),mapFloor::triangle({{46.454012393951416,45.06559669971466}, {51.37625336647034,42.13124215602875}, {51.948899030685425,44.27867531776428}}),mapFloor::triangle({{47.40384221076965,52.7351975440979}, {52.41432785987854,46.02402448654175}, {54.01214361190796,52.01582908630371}}),
				mapFloor::triangle({{46.7032790184021,46.841901540756226}, {44.278424978256226,52.997398376464844}, {42.04740822315216,47.53071665763855}}),mapFloor::triangle({{47.60114252567291,54.39493656158447}, {54.01214361190796,52.01582908630371}, {54.462140798568726,53.70331406593323}}),mapFloor::triangle({{41.90409481525421,53.16284894943237}, {42.273736000061035,50.68162679672241}, {42.7021324634552,53.11139225959778}}),
				mapFloor::triangle({{42.7021324634552,53.11139225959778}, {44.46992874145508,50.567638874053955}, {44.46992874145508,52.997398376464844}}),mapFloor::triangle({{43.448323011398315,49.14799928665161}, {42.273736000061035,50.68162679672241}, {42.02996790409088,49.14799928665161}}),mapFloor::triangle({{46.7032790184021,46.841901540756226}, {51.948899030685425,44.27867531776428}, {52.41432785987854,46.02402448654175}}),
				mapFloor::triangle({{47.40384221076965,52.7351975440979}, {44.756484031677246,54.16879057884216}, {44.278424978256226,52.997398376464844}}),mapFloor::triangle({{47.70389795303345,40.665894746780396}, {49.43193793296814,38.7274295091629}, {49.83611702919006,40.28853476047516}}),mapFloor::triangle({{47.8838324546814,40.665894746780396}, {46.0231751203537,43.46131682395935}, {45.449092984199524,41.04325473308563}}),
				mapFloor::triangle({{45.449092984199524,41.04325473308563}, {47.42230772972107,39.10478949546814}, {47.8838324546814,40.665894746780396}}),mapFloor::triangle({{43.050575256347656,43.28620433807373}, {39.78189826011658,45.269814133644104}, {39.42515254020691,43.867915868759155}}),mapFloor::triangle({{39.42515254020691,43.867915868759155}, {42.474937438964844,41.44938588142395}, {43.050575256347656,43.28620433807373}}),
				mapFloor::triangle({{43.050575256347656,43.28620433807373}, {43.41710805892944,41.32582247257233}, {43.992745876312256,43.16264092922211}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {43.34287345409393,44.67802047729492}, {43.050575256347656,43.28620433807373}}),mapFloor::triangle({{38.897791504859924,42.0167475938797}, {42.083701491355896,40.268293023109436}, {42.474937438964844,41.44938588142395}}),
				mapFloor::triangle({{46.14731967449188,42.88007318973541}, {44.255539774894714,44.5544570684433}, {43.992745876312256,43.16264092922211}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {45.57168185710907,41.04325473308563}, {46.14731967449188,42.88007318973541}}),mapFloor::triangle({{43.41710805892944,41.32582247257233}, {45.167502760887146,39.48214948177338}, {45.57168185710907,41.04325473308563}}),
				mapFloor::triangle({{44.26324963569641,61.39955520629883}, {43.420976400375366,58.37917923927307}, {46.67193591594696,57.91361927986145}}),mapFloor::triangle({{47.514209151268005,60.93399524688721}, {46.67193591594696,57.91361927986145}, {55.145782232284546,56.70009255409241}}),mapFloor::triangle({{55.145782232284546,56.70009255409241}, {46.67193591594696,57.91361927986145}, {47.12591469287872,57.16918706893921}}),
				mapFloor::triangle({{55.145782232284546,56.020671129226685}, {47.12591469287872,57.16918706893921}, {47.69083857536316,56.497544050216675}}),mapFloor::triangle({{55.145782232284546,56.020671129226685}, {55.01830577850342,55.44818639755249}, {55.57105541229248,55.36314845085144}}),mapFloor::triangle({{55.01830577850342,55.44818639755249}, {54.462140798568726,53.70331406593323}, {55.22027015686035,53.5888671875}}),
				mapFloor::triangle({{47.69083857536316,56.497544050216675}, {47.60114252567291,54.39493656158447}, {54.462140798568726,53.70331406593323}}),mapFloor::triangle({{46.454012393951416,45.06559669971466}, {46.14731967449188,42.88007318973541}, {51.37625336647034,42.13124215602875}}),mapFloor::triangle({{47.40384221076965,52.7351975440979}, {46.7032790184021,46.841901540756226}, {52.41432785987854,46.02402448654175}}),
				mapFloor::triangle({{46.7032790184021,46.841901540756226}, {47.40384221076965,52.7351975440979}, {44.278424978256226,52.997398376464844}}),mapFloor::triangle({{47.60114252567291,54.39493656158447}, {47.40384221076965,52.7351975440979}, {54.01214361190796,52.01582908630371}}),mapFloor::triangle({{41.90409481525421,53.16284894943237}, {41.28230810165405,50.733089447021484}, {42.273736000061035,50.68162679672241}}),
				mapFloor::triangle({{42.7021324634552,53.11139225959778}, {42.273736000061035,50.68162679672241}, {44.46992874145508,50.567638874053955}}),mapFloor::triangle({{43.448323011398315,49.14799928665161}, {44.46992874145508,50.567638874053955}, {42.273736000061035,50.68162679672241}}),mapFloor::triangle({{46.7032790184021,46.841901540756226}, {46.454012393951416,45.06559669971466}, {51.948899030685425,44.27867531776428}}),
				mapFloor::triangle({{47.40384221076965,52.7351975440979}, {47.545406222343445,53.92605662345886}, {44.756484031677246,54.16879057884216}}),mapFloor::triangle({{47.70389795303345,40.665894746780396}, {47.29971885681152,39.10478949546814}, {49.43193793296814,38.7274295091629}}),mapFloor::triangle({{47.8838324546814,40.665894746780396}, {48.79792928695679,43.03125739097595}, {46.0231751203537,43.46131682395935}}),
				mapFloor::triangle({{45.449092984199524,41.04325473308563}, {44.9875682592392,39.48214948177338}, {47.42230772972107,39.10478949546814}}),mapFloor::triangle({{43.050575256347656,43.28620433807373}, {43.34287345409393,44.67802047729492}, {39.78189826011658,45.269814133644104}}),mapFloor::triangle({{39.42515254020691,43.867915868759155}, {38.897791504859924,42.0167475938797}, {42.474937438964844,41.44938588142395}}),
				mapFloor::triangle({{43.050575256347656,43.28620433807373}, {42.474937438964844,41.44938588142395}, {43.41710805892944,41.32582247257233}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {44.255539774894714,44.5544570684433}, {43.34287345409393,44.67802047729492}}),mapFloor::triangle({{38.897791504859924,42.0167475938797}, {38.53760361671448,40.825873613357544}, {42.083701491355896,40.268293023109436}}),
				mapFloor::triangle({{46.14731967449188,42.88007318973541}, {46.34263217449188,44.271889328956604}, {44.255539774894714,44.5544570684433}}),mapFloor::triangle({{43.992745876312256,43.16264092922211}, {43.41710805892944,41.32582247257233}, {45.57168185710907,41.04325473308563}}),mapFloor::triangle({{43.41710805892944,41.32582247257233}, {43.01292896270752,39.76471722126007}, {45.167502760887146,39.48214948177338}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
										}), {}, { 5000,5000 }, { Map<string, string>({
											pair<string,string>({"song2",to_string(TOWN_WAV_1) + " " + "MusicVolume"}),
											pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
											pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
											}) });
		maps["EstateOutside1"] = mapInstance("EstateOutside1", MAP_ESTATEOUTSIDE1, { 24,95 }, List<mapObject>({
										mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
										mapObject("EstateOutside1TopLayer", false, true, false, imageLookup.getSequenceAsString("EstateOutside1TopLayer","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
										mapObject("AT_EnterTown", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 24, 97 }, false, List<mapFloor::triangle>({mapFloor::triangle({{18.6276376247406,100.04528760910034}, {29.169324040412903,95.83765864372253}, {29.169324040412903,100.04528760910034}}),mapFloor::triangle({{18.6276376247406,100.04528760910034}, {18.24190318584442,95.83765864372253}, {29.169324040412903,95.83765864372253}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Bénouville"}),
																		pair<string, string>({"copy","AT_EnterTown"}),
																		pair<string, string>({"areaTransition","BénouvilleTown1"}),
																		pair<string, string>({"direction","STAND_FRONT"}),
																		pair<string, string>({"playerPosX","27"}),
																		pair<string, string>({"playerPosY","6"}),
																		pair<string, string>({"audio","STONESTEPS"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"}),
										})),
										mapObject("AT_EnterChapel", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 50, 53 }, false, List<mapFloor::triangle>({mapFloor::triangle({{46.45344018936157,48.13438355922699}, {53.231436014175415,55.1128625869751}, {46.286171674728394,54.11650538444519}}),mapFloor::triangle({{46.45344018936157,48.13438355922699}, {54.817503690719604,49.334290623664856}, {53.231436014175415,55.1128625869751}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Chapel"}),
																		pair<string, string>({"copy","AT_EnterChapel"}),
																		pair<string, string>({"areaTransition","ChapelEntryHall"}),
																		pair<string, string>({"direction","STAND_BACK"}),
																		pair<string, string>({"playerPosX","50"}),
																		pair<string, string>({"playerPosY","80"}),
																		pair<string, string>({"audio","CHAPELMULTISTEP"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"}),
										})),
										mapObject("AT_EnterBasement", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 86, 66 }, false, List<mapFloor::triangle>({mapFloor::triangle({{84.13069248199463,69.25116777420044}, {86.61420345306396,67.54971742630005}, {86.8587076663971,68.63859295845032}}),mapFloor::triangle({{84.13069248199463,69.25116777420044}, {83.78636240959167,68.18471550941467}, {86.61420345306396,67.54971742630005}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Basement"}),
																		pair<string, string>({"copy","AT_EnterBasement"}),
																		pair<string, string>({"areaTransition","ChapelRight2"}),
																		pair<string, string>({"direction","STAND_BACK"}),
																		pair<string, string>({"playerPosX","85"}),
																		pair<string, string>({"playerPosY","95"}),
																		pair<string, string>({"audio","CHAPELMULTISTEP"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"}),
										})),
										mapObject::getTreasureChest("BalconyChest1", {39.5,82},"ELITE1_FRONT","Tome of Light of Day"),
										mapObject::getTreasureChest("BalconyChest2", {97,68},"ELITE1_FRONT","Tome of Castigate Cruor"),
										}), List<mapFloor>({
											mapFloor("Walkable", List<mapFloor::triangle>({mapFloor::triangle({{18.425588309764862,96.11464142799377}, {29.18049693107605,100.02213716506958}, {29.424387216567993,96.01919651031494}}),mapFloor::triangle({{11.677411198616028,92.17037558555603}, {0.02800583024509251,96.27995491027832}, {11.831343173980713,96.27995491027832}}),mapFloor::triangle({{17.176538705825806,96.27995491027832}, {13.679563999176025,93.4194266796112}, {13.83349597454071,96.27995491027832}}),
mapFloor::triangle({{13.679563999176025,93.4194266796112}, {11.831343173980713,96.27995491027832}, {13.83349597454071,96.27995491027832}}),mapFloor::triangle({{18.425588309764862,96.11464142799377}, {17.02260673046112,92.88674592971802}, {17.176538705825806,96.27995491027832}}),mapFloor::triangle({{18.271654844284058,92.00506210327148}, {29.424387216567993,96.01919651031494}, {29.753458499908447,90.6181812286377}}),
mapFloor::triangle({{11.677411198616028,92.17037558555603}, {10.31193733215332,67.16897487640381}, {0.028006016509607434,92.17037558555603}}),mapFloor::triangle({{25.685718655586243,61.68085336685181}, {11.180254817008972,56.19272589683533}, {11.203084141016006,61.68085336685181}}),mapFloor::triangle({{21.96134328842163,67.16897487640381}, {11.203084141016006,61.68085336685181}, {10.31193733215332,67.16897487640381}}),
mapFloor::triangle({{4.150979220867157,38.1668359041214}, {15.012656152248383,32.15820491313934}, {0.16508987173438072,32.15820491313934}}),mapFloor::triangle({{11.180254817008972,56.19272589683533}, {24.506129324436188,50.18409490585327}, {9.658561646938324,50.18409490585327}}),mapFloor::triangle({{9.658561646938324,50.18409490585327}, {21.752338111400604,44.175463914871216}, {6.9047704339027405,44.175466895103455}}),
mapFloor::triangle({{6.9047704339027405,44.175466895103455}, {18.99854689836502,38.16683292388916}, {4.150979220867157,38.1668359041214}}),mapFloor::triangle({{0.16508987173438072,32.15820491313934}, {6.653813272714615,18.008416891098022}, {0.1650904887355864,18.008416891098022}}),mapFloor::triangle({{0.1650904887355864,18.008416891098022}, {3.557944670319557,12.898267805576324}, {2.8165917843580246,12.898267805576324}}),
mapFloor::triangle({{29.424387216567993,96.01919651031494}, {31.530937552452087,91.33017659187317}, {29.753458499908447,90.6181812286377}}),mapFloor::triangle({{31.530937552452087,96.01919651031494}, {34.66924428939819,91.33017659187317}, {31.530937552452087,91.33017659187317}}),mapFloor::triangle({{34.66924428939819,91.33017659187317}, {55.642688274383545,96.01919651031494}, {55.642688274383545,87.108314037323}}),
mapFloor::triangle({{55.642688274383545,87.108314037323}, {70.33275365829468,98.01185727119446}, {70.33275365829468,84.9461317062378}}),mapFloor::triangle({{85.12771725654602,98.94925355911255}, {99.92268085479736,80.53086996078491}, {85.12771725654602,77.4810791015625}}),mapFloor::triangle({{70.33275365829468,84.9461317062378}, {85.12771725654602,98.94925355911255}, {85.12771725654602,77.4810791015625}}),
mapFloor::triangle({{29.753458499908447,90.6181812286377}, {22.556772828102112,88.47372531890869}, {18.271654844284058,92.00506210327148}}),mapFloor::triangle({{22.556772828102112,85.04118919372559}, {29.054594039916992,81.41239881515503}, {18.620945513248444,81.79606199264526}}),mapFloor::triangle({{29.054591059684753,87.63798475265503}, {22.556772828102112,85.04118919372559}, {22.556772828102112,88.47372531890869}}),
mapFloor::triangle({{29.054594039916992,81.41239881515503}, {24.931316077709198,71.43762111663818}, {18.620945513248444,81.79606199264526}}),mapFloor::triangle({{33.09163749217987,76.09100341796875}, {34.22519266605377,62.14852333068848}, {24.931316077709198,71.43762111663818}}),mapFloor::triangle({{56.72540068626404,67.3406720161438}, {37.93248534202576,52.509552240371704}, {36.078840494155884,57.32903480529785}}),
mapFloor::triangle({{40.04891514778137,70.71596384048462}, {36.078840494155884,57.32903480529785}, {34.22519266605377,62.14852333068848}}),mapFloor::triangle({{53.221118450164795,52.85990238189697}, {47.390809655189514,55.22400140762329}, {52.40629315376282,55.65769672393799}}),mapFloor::triangle({{44.35563087463379,53.32358479499817}, {45.29799818992615,54.252493381500244}, {45.53695321083069,53.53021025657654}}),
mapFloor::triangle({{41.68132543563843,52.99226641654968}, {42.02336072921753,53.81617546081543}, {42.26231575012207,53.093886375427246}}),mapFloor::triangle({{39.27530646324158,52.392709255218506}, {37.651750445365906,52.94232368469238}, {39.26505148410797,53.097522258758545}}),mapFloor::triangle({{38.15609812736511,52.28326916694641}, {34.13381576538086,57.25926160812378}, {37.24566102027893,52.45164632797241}}),
mapFloor::triangle({{34.13381576538086,57.25926160812378}, {35.73026955127716,52.25265622138977}, {37.24566102027893,52.45164632797241}}),mapFloor::triangle({{32.36163556575775,55.14322519302368}, {34.980228543281555,51.04340314865112}, {35.73026955127716,52.25265622138977}}),mapFloor::triangle({{29.504981637001038,45.118504762649536}, {31.039106845855713,44.98625099658966}, {30.986210703849792,50.77890157699585}}),
mapFloor::triangle({{31.039106845855713,44.98625099658966}, {31.80617094039917,45.885565876960754}, {30.986210703849792,50.77890157699585}}),mapFloor::triangle({{30.986210703849792,50.77890157699585}, {31.80617094039917,45.885565876960754}, {34.980228543281555,51.04340314865112}}),mapFloor::triangle({{31.80617094039917,45.885565876960754}, {35.006678104400635,46.573275327682495}, {34.980228543281555,51.04340314865112}}),
mapFloor::triangle({{64.38435912132263,62.488824129104614}, {68.31786632537842,55.006927251815796}, {70.38880586624146,57.939743995666504}}),mapFloor::triangle({{54.56417798995972,54.919588565826416}, {55.45340180397034,56.133854389190674}, {55.743223428726196,55.138856172561646}}),mapFloor::triangle({{57.650065422058105,55.47063946723938}, {58.539289236068726,56.68490529060364}, {58.829110860824585,55.68990707397461}}),
mapFloor::triangle({{60.77273488044739,56.19435906410217}, {62.4065637588501,57.509517669677734}, {62.89755702018738,56.52485489845276}}),mapFloor::triangle({{62.89755702018738,56.52485489845276}, {64.32532668113708,57.787007093429565}, {64.61514830589294,56.79200887680054}}),mapFloor::triangle({{62.89755702018738,56.52485489845276}, {61.593979597091675,55.91084361076355}, {60.77273488044739,56.19435906410217}}),
mapFloor::triangle({{70.38880586624146,57.939743995666504}, {74.6977150440216,52.055054903030396}, {76.76865458488464,54.9878716468811}}),mapFloor::triangle({{76.76865458488464,54.9878716468811}, {85.21936535835266,47.68105745315552}, {88.02101016044617,49.92627799510956}}),mapFloor::triangle({{36.53962016105652,83.35465788841248}, {84.02966260910034,70.4902172088623}, {36.104580760002136,81.90499544143677}}),
mapFloor::triangle({{84.46470499038696,71.93987965583801}, {88.43803405761719,69.53911781311035}, {84.02966260910034,70.4902172088623}}),mapFloor::triangle({{88.87307643890381,70.98878026008606}, {97.78546094894409,67.37669110298157}, {88.43803405761719,69.53911781311035}}),mapFloor::triangle({{84.02966260910034,70.4902172088623}, {87.52596378326416,67.54330396652222}, {83.11759233474731,68.49440336227417}}),
mapFloor::triangle({{18.425588309764862,96.11464142799377}, {18.571951985359192,100.02213716506958}, {29.18049693107605,100.02213716506958}}),mapFloor::triangle({{11.677411198616028,92.17037558555603}, {0.028006016509607434,92.17037558555603}, {0.02800583024509251,96.27995491027832}}),mapFloor::triangle({{17.176538705825806,96.27995491027832}, {17.02260673046112,92.88674592971802}, {13.679563999176025,93.4194266796112}}),
mapFloor::triangle({{13.679563999176025,93.4194266796112}, {11.677411198616028,92.17037558555603}, {11.831343173980713,96.27995491027832}}),mapFloor::triangle({{18.425588309764862,96.11464142799377}, {18.271654844284058,92.00506210327148}, {17.02260673046112,92.88674592971802}}),mapFloor::triangle({{18.271654844284058,92.00506210327148}, {18.425588309764862,96.11464142799377}, {29.424387216567993,96.01919651031494}}),
mapFloor::triangle({{11.677411198616028,92.17037558555603}, {21.96134328842163,67.16897487640381}, {10.31193733215332,67.16897487640381}}),mapFloor::triangle({{25.685718655586243,61.68085336685181}, {26.027822494506836,56.19272589683533}, {11.180254817008972,56.19272589683533}}),mapFloor::triangle({{21.96134328842163,67.16897487640381}, {25.685718655586243,61.68085336685181}, {11.203084141016006,61.68085336685181}}),
mapFloor::triangle({{4.150979220867157,38.1668359041214}, {18.99854689836502,38.16683292388916}, {15.012656152248383,32.15820491313934}}),mapFloor::triangle({{11.180254817008972,56.19272589683533}, {26.027822494506836,56.19272589683533}, {24.506129324436188,50.18409490585327}}),mapFloor::triangle({{9.658561646938324,50.18409490585327}, {24.506129324436188,50.18409490585327}, {21.752338111400604,44.175463914871216}}),
mapFloor::triangle({{6.9047704339027405,44.175466895103455}, {21.752338111400604,44.175463914871216}, {18.99854689836502,38.16683292388916}}),mapFloor::triangle({{0.16508987173438072,32.15820491313934}, {15.012656152248383,32.15820491313934}, {6.653813272714615,18.008416891098022}}),mapFloor::triangle({{0.1650904887355864,18.008416891098022}, {6.653813272714615,18.008416891098022}, {3.557944670319557,12.898267805576324}}),
mapFloor::triangle({{29.424387216567993,96.01919651031494}, {31.530937552452087,96.01919651031494}, {31.530937552452087,91.33017659187317}}),mapFloor::triangle({{31.530937552452087,96.01919651031494}, {34.66924428939819,96.01919651031494}, {34.66924428939819,91.33017659187317}}),mapFloor::triangle({{34.66924428939819,91.33017659187317}, {34.66924428939819,96.01919651031494}, {55.642688274383545,96.01919651031494}}),
mapFloor::triangle({{55.642688274383545,87.108314037323}, {55.642688274383545,96.01919651031494}, {70.33275365829468,98.01185727119446}}),mapFloor::triangle({{85.12771725654602,98.94925355911255}, {99.92268085479736,99.88664388656616}, {99.92268085479736,80.53086996078491}}),mapFloor::triangle({{70.33275365829468,84.9461317062378}, {70.33275365829468,98.01185727119446}, {85.12771725654602,98.94925355911255}}),
mapFloor::triangle({{29.753458499908447,90.6181812286377}, {29.054591059684753,87.63798475265503}, {22.556772828102112,88.47372531890869}}),mapFloor::triangle({{22.556772828102112,85.04118919372559}, {29.054591059684753,84.51884984970093}, {29.054594039916992,81.41239881515503}}),mapFloor::triangle({{29.054591059684753,87.63798475265503}, {29.054591059684753,84.51884984970093}, {22.556772828102112,85.04118919372559}}),
mapFloor::triangle({{29.054594039916992,81.41239881515503}, {33.09163749217987,76.09100341796875}, {24.931316077709198,71.43762111663818}}),mapFloor::triangle({{33.09163749217987,76.09100341796875}, {40.04891514778137,70.71596384048462}, {34.22519266605377,62.14852333068848}}),mapFloor::triangle({{56.72540068626404,67.3406720161438}, {68.95819902420044,58.69644284248352}, {37.93248534202576,52.509552240371704}}),
mapFloor::triangle({{40.04891514778137,70.71596384048462}, {56.72540068626404,67.3406720161438}, {36.078840494155884,57.32903480529785}}),mapFloor::triangle({{53.221118450164795,52.85990238189697}, {47.390809655189514,51.84012055397034}, {47.390809655189514,55.22400140762329}}),mapFloor::triangle({{44.35563087463379,53.32358479499817}, {44.281771779060364,54.164618253707886}, {45.29799818992615,54.252493381500244}}),
mapFloor::triangle({{41.68132543563843,52.99226641654968}, {40.91855585575104,53.410136699676514}, {42.02336072921753,53.81617546081543}}),mapFloor::triangle({{39.27530646324158,52.392709255218506}, {37.78132498264313,52.10805535316467}, {37.651750445365906,52.94232368469238}}),mapFloor::triangle({{38.15609812736511,52.28326916694641}, {35.905998945236206,59.37529802322388}, {34.13381576538086,57.25926160812378}}),
mapFloor::triangle({{34.13381576538086,57.25926160812378}, {32.36163556575775,55.14322519302368}, {35.73026955127716,52.25265622138977}}),mapFloor::triangle({{32.36163556575775,55.14322519302368}, {30.986210703849792,50.77890157699585}, {34.980228543281555,51.04340314865112}}),mapFloor::triangle({{31.80617094039917,45.885565876960754}, {32.46743381023407,45.99136710166931}, {35.006678104400635,46.573275327682495}}),
mapFloor::triangle({{64.38435912132263,62.488824129104614}, {61.08102202415466,57.81071186065674}, {68.31786632537842,55.006927251815796}}),mapFloor::triangle({{54.56417798995972,54.919588565826416}, {54.44459915161133,55.98297119140625}, {55.45340180397034,56.133854389190674}}),mapFloor::triangle({{57.650065422058105,55.47063946723938}, {57.53048658370972,56.534022092819214}, {58.539289236068726,56.68490529060364}}),
mapFloor::triangle({{60.77273488044739,56.19435906410217}, {60.347479581832886,57.18100070953369}, {62.4065637588501,57.509517669677734}}),mapFloor::triangle({{62.89755702018738,56.52485489845276}, {62.4065637588501,57.509517669677734}, {64.32532668113708,57.787007093429565}}),mapFloor::triangle({{62.89755702018738,56.52485489845276}, {62.46653199195862,56.046563386917114}, {61.593979597091675,55.91084361076355}}),
mapFloor::triangle({{70.38880586624146,57.939743995666504}, {68.31786632537842,55.006927251815796}, {74.6977150440216,52.055054903030396}}),mapFloor::triangle({{76.76865458488464,54.9878716468811}, {74.6977150440216,52.055054903030396}, {85.21936535835266,47.68105745315552}}),mapFloor::triangle({{36.53962016105652,83.35465788841248}, {84.46470499038696,71.93987965583801}, {84.02966260910034,70.4902172088623}}),
mapFloor::triangle({{84.46470499038696,71.93987965583801}, {88.87307643890381,70.98878026008606}, {88.43803405761719,69.53911781311035}}),mapFloor::triangle({{88.87307643890381,70.98878026008606}, {98.22050333023071,68.82635354995728}, {97.78546094894409,67.37669110298157}}),mapFloor::triangle({{84.02966260910034,70.4902172088623}, {88.43803405761719,69.53911781311035}, {87.52596378326416,67.54330396652222}}),}),true,{}),
											mapFloor("Stone", List<mapFloor::triangle>({mapFloor::triangle({{46.45344018936157,48.13438355922699}, {53.231436014175415,55.1128625869751}, {46.286171674728394,54.11650538444519}}),mapFloor::triangle({{88.36116194725037,69.67945694923401}, {84.46469902992249,71.93988561630249}, {83.91356468200684,70.53025960922241}}),mapFloor::triangle({{87.50855326652527,67.49874353408813}, {83.91356468200684,70.53025960922241}, {83.11759233474731,68.49440336227417}}),
mapFloor::triangle({{88.87307643890381,70.98878026008606}, {97.62806296348572,67.53566265106201}, {98.22050333023071,68.82635354995728}}),mapFloor::triangle({{83.91356468200684,70.53025960922241}, {36.53962016105652,83.35465788841248}, {36.104580760002136,81.90499544143677}}),mapFloor::triangle({{46.45344018936157,48.13438355922699}, {54.817503690719604,49.334290623664856}, {53.231436014175415,55.1128625869751}}),
mapFloor::triangle({{88.36116194725037,69.67945694923401}, {88.87307643890381,70.98878026008606}, {84.46469902992249,71.93988561630249}}),mapFloor::triangle({{87.50855326652527,67.49874353408813}, {88.36116194725037,69.67945694923401}, {83.91356468200684,70.53025960922241}}),mapFloor::triangle({{88.87307643890381,70.98878026008606}, {88.36116194725037,69.67945694923401}, {97.62806296348572,67.53566265106201}}),
mapFloor::triangle({{83.91356468200684,70.53025960922241}, {84.46469902992249,71.93988561630249}, {36.53962016105652,83.35465788841248}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "STONE"})})),
											mapFloor("Grass", List<mapFloor::triangle>({mapFloor::triangle({{44.57570016384125,63.645631074905396}, {34.328269958496094,56.930023431777954}, {31.349855661392212,63.645631074905396}}),mapFloor::triangle({{22.556772828102112,85.04118919372559}, {13.061405718326569,78.597092628479}, {10.60677319765091,85.04118919372559}}),mapFloor::triangle({{22.556772828102112,85.04118919372559}, {10.60677319765091,88.47372531890869}, {22.556772828102112,88.47372531890869}}),
							mapFloor::triangle({{41.94469153881073,65.972238779068}, {31.349855661392212,63.645631074905396}, {28.87129783630371,65.972238779068}}),mapFloor::triangle({{27.332475781440735,75.37504434585571}, {17.322339117527008,72.15299606323242}, {15.105758607387543,75.37504434585571}}),mapFloor::triangle({{34.11920666694641,69.43235993385315}, {26.392734050750732,67.41076111793518}, {21.49924486875534,69.43235993385315}}),
							mapFloor::triangle({{47.737306356430054,54.43822741508484}, {34.328269958496094,56.930023431777954}, {47.737306356430054,56.930023431777954}}),mapFloor::triangle({{29.68539297580719,72.15299606323242}, {21.49924486875534,69.43235993385315}, {17.322339117527008,72.15299606323242}}),mapFloor::triangle({{39.31368291378021,67.41076111793518}, {28.87129783630371,65.972238779068}, {26.392734050750732,67.41076111793518}}),
							mapFloor::triangle({{25.162380933761597,78.597092628479}, {15.105758607387543,75.37504434585571}, {13.061405718326569,78.597092628479}}),mapFloor::triangle({{22.556772828102112,88.47372531890869}, {10.60677394270897,97.02993631362915}, {22.556772828102112,97.02993631362915}}),mapFloor::triangle({{68.62673759460449,85.22711396217346}, {100.023353099823,99.41045641899109}, {100.023353099823,83.17570090293884}}),
							mapFloor::triangle({{25.22178888320923,88.06313872337341}, {68.62673759460449,98.41127395629883}, {68.62673759460449,85.22711396217346}}),mapFloor::triangle({{100.023353099823,83.17570090293884}, {68.62673759460449,78.41885089874268}, {68.62673759460449,85.22711396217346}}),mapFloor::triangle({{31.31091296672821,73.34522008895874}, {52.496516704559326,76.21666193008423}, {54.43696975708008,73.34522008895874}}),
							mapFloor::triangle({{25.83216428756714,84.8309874534607}, {48.34784269332886,88.06313872337341}, {48.95821809768677,84.8309874534607}}),mapFloor::triangle({{42.86315441131592,67.55860447883606}, {62.6322865486145,69.41741108894348}, {65.98920822143555,67.55860447883606}}),mapFloor::triangle({{46.220073103904724,64.81172442436218}, {65.98920822143555,67.55860447883606}, {69.34612989425659,64.81172442436218}}),
							mapFloor::triangle({{27.61283814907074,79.08810377120972}, {48.95821809768677,84.8309874534607}, {50.73889493942261,79.08810377120972}}),mapFloor::triangle({{29.370462894439697,76.21666193008423}, {50.73889493942261,79.08810377120972}, {52.496516704559326,76.21666193008423}}),mapFloor::triangle({{35.02824306488037,71.03180289268494}, {54.43696975708008,73.34522008895874}, {58.154296875,71.03180289268494}}),
							mapFloor::triangle({{39.50623571872711,69.41741108894348}, {58.154296875,71.03180289268494}, {62.6322865486145,69.41741108894348}}),mapFloor::triangle({{63.281917572021484,58.52283239364624}, {55.53277134895325,67.51523017883301}, {63.281917572021484,68.16364526748657}}),mapFloor::triangle({{46.220073103904724,64.81172442436218}, {49.88853931427002,66.96761846542358}, {49.88853931427002,57.326799631118774}}),
							mapFloor::triangle({{83.56088399887085,51.322442293167114}, {87.14271187782288,58.924371004104614}, {87.14271187782288,49.28355515003204}}),mapFloor::triangle({{52.26941704750061,57.58877992630005}, {49.88853931427002,66.96761846542358}, {52.26941704750061,67.22959280014038}}),mapFloor::triangle({{54.42727208137512,57.74685740470886}, {52.26941704750061,67.22959280014038}, {54.42727208137512,67.38767623901367}}),
							mapFloor::triangle({{75.95639228820801,53.61848473548889}, {83.56088399887085,60.963261127471924}, {83.56088399887085,51.322442293167114}}),mapFloor::triangle({{64.49455618858337,58.52283239364624}, {75.95639228820801,63.2593035697937}, {75.95639228820801,53.61848473548889}}),mapFloor::triangle({{63.281917572021484,58.52283239364624}, {64.49455618858337,68.16364526748657}, {64.49455618858337,58.52283239364624}}),
							mapFloor::triangle({{55.53277134895325,57.874417304992676}, {54.42727208137512,67.38767623901367}, {55.53277134895325,67.51523017883301}}),mapFloor::triangle({{44.57570016384125,63.645631074905396}, {47.737306356430054,56.930023431777954}, {34.328269958496094,56.930023431777954}}),mapFloor::triangle({{22.556772828102112,85.04118919372559}, {25.162380933761597,78.597092628479}, {13.061405718326569,78.597092628479}}),
							mapFloor::triangle({{22.556772828102112,85.04118919372559}, {10.60677319765091,85.04118919372559}, {10.60677319765091,88.47372531890869}}),mapFloor::triangle({{41.94469153881073,65.972238779068}, {44.57570016384125,63.645631074905396}, {31.349855661392212,63.645631074905396}}),mapFloor::triangle({{27.332475781440735,75.37504434585571}, {29.68539297580719,72.15299606323242}, {17.322339117527008,72.15299606323242}}),
							mapFloor::triangle({{34.11920666694641,69.43235993385315}, {39.31368291378021,67.41076111793518}, {26.392734050750732,67.41076111793518}}),mapFloor::triangle({{47.737306356430054,54.43822741508484}, {34.328269958496094,51.92437767982483}, {34.328269958496094,56.930023431777954}}),mapFloor::triangle({{29.68539297580719,72.15299606323242}, {34.11920666694641,69.43235993385315}, {21.49924486875534,69.43235993385315}}),
							mapFloor::triangle({{39.31368291378021,67.41076111793518}, {41.94469153881073,65.972238779068}, {28.87129783630371,65.972238779068}}),mapFloor::triangle({{25.162380933761597,78.597092628479}, {27.332475781440735,75.37504434585571}, {15.105758607387543,75.37504434585571}}),mapFloor::triangle({{22.556772828102112,88.47372531890869}, {10.60677319765091,88.47372531890869}, {10.60677394270897,97.02993631362915}}),
							mapFloor::triangle({{68.62673759460449,85.22711396217346}, {68.62673759460449,98.41127395629883}, {100.023353099823,99.41045641899109}}),mapFloor::triangle({{25.22178888320923,88.06313872337341}, {25.22178888320923,97.02993631362915}, {68.62673759460449,98.41127395629883}}),mapFloor::triangle({{100.023353099823,83.17570090293884}, {100.023353099823,76.36743783950806}, {68.62673759460449,78.41885089874268}}),
							mapFloor::triangle({{31.31091296672821,73.34522008895874}, {29.370462894439697,76.21666193008423}, {52.496516704559326,76.21666193008423}}),mapFloor::triangle({{25.83216428756714,84.8309874534607}, {25.22178888320923,88.06313872337341}, {48.34784269332886,88.06313872337341}}),mapFloor::triangle({{42.86315441131592,67.55860447883606}, {39.50623571872711,69.41741108894348}, {62.6322865486145,69.41741108894348}}),
							mapFloor::triangle({{46.220073103904724,64.81172442436218}, {42.86315441131592,67.55860447883606}, {65.98920822143555,67.55860447883606}}),mapFloor::triangle({{27.61283814907074,79.08810377120972}, {25.83216428756714,84.8309874534607}, {48.95821809768677,84.8309874534607}}),mapFloor::triangle({{29.370462894439697,76.21666193008423}, {27.61283814907074,79.08810377120972}, {50.73889493942261,79.08810377120972}}),
							mapFloor::triangle({{35.02824306488037,71.03180289268494}, {31.31091296672821,73.34522008895874}, {54.43696975708008,73.34522008895874}}),mapFloor::triangle({{39.50623571872711,69.41741108894348}, {35.02824306488037,71.03180289268494}, {58.154296875,71.03180289268494}}),mapFloor::triangle({{63.281917572021484,58.52283239364624}, {55.53277134895325,57.874417304992676}, {55.53277134895325,67.51523017883301}}),
							mapFloor::triangle({{46.220073103904724,64.81172442436218}, {46.220073103904724,74.45254325866699}, {49.88853931427002,66.96761846542358}}),mapFloor::triangle({{83.56088399887085,51.322442293167114}, {83.56088399887085,60.963261127471924}, {87.14271187782288,58.924371004104614}}),mapFloor::triangle({{52.26941704750061,57.58877992630005}, {49.88853931427002,57.326799631118774}, {49.88853931427002,66.96761846542358}}),
							mapFloor::triangle({{54.42727208137512,57.74685740470886}, {52.26941704750061,57.58877992630005}, {52.26941704750061,67.22959280014038}}),mapFloor::triangle({{75.95639228820801,53.61848473548889}, {75.95639228820801,63.2593035697937}, {83.56088399887085,60.963261127471924}}),mapFloor::triangle({{64.49455618858337,58.52283239364624}, {64.49455618858337,68.16364526748657}, {75.95639228820801,63.2593035697937}}),
							mapFloor::triangle({{63.281917572021484,58.52283239364624}, {63.281917572021484,68.16364526748657}, {64.49455618858337,68.16364526748657}}),mapFloor::triangle({{55.53277134895325,57.874417304992676}, {54.42727208137512,57.74685740470886}, {54.42727208137512,67.38767623901367}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "GRASS"})})),
											mapFloor("Beach", List<mapFloor::triangle>({mapFloor::triangle({{18.63568276166916,72.15299606323242}, {3.736230358481407,69.43235993385315}, {21.388080716133118,69.43235993385315}}),mapFloor::triangle({{14.210417866706848,97.02993631362915}, {0.5134537816047668,88.47372531890869}, {14.210417866706848,88.47372531890869}}),mapFloor::triangle({{15.827913582324982,78.597092628479}, {1.8445776775479317,75.37504434585571}, {17.17505306005478,75.37504434585571}}),
							mapFloor::triangle({{24.612677097320557,67.41076111793518}, {5.917414277791977,65.972238779068}, {26.24594271183014,65.972238779068}}),mapFloor::triangle({{26.24594271183014,65.972238779068}, {6.6507503390312195,63.645631074905396}, {27.879205346107483,63.645631074905396}}),mapFloor::triangle({{17.17505306005478,75.37504434585571}, {2.5004014372825623,72.15299606323242}, {18.63568276166916,72.15299606323242}}),
							mapFloor::triangle({{21.388080716133118,69.43235993385315}, {5.184076726436615,67.41076111793518}, {24.612677097320557,67.41076111793518}}),mapFloor::triangle({{27.879205346107483,63.645631074905396}, {7.53197968006134,56.930023431777954}, {29.841849207878113,56.930023431777954}}),mapFloor::triangle({{14.210417866706848,85.04118919372559}, {1.2397108599543571,78.597092628479}, {15.827913582324982,78.597092628479}}),
							mapFloor::triangle({{29.841849207878113,56.930023431777954}, {7.53197968006134,51.92437767982483}, {29.841849207878113,51.92437767982483}}),mapFloor::triangle({{14.210417866706848,88.47372531890869}, {0.5134537816047668,85.04118919372559}, {14.210417866706848,85.04118919372559}}),mapFloor::triangle({{7.53197968006134,51.92437767982483}, {-1.1501282453536987,-0.9609699249267578}, {29.841849207878113,51.92437767982483}}),
							mapFloor::triangle({{18.63568276166916,72.15299606323242}, {2.5004014372825623,72.15299606323242}, {3.736230358481407,69.43235993385315}}),mapFloor::triangle({{14.210417866706848,97.02993631362915}, {0.513453409075737,97.02993631362915}, {0.5134537816047668,88.47372531890869}}),mapFloor::triangle({{15.827913582324982,78.597092628479}, {1.2397108599543571,78.597092628479}, {1.8445776775479317,75.37504434585571}}),
							mapFloor::triangle({{24.612677097320557,67.41076111793518}, {5.184076726436615,67.41076111793518}, {5.917414277791977,65.972238779068}}),mapFloor::triangle({{26.24594271183014,65.972238779068}, {5.917414277791977,65.972238779068}, {6.6507503390312195,63.645631074905396}}),mapFloor::triangle({{17.17505306005478,75.37504434585571}, {1.8445776775479317,75.37504434585571}, {2.5004014372825623,72.15299606323242}}),
							mapFloor::triangle({{21.388080716133118,69.43235993385315}, {3.736230358481407,69.43235993385315}, {5.184076726436615,67.41076111793518}}),mapFloor::triangle({{27.879205346107483,63.645631074905396}, {6.6507503390312195,63.645631074905396}, {7.53197968006134,56.930023431777954}}),mapFloor::triangle({{14.210417866706848,85.04118919372559}, {0.5134537816047668,85.04118919372559}, {1.2397108599543571,78.597092628479}}),
							mapFloor::triangle({{29.841849207878113,56.930023431777954}, {7.53197968006134,56.930023431777954}, {7.53197968006134,51.92437767982483}}),mapFloor::triangle({{14.210417866706848,88.47372531890869}, {0.5134537816047668,88.47372531890869}, {0.5134537816047668,85.04118919372559}}),mapFloor::triangle({{7.53197968006134,51.92437767982483}, {-23.45999777317047,-0.9609699249267578}, {-1.1501282453536987,-0.9609699249267578}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "BEACH"})})),
											mapFloor("Gravel", List<mapFloor::triangle>({mapFloor::triangle({{25.22178888320923,88.06313872337341}, {22.556772828102112,85.04118919372559}, {22.556772828102112,88.47372531890869}}),mapFloor::triangle({{55.53277134895325,55.9597909450531}, {54.714274406433105,54.88609075546265}, {54.42727208137512,55.70458769798279}}),mapFloor::triangle({{54.42727208137512,57.74685740470886}, {55.53277134895325,55.9597909450531}, {54.42727208137512,55.70458769798279}}),
							mapFloor::triangle({{52.26941704750061,57.58877992630005}, {54.42727208137512,55.70458769798279}, {52.26941704750061,55.288660526275635}}),mapFloor::triangle({{47.737306356430054,56.930023431777954}, {49.88853931427002,54.83500361442566}, {47.737306356430054,54.43822741508484}}),mapFloor::triangle({{49.88853931427002,57.326799631118774}, {52.26941704750061,55.288660526275635}, {49.88853931427002,54.83500361442566}}),
							mapFloor::triangle({{46.220073103904724,64.81172442436218}, {47.737306356430054,56.930023431777954}, {44.57570016384125,63.645631074905396}}),mapFloor::triangle({{31.31091296672821,73.34522008895874}, {34.11920666694641,69.43235993385315}, {29.68539297580719,72.15299606323242}}),mapFloor::triangle({{42.86315441131592,67.55860447883606}, {44.57570016384125,63.645631074905396}, {41.94469153881073,65.972238779068}}),
							mapFloor::triangle({{34.11920666694641,69.43235993385315}, {39.50623571872711,69.41741108894348}, {39.31368291378021,67.41076111793518}}),mapFloor::triangle({{25.83216428756714,84.8309874534607}, {25.162380933761597,78.597092628479}, {22.556772828102112,85.04118919372559}}),mapFloor::triangle({{27.61283814907074,79.08810377120972}, {27.332475781440735,75.37504434585571}, {25.162380933761597,78.597092628479}}),
							mapFloor::triangle({{29.370462894439697,76.21666193008423}, {29.68539297580719,72.15299606323242}, {27.332475781440735,75.37504434585571}}),mapFloor::triangle({{39.31368291378021,67.41076111793518}, {42.86315441131592,67.55860447883606}, {41.94469153881073,65.972238779068}}),mapFloor::triangle({{22.556772828102112,88.47372531890869}, {25.22178888320923,97.02993631362915}, {25.22178888320923,88.06313872337341}}),
							mapFloor::triangle({{55.53277134895325,57.874417304992676}, {63.281917572021484,56.608206033706665}, {55.53277134895325,55.9597909450531}}),mapFloor::triangle({{55.81977367401123,55.14129400253296}, {63.281917572021484,56.608206033706665}, {63.388216495513916,55.68341016769409}}),mapFloor::triangle({{63.281917572021484,58.52283239364624}, {64.49455618858337,56.608206033706665}, {63.281917572021484,56.608206033706665}}),
							mapFloor::triangle({{64.49455618858337,56.608206033706665}, {75.95639228820801,53.61848473548889}, {75.95639228820801,51.703858375549316}}),mapFloor::triangle({{75.95639228820801,51.703858375549316}, {83.56088399887085,51.322442293167114}, {83.56088399887085,49.4078129529953}}),mapFloor::triangle({{83.56088399887085,49.4078129529953}, {87.14271187782288,49.28355515003204}, {85.54466366767883,48.08529019355774}}),
							mapFloor::triangle({{25.22178888320923,88.06313872337341}, {25.83216428756714,84.8309874534607}, {22.556772828102112,85.04118919372559}}),mapFloor::triangle({{55.53277134895325,55.9597909450531}, {55.81977367401123,55.14129400253296}, {54.714274406433105,54.88609075546265}}),mapFloor::triangle({{54.42727208137512,57.74685740470886}, {55.53277134895325,57.874417304992676}, {55.53277134895325,55.9597909450531}}),
							mapFloor::triangle({{52.26941704750061,57.58877992630005}, {54.42727208137512,57.74685740470886}, {54.42727208137512,55.70458769798279}}),mapFloor::triangle({{47.737306356430054,56.930023431777954}, {49.88853931427002,57.326799631118774}, {49.88853931427002,54.83500361442566}}),mapFloor::triangle({{49.88853931427002,57.326799631118774}, {52.26941704750061,57.58877992630005}, {52.26941704750061,55.288660526275635}}),
							mapFloor::triangle({{46.220073103904724,64.81172442436218}, {49.88853931427002,57.326799631118774}, {47.737306356430054,56.930023431777954}}),mapFloor::triangle({{31.31091296672821,73.34522008895874}, {35.02824306488037,71.03180289268494}, {34.11920666694641,69.43235993385315}}),mapFloor::triangle({{42.86315441131592,67.55860447883606}, {46.220073103904724,64.81172442436218}, {44.57570016384125,63.645631074905396}}),
							mapFloor::triangle({{34.11920666694641,69.43235993385315}, {35.02824306488037,71.03180289268494}, {39.50623571872711,69.41741108894348}}),mapFloor::triangle({{25.83216428756714,84.8309874534607}, {27.61283814907074,79.08810377120972}, {25.162380933761597,78.597092628479}}),mapFloor::triangle({{27.61283814907074,79.08810377120972}, {29.370462894439697,76.21666193008423}, {27.332475781440735,75.37504434585571}}),
							mapFloor::triangle({{29.370462894439697,76.21666193008423}, {31.31091296672821,73.34522008895874}, {29.68539297580719,72.15299606323242}}),mapFloor::triangle({{39.31368291378021,67.41076111793518}, {39.50623571872711,69.41741108894348}, {42.86315441131592,67.55860447883606}}),mapFloor::triangle({{22.556772828102112,88.47372531890869}, {22.556772828102112,97.02993631362915}, {25.22178888320923,97.02993631362915}}),
							mapFloor::triangle({{55.53277134895325,57.874417304992676}, {63.281917572021484,58.52283239364624}, {63.281917572021484,56.608206033706665}}),mapFloor::triangle({{55.81977367401123,55.14129400253296}, {55.53277134895325,55.9597909450531}, {63.281917572021484,56.608206033706665}}),mapFloor::triangle({{63.281917572021484,58.52283239364624}, {64.49455618858337,58.52283239364624}, {64.49455618858337,56.608206033706665}}),
							mapFloor::triangle({{64.49455618858337,56.608206033706665}, {64.49455618858337,58.52283239364624}, {75.95639228820801,53.61848473548889}}),mapFloor::triangle({{75.95639228820801,51.703858375549316}, {75.95639228820801,53.61848473548889}, {83.56088399887085,51.322442293167114}}),mapFloor::triangle({{83.56088399887085,49.4078129529953}, {83.56088399887085,51.322442293167114}, {87.14271187782288,49.28355515003204}}),}),false,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "GRAVEL"})})),
											}), {}, { 5000,5000 }, { Map<string, string>({
												pair<string,string>({"song1",to_string(WINDOUTSIDE1_WAV) + " " + "AmbienceVolume/2"}),
												pair<string,string>({"song2",to_string(OCEAN_LEFT_PAN_WAV) + " " + "AmbienceVolume/2"}),
												pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
												}) });
		maps["ChapelEntryHall"] = mapInstance("ChapelEntryHall", EMPTYMAP, { 50,70 }, List<mapObject>({
			mapObject("AT_ChapelLeft", true, false, false, "","0",0,0,"1","1.0","CENTRE",{25, 51}, false, List<mapFloor::triangle>({mapFloor::triangle({{28.228163719177246,54.26681637763977}, {24.764961004257202,56.846749782562256}, {26.152867078781128,57.73525834083557}}),mapFloor::triangle({{28.228163719177246,54.26681637763977}, {26.029062271118164,53.41002345085144}, {24.764961004257202,56.846749782562256}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Chapel Left Wing"}),
												pair<string, string>({"copy","AT_ChapelLeft"}),
												pair<string, string>({"cutscene","Chapel2LeftWing"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}),})),
			mapObject("AT_ChapelRight", true, false, false, "","0",0,0,"1","1.0","CENTRE",{75, 52}, false, List<mapFloor::triangle>({mapFloor::triangle({{71.70406579971313,54.26681637763977}, {75.16726851463318,56.846749782562256}, {73.77936244010925,57.73525834083557}}),mapFloor::triangle({{71.70406579971313,54.26681637763977}, {73.90316724777222,53.41002345085144}, {75.16726851463318,56.846749782562256}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Chapel Right Wing"}),
												pair<string, string>({"copy","AT_ChapelRight"}),
												pair<string, string>({"areaTransition","ChapelRight1"}),
												pair<string, string>({"direction","STAND_RIGHT"}),
												pair<string, string>({"playerPosX","3"}),
												pair<string, string>({"playerPosY","42"}),
												pair<string, string>({"audio","CHAPELDOOR"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}),})),
			mapObject("AT_Chapel2Nave", true, false, false, "","0",0,0,"1","1.0","CENTRE",{50, 54}, false, List<mapFloor::triangle>({mapFloor::triangle({{49.995625019073486,57.37406015396118}, {52.03852653503418,58.47830772399902}, {52.16870307922363,57.38120675086975}}),mapFloor::triangle({{49.995625019073486,57.37406015396118}, {47.95272350311279,58.47830772399902}, {47.82254695892334,57.38120675086975}}),mapFloor::triangle({{49.995625019073486,57.37406015396118}, {49.995625019073486,58.47159028053284}, {52.03852653503418,58.47830772399902}}),
mapFloor::triangle({{49.995625019073486,57.37406015396118}, {49.995625019073486,58.47159028053284}, {47.95272350311279,58.47830772399902}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter the Nave"}),
												pair<string, string>({"copy","AT_Chapel2Nave"}),
												pair<string, string>({"cutscene","EnterTheChapelNave"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","-3"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "CENTRE"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>("don'tLoadIfNot", "BloodWallDestroyed"),
												pair<string, string>({"format","LightText_20"}),})),
			mapObject("AT_Nave2Chapel", true, false, false, "","0",0,0,"1","1.0","CENTRE",{50, 54}, false, List<mapFloor::triangle>({mapFloor::triangle({{49.995625019073486,44.096729159355164}, {52.03852653503418,45.200976729393005}, {52.16870307922363,44.10387575626373}}),mapFloor::triangle({{49.995625019073486,44.096729159355164}, {47.95272350311279,45.200976729393005}, {47.82254695892334,44.10387575626373}}),mapFloor::triangle({{49.995625019073486,44.096729159355164}, {49.995625019073486,45.19425928592682}, {52.03852653503418,45.200976729393005}}),
mapFloor::triangle({{49.995625019073486,44.096729159355164}, {49.995625019073486,45.19425928592682}, {47.95272350311279,45.200976729393005}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Return to Entry Hall"}),
												pair<string, string>({"copy","AT_Nave2Chapel"}),
												pair<string, string>({"cutscene","LeaveTheChapelNave"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","-3"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "CENTRE"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>("don'tLoadIfNot", "BloodWallDestroyed"),
												pair<string, string>({"format","LightText_20"}),})),
			mapObject("ChapelEntryAsObject", false, true, false, imageLookup.getSequenceAsString("ChapelEntryAsObject","STAND_FRONT"),"0",0,imageLookup.layerDefaults["Map"] + 1,"1.0","1.5","CENTRE",{50, 50}, false, {}, {}),
			mapObject::getOnetimeTrigger("OudinIntro",List<mapFloor::triangle>({mapFloor::triangle({{57.646697759628296,83.61799120903015}, {42.36768186092377,77.54095792770386}, {42.36768186092377,83.61799120903015}}),mapFloor::triangle({{57.646697759628296,83.61799120903015}, {57.646697759628296,77.54092216491699}, {42.36768186092377,77.54095792770386}}),}), ""),
			mapObject::getOnetimeTrigger("OudinCutsceneThroughGapInWall",List<mapFloor::triangle>({mapFloor::triangle({{68.96175146102905,26.974374055862427}, {71.23281955718994,23.889872431755066}, {68.96175146102905,23.403987288475037}}),mapFloor::triangle({{68.96175146102905,26.974374055862427}, {71.23281955718994,27.752569317817688}, {71.23281955718994,23.889872431755066}}),}), ""),
			mapObject::getOnetimeTrigger("BloodWallIntro", List<mapFloor::triangle>({mapFloor::triangle({{79.49051856994629,85.2561891078949}, {20.52386701107025,52.065712213516235}, {20.52386701107025,85.2561891078949}}),mapFloor::triangle({{79.49051856994629,85.2561891078949}, {79.49051856994629,52.06562876701355}, {20.52386701107025,52.065712213516235}}),}), "SawOudinThroughGap"),
			mapObject("Blood Wall", true, true, false, imageLookup.getSequenceAsString("Blood Wall","STAND_FRONT"),"1",300,imageLookup.layerDefaults["PLAYER"],"1.0","2.0","CENTRE",{50, 55}, false, List<mapFloor::triangle>({mapFloor::triangle({{52.672046422958374,58.969444036483765}, {47.34233319759369,55.96951246261597}, {47.34233319759369,58.969444036483765}}),mapFloor::triangle({{52.672046422958374,58.969444036483765}, {52.672046422958374,55.96950650215149}, {47.34233319759369,55.96951246261597}}),}), Map<string, string>({
																			pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Inspect the Blood Wall"}),
																			pair<string, string>({"copy","Blood Wall"}),
																			pair<string, string>({"cutscene","Blood Wall"}),
																			pair<string, string>({"x","5"}),
																			pair<string, string>({"y","0"}),
																			pair<string, string>({"h","50"}),
																			pair<string, string>({"w","50"}),
																			pair<string, string>("colour", "WHITE"),
																			pair<string, string>("uniqueID", mapPopupTextID),
																			pair<string, string>("anchorStyle", "TOPLEFT"),
																			pair<string, string>("shadowColour", "BLACK"),
																			pair<string, string>("don'tLoadIfNot", "SawOudinThroughGap"),
																			pair<string, string>("don'tLoadIf", "BloodWallDestroyed"),
																			pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																			pair<string, string>({"format","LightText_20"})})),
												mapObject("OUDIN", true, true, false, imageLookup.getSequenceAsString("OUDIN","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{49.5, 69}, false, List<mapFloor::triangle>({mapFloor::triangle({{51.356154680252075,72.34838604927063}, {47.68877923488617,68.95595192909241}, {47.68877923488617,72.34838604927063}}),mapFloor::triangle({{51.356154680252075,72.34838604927063}, {51.356154680252075,68.9559280872345}, {47.68877923488617,68.95595192909241}}),}), Map<string, string>({
																			pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Talk to OUDIN"}),
																			pair<string, string>({"copy","OUDIN"}),
																			pair<string, string>({"cutscene","Oudin1"}),
																			pair<string, string>({"x","5"}),
																			pair<string, string>({"y","0"}),
																			pair<string, string>({"h","50"}),
																			pair<string, string>({"w","50"}),
																			pair<string, string>("colour", "WHITE"),
																			pair<string, string>("uniqueID", mapPopupTextID),
																			pair<string, string>("anchorStyle", "TOPLEFT"),
																			pair<string, string>("shadowColour", "BLACK"),
																			pair<string, string>("don'tLoadIf", "OudinHostileTriggered"),
																			pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																			pair<string, string>({"format","LightText_20"})})),
													mapObject("Shadow OUDIN", false, true, false, imageLookup.getSequenceAsString("Shadow OUDIN","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{49.5, 69}, false, {},Map<string, string>({
														pair<string, string>("don'tLoadIf", "OudinHostileTriggered")
													})),
												mapObject("BrazierFire", false, true, false, imageLookup.getSequenceAsString("BrazierFire","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"] + 2,"1.0","1.5","CENTRE",{33.4, 75.3}, false, {}, {}),
												mapObject("BrazierFire2", false, true, false, imageLookup.getSequenceAsString("BrazierFire","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"] + 2,"1.0","1.5","CENTRE",{37.2, 75.3}, false, {}, {}),
												mapObject("BrazierFire3", false, true, false, imageLookup.getSequenceAsString("BrazierFire","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"] + 2,"1.0","1.5","CENTRE",{41, 75.3}, false, {}, {}),
												mapObject("BrazierFire4", false, true, false, imageLookup.getSequenceAsString("BrazierFire","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"] + 2,"1.0","1.5","CENTRE",{59.2, 75.3}, false, {}, {}),
												mapObject("BrazierFire5", false, true, false, imageLookup.getSequenceAsString("BrazierFire","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"] + 2,"1.0","1.5","CENTRE",{63.1, 75.3}, false, {}, {}),
												mapObject("BrazierFire6", false, true, false, imageLookup.getSequenceAsString("BrazierFire","STAND_FRONT"),"1",20,imageLookup.layerDefaults["PLAYER"] + 2,"1.0","1.5","CENTRE",{66.7, 75.3}, false, {}, {}),
												mapObject("ChapelEntryTopLayer1", false, true, false, imageLookup.getSequenceAsString("ChapelEntryTopLayer1","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","1.5","CENTRE",{50, 50}, false, {}, {}),
												mapObject("ChapelEntryCandles", false, true, false, imageLookup.getSequenceAsString("ChapelEntryCandles","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"] + 2,"1.0","1.5","CENTRE",{50, 50}, false, {}, {}),
												mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
											mapObject("AT_Door", true, false, false, "","0",0,0,"1","1.0","CENTRE",{50, 83}, false, List<mapFloor::triangle>({mapFloor::triangle({{53.95346283912659,84.48187708854675}, {46.037787199020386,83.33396911621094}, {46.037787199020386,84.48187708854675}}),mapFloor::triangle({{53.95346283912659,84.48187708854675}, {53.95346283912659,83.33396911621094}, {46.037787199020386,83.33396911621094}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave Chapel"}),
												pair<string, string>({"copy","AT_Door"}),
												pair<string, string>({"areaTransition","EstateOutside1"}),
												pair<string, string>({"direction","STAND_FRONT"}),
												pair<string, string>({"playerPosX","50"}),
												pair<string, string>({"playerPosY","55"}),
												pair<string, string>({"audio","CHAPELMULTISTEP"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}),})),
											mapObject("AT_Chapel2Basement", true, false, false, "","0",0,0,"1","1.0","CENTRE",{84, 27}, false, List<mapFloor::triangle>({mapFloor::triangle({{81.475830078125,28.819304704666138}, {84.30764675140381,25.476360321044922}, {81.475830078125,25.579580664634705}}),mapFloor::triangle({{81.475830078125,28.819304704666138}, {84.30764675140381,29.150956869125366}, {84.30764675140381,25.476360321044922}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Through The Gap"}),
												pair<string, string>({"copy","AT_Chapel2Basement"}),
												pair<string, string>({"areaTransition","ChapelRight2"}),
												pair<string, string>({"direction","STAND_RIGHT"}),
												pair<string, string>({"playerPosX","55"}),
												pair<string, string>({"playerPosY","64"}),
												pair<string, string>({"audio","DUSTY"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}),})),
												}), List<mapFloor>({
										mapFloor("ChapelFloor", List<mapFloor::triangle>({mapFloor::triangle({{49.05902445316315,82.12100863456726}, {45.61507701873779,84.49022173881531}, {49.05902445316315,84.49022173881531}}),mapFloor::triangle({{43.81645321846008,81.9620132446289}, {45.61507701873779,84.49022173881531}, {45.61507701873779,81.56251907348633}}),mapFloor::triangle({{49.05902445316315,67.48740673065186}, {45.61507701873779,66.72072410583496}, {45.61507701873779,67.48740673065186}}),
mapFloor::triangle({{45.61507701873779,81.56251907348633}, {49.05902445316315,78.22794914245605}, {45.61507701873779,78.22794914245605}}),mapFloor::triangle({{49.05902445316315,78.22794914245605}, {45.61507701873779,76.37494802474976}, {45.61507701873779,78.22794914245605}}),mapFloor::triangle({{32.23850727081299,66.72072410583496}, {31.368547677993774,75.08246898651123}, {32.23850727081299,75.08246898651123}}),
mapFloor::triangle({{45.61507701873779,66.72072410583496}, {44.45151090621948,67.45651960372925}, {45.61507701873779,67.48740673065186}}),mapFloor::triangle({{37.002912163734436,74.11537170410156}, {32.23850727081299,66.72072410583496}, {32.23850727081299,75.08246898651123}}),mapFloor::triangle({{37.002912163734436,66.72072410583496}, {32.23850727081299,66.29634499549866}, {32.23850727081299,66.72072410583496}}),
mapFloor::triangle({{28.287193179130554,54.26681637763977}, {24.82398748397827,56.846749782562256}, {26.21188759803772,57.73525834083557}}),mapFloor::triangle({{32.23850727081299,66.29634499549866}, {36.76255941390991,64.32026028633118}, {31.534308195114136,64.55913186073303}}),mapFloor::triangle({{35.0428581237793,60.71818470954895}, {31.534308195114136,64.55913186073303}, {36.76255941390991,64.32026028633118}}),
mapFloor::triangle({{28.287193179130554,54.26681637763977}, {28.26991081237793,59.6733033657074}, {32.02490508556366,56.98847770690918}}),mapFloor::triangle({{32.02490508556366,56.98847770690918}, {29.885074496269226,61.77237033843994}, {35.0428581237793,60.71818470954895}}),mapFloor::triangle({{45.167478919029236,62.88251876831055}, {48.042574524879456,57.87727236747742}, {44.5986270904541,58.00451040267944}}),
mapFloor::triangle({{45.167478919029236,62.88251876831055}, {42.964327335357666,58.909064531326294}, {43.79924535751343,63.28083276748657}}),mapFloor::triangle({{45.61507701873779,66.72072410583496}, {43.79924535751343,63.28083276748657}, {44.45619583129883,66.72072410583496}}),mapFloor::triangle({{45.61507701873779,66.72072410583496}, {48.61142635345459,62.82649040222168}, {45.167478919029236,62.88251876831055}}),
mapFloor::triangle({{49.05902445316315,84.49022173881531}, {49.995625019073486,82.12100863456726}, {49.05902445316315,82.12100863456726}}),mapFloor::triangle({{48.61142635345459,62.82649040222168}, {49.995625019073486,57.87012577056885}, {48.042574524879456,57.87727236747742}}),mapFloor::triangle({{49.05902445316315,78.22794914245605}, {49.995625019073486,75.5698561668396}, {49.05902445316315,75.5698561668396}}),
mapFloor::triangle({{49.05902445316315,82.12100863456726}, {49.995625019073486,78.22794914245605}, {49.05902445316315,78.22794914245605}}),mapFloor::triangle({{49.05902445316315,75.5698561668396}, {49.995625019073486,67.48740673065186}, {49.05902445316315,67.48740673065186}}),mapFloor::triangle({{49.05902445316315,66.72072410583496}, {49.995625019073486,62.82649040222168}, {48.61142635345459,62.82649040222168}}),
mapFloor::triangle({{50.93224048614502,82.12100863456726}, {54.376184940338135,84.49022173881531}, {50.93224048614502,84.49022173881531}}),mapFloor::triangle({{56.17479681968689,81.9620132446289}, {54.376184940338135,84.49022173881531}, {54.376184940338135,81.56251907348633}}),mapFloor::triangle({{50.93224048614502,67.48740673065186}, {54.376184940338135,66.72072410583496}, {54.376184940338135,67.48740673065186}}),
mapFloor::triangle({{54.376184940338135,81.56251907348633}, {50.93224048614502,78.22794914245605}, {54.376184940338135,78.22794914245605}}),mapFloor::triangle({{50.93224048614502,78.22794914245605}, {54.376184940338135,76.37494802474976}, {54.376184940338135,78.22794914245605}}),mapFloor::triangle({{67.75274872779846,66.72072410583496}, {68.62270832061768,67.48740673065186}, {67.75274872779846,67.48740673065186}}),
mapFloor::triangle({{62.988340854644775,66.72072410583496}, {67.75274872779846,67.48740673065186}, {62.988340854644775,67.48740673065186}}),mapFloor::triangle({{62.988340854644775,66.72072410583496}, {67.75274872779846,66.29634499549866}, {67.75274872779846,66.72072410583496}}),mapFloor::triangle({{71.70406579971313,54.26681637763977}, {75.16726851463318,56.846749782562256}, {73.77936244010925,57.73525834083557}}),
mapFloor::triangle({{67.75274872779846,66.29634499549866}, {63.22869658470154,64.32026028633118}, {68.45694184303284,64.55913186073303}}),mapFloor::triangle({{64.94839191436768,60.71818470954895}, {68.45694184303284,64.55913186073303}, {63.22869658470154,64.32026028633118}}),mapFloor::triangle({{71.70406579971313,54.26681637763977}, {71.72134518623352,59.6733033657074}, {67.96635389328003,56.98847770690918}}),
mapFloor::triangle({{67.96635389328003,56.98847770690918}, {70.10618448257446,61.77237033843994}, {64.94839191436768,60.71818470954895}}),mapFloor::triangle({{54.82378005981445,62.88251876831055}, {51.948678493499756,57.87727236747742}, {55.39262294769287,58.00451040267944}}),mapFloor::triangle({{54.82378005981445,62.88251876831055}, {57.02692270278931,58.909064531326294}, {56.19201064109802,63.28083276748657}}),
mapFloor::triangle({{54.376184940338135,66.72072410583496}, {56.19201064109802,63.28083276748657}, {55.53506016731262,66.72072410583496}}),mapFloor::triangle({{54.376184940338135,66.72072410583496}, {51.37982964515686,62.82649040222168}, {54.82378005981445,62.88251876831055}}),mapFloor::triangle({{50.93224048614502,84.49022173881531}, {49.995625019073486,82.12100863456726}, {50.93224048614502,82.12100863456726}}),
mapFloor::triangle({{51.37982964515686,62.82649040222168}, {49.995625019073486,57.87012577056885}, {51.948678493499756,57.87727236747742}}),mapFloor::triangle({{50.93224048614502,78.22794914245605}, {49.995625019073486,75.5698561668396}, {50.93224048614502,75.5698561668396}}),mapFloor::triangle({{50.93224048614502,82.12100863456726}, {49.995625019073486,78.22794914245605}, {50.93224048614502,78.22794914245605}}),
mapFloor::triangle({{50.93224048614502,75.5698561668396}, {49.995625019073486,67.48740673065186}, {50.93224048614502,67.48740673065186}}),mapFloor::triangle({{50.93224048614502,66.72072410583496}, {49.995625019073486,62.82649040222168}, {51.37982964515686,62.82649040222168}}),mapFloor::triangle({{38.16927373409271,67.45674014091492}, {43.17518472671509,66.72072410583496}, {38.16927373409271,66.72072410583496}}),
mapFloor::triangle({{43.17518472671509,75.08246898651123}, {43.745025992393494,67.45651960372925}, {43.17518472671509,67.46883392333984}}),mapFloor::triangle({{38.16927373409271,74.11537170410156}, {43.17518472671509,67.46883392333984}, {38.16927373409271,67.45674014091492}}),mapFloor::triangle({{38.16927373409271,67.45674014091492}, {37.002912163734436,74.11537170410156}, {38.16927373409271,74.11537170410156}}),
mapFloor::triangle({{44.39351260662079,75.5698561668396}, {43.745025992393494,67.45651960372925}, {43.745025992393494,75.5698561668396}}),mapFloor::triangle({{61.84046268463135,67.48740673065186}, {62.988340854644775,74.15121793746948}, {61.84046268463135,74.15121793746948}}),mapFloor::triangle({{61.84046268463135,74.15121793746948}, {56.80861473083496,67.48740673065186}, {61.84046268463135,67.48740673065186}}),
mapFloor::triangle({{61.84046268463135,67.48740673065186}, {56.80861473083496,66.72072410583496}, {61.84046268463135,66.72072410583496}}),mapFloor::triangle({{50.93224048614502,67.48740673065186}, {49.995625019073486,66.72072410583496}, {50.93224048614502,66.72072410583496}}),mapFloor::triangle({{62.988340854644775,74.15121793746948}, {67.75274872779846,67.48740673065186}, {67.75274872779846,75.08246898651123}}),
mapFloor::triangle({{55.59239983558655,67.48740673065186}, {56.80861473083496,75.08246898651123}, {55.59239983558655,75.5698561668396}}),mapFloor::triangle({{67.75274872779846,67.48740673065186}, {68.62270832061768,75.08246898651123}, {67.75274872779846,75.08246898651123}}),mapFloor::triangle({{50.93224048614502,75.5698561668396}, {54.376184940338135,67.48740673065186}, {54.376184940338135,76.37494802474976}}),
mapFloor::triangle({{49.05902445316315,67.48740673065186}, {49.995625019073486,66.72072410583496}, {49.05902445316315,66.72072410583496}}),mapFloor::triangle({{45.61507701873779,67.48740673065186}, {44.39351260662079,75.5698561668396}, {45.61507701873779,76.37494802474976}}),mapFloor::triangle({{49.05902445316315,75.5698561668396}, {45.61507701873779,67.48740673065186}, {45.61507701873779,76.37494802474976}}),
mapFloor::triangle({{54.376184940338135,67.48740673065186}, {55.59239983558655,75.5698561668396}, {54.376184940338135,76.37494802474976}}),mapFloor::triangle({{54.376184940338135,67.48740673065186}, {55.53506016731262,66.72072410583496}, {55.59239983558655,67.48740673065186}}),mapFloor::triangle({{43.17518472671509,75.08246898651123}, {38.16927373409271,74.11537170410156}, {39.64410126209259,75.23702383041382}}),
mapFloor::triangle({{37.002912163734436,74.11537170410156}, {32.23850727081299,75.08246898651123}, {35.52808463573456,75.01250505447388}}),mapFloor::triangle({{56.80861473083496,75.08246898651123}, {61.84046268463135,74.15121793746948}, {60.0796103477478,75.0478982925415}}),mapFloor::triangle({{67.75274872779846,75.08246898651123}, {64.74919319152832,75.0478982925415}, {62.988340854644775,74.15121793746948}}),
mapFloor::triangle({{57.424843311309814,44.845110177993774}, {50.0607430934906,33.53111445903778}, {50.0607430934906,44.845110177993774}}),mapFloor::triangle({{52.67741680145264,34.739333391189575}, {47.38111197948456,22.175197303295135}, {47.38111197948456,34.739333391189575}}),mapFloor::triangle({{33.49521458148956,34.00563597679138}, {40.737536549568176,18.81553828716278}, {33.495211601257324,18.81553828716278}}),
mapFloor::triangle({{41.4729118347168,20.00558078289032}, {33.495211601257324,16.936278343200684}, {33.495211601257324,20.00558078289032}}),mapFloor::triangle({{58.924174308776855,32.99248218536377}, {66.57431721687317,18.8155397772789}, {58.92416834831238,18.8155397772789}}),mapFloor::triangle({{66.33628010749817,20.00558227300644}, {58.358579874038696,16.936279833316803}, {58.358579874038696,20.00558227300644}}),
mapFloor::triangle({{39.87639844417572,23.18582981824875}, {47.76158928871155,22.022084891796112}, {47.76158928871155,23.18582981824875}}),mapFloor::triangle({{56.115370988845825,18.92993450164795}, {47.02724814414978,20.06998062133789}, {45.5565482378006,18.68044286966324}}),mapFloor::triangle({{58.06261897087097,22.282661497592926}, {56.115370988845825,20.442597568035126}, {58.06261897087097,20.442597568035126}}),
mapFloor::triangle({{58.06261897087097,20.442597568035126}, {56.115370988845825,18.92993450164795}, {58.06261897087097,18.884071707725525}}),mapFloor::triangle({{56.115370988845825,18.92993450164795}, {52.2092342376709,16.46430790424347}, {56.115370988845825,16.418446600437164}}),mapFloor::triangle({{47.77179956436157,18.905532360076904}, {43.865662813186646,16.387002170085907}, {47.77179956436157,16.33257120847702}}),
mapFloor::triangle({{45.72237730026245,22.417402267456055}, {45.5565482378006,21.540680527687073}, {47.02724814414978,20.06998062133789}}),mapFloor::triangle({{44.085848331451416,20.06998062133789}, {40.07319211959839,18.975795805454254}, {45.5565482378006,18.68044286966324}}),mapFloor::triangle({{33.49521458148956,35.63179075717926}, {40.737539529800415,34.00563597679138}, {33.49521458148956,34.00563597679138}}),
mapFloor::triangle({{42.45138168334961,35.63179075717926}, {45.00461220741272,34.88732576370239}, {40.737539529800415,34.00563597679138}}),mapFloor::triangle({{50.0607430934906,44.845110177993774}, {42.69664287567139,34.30318832397461}, {42.69664287567139,44.845110177993774}}),mapFloor::triangle({{58.924174308776855,32.99248218536377}, {66.57431721687317,35.43517589569092}, {66.57431721687317,32.99248218536377}}),
mapFloor::triangle({{58.924174308776855,35.43517589569092}, {55.20962476730347,33.278149366378784}, {55.20962476730347,35.72084307670593}}),mapFloor::triangle({{68.96175146102905,26.974374055862427}, {71.23281955718994,23.889872431755066}, {68.96175146102905,23.403987288475037}}),mapFloor::triangle({{71.23281955718994,27.752569317817688}, {74.1666853427887,25.101536512374878}, {71.23281955718994,23.889872431755066}}),
mapFloor::triangle({{74.1666853427887,28.722018003463745}, {77.87865996360779,25.603997707366943}, {74.1666853427887,25.101536512374878}}),mapFloor::triangle({{77.87865996360779,25.603997707366943}, {81.475830078125,28.819304704666138}, {81.475830078125,25.579580664634705}}),mapFloor::triangle({{81.475830078125,28.819304704666138}, {84.30764675140381,25.476360321044922}, {81.475830078125,25.579580664634705}}),
mapFloor::triangle({{44.085848331451416,20.06998062133789}, {45.72237730026245,22.417402267456055}, {40.07319211959839,22.417402267456055}}),mapFloor::triangle({{49.05902445316315,82.12100863456726}, {45.61507701873779,81.56251907348633}, {45.61507701873779,84.49022173881531}}),mapFloor::triangle({{43.81645321846008,81.9620132446289}, {43.81645321846008,84.49022173881531}, {45.61507701873779,84.49022173881531}}),
mapFloor::triangle({{49.05902445316315,67.48740673065186}, {49.05902445316315,66.72072410583496}, {45.61507701873779,66.72072410583496}}),mapFloor::triangle({{45.61507701873779,81.56251907348633}, {49.05902445316315,82.12100863456726}, {49.05902445316315,78.22794914245605}}),mapFloor::triangle({{49.05902445316315,78.22794914245605}, {49.05902445316315,75.5698561668396}, {45.61507701873779,76.37494802474976}}),
mapFloor::triangle({{32.23850727081299,66.72072410583496}, {31.368547677993774,66.72072410583496}, {31.368547677993774,75.08246898651123}}),mapFloor::triangle({{45.61507701873779,66.72072410583496}, {44.45619583129883,66.72072410583496}, {44.45151090621948,67.45651960372925}}),mapFloor::triangle({{37.002912163734436,74.11537170410156}, {37.002912163734436,66.72072410583496}, {32.23850727081299,66.72072410583496}}),
mapFloor::triangle({{37.002912163734436,66.72072410583496}, {37.002912163734436,66.05746746063232}, {32.23850727081299,66.29634499549866}}),mapFloor::triangle({{28.287193179130554,54.26681637763977}, {26.088088750839233,53.41002345085144}, {24.82398748397827,56.846749782562256}}),mapFloor::triangle({{32.23850727081299,66.29634499549866}, {37.002912163734436,66.05746746063232}, {36.76255941390991,64.32026028633118}}),
mapFloor::triangle({{35.0428581237793,60.71818470954895}, {29.885074496269226,61.77237033843994}, {31.534308195114136,64.55913186073303}}),mapFloor::triangle({{28.287193179130554,54.26681637763977}, {26.21188759803772,57.73525834083557}, {28.26991081237793,59.6733033657074}}),mapFloor::triangle({{32.02490508556366,56.98847770690918}, {28.26991081237793,59.6733033657074}, {29.885074496269226,61.77237033843994}}),
mapFloor::triangle({{45.167478919029236,62.88251876831055}, {48.61142635345459,62.82649040222168}, {48.042574524879456,57.87727236747742}}),mapFloor::triangle({{45.167478919029236,62.88251876831055}, {44.5986270904541,58.00451040267944}, {42.964327335357666,58.909064531326294}}),mapFloor::triangle({{45.61507701873779,66.72072410583496}, {45.167478919029236,62.88251876831055}, {43.79924535751343,63.28083276748657}}),
mapFloor::triangle({{45.61507701873779,66.72072410583496}, {49.05902445316315,66.72072410583496}, {48.61142635345459,62.82649040222168}}),mapFloor::triangle({{49.05902445316315,84.49022173881531}, {49.995625019073486,84.49022173881531}, {49.995625019073486,82.12100863456726}}),mapFloor::triangle({{48.61142635345459,62.82649040222168}, {49.995625019073486,62.82649040222168}, {49.995625019073486,57.87012577056885}}),
mapFloor::triangle({{49.05902445316315,78.22794914245605}, {49.995625019073486,78.22794914245605}, {49.995625019073486,75.5698561668396}}),mapFloor::triangle({{49.05902445316315,82.12100863456726}, {49.995625019073486,82.12100863456726}, {49.995625019073486,78.22794914245605}}),mapFloor::triangle({{49.05902445316315,75.5698561668396}, {49.995625019073486,75.5698561668396}, {49.995625019073486,67.48740673065186}}),
mapFloor::triangle({{49.05902445316315,66.72072410583496}, {49.995625019073486,66.72072410583496}, {49.995625019073486,62.82649040222168}}),mapFloor::triangle({{50.93224048614502,82.12100863456726}, {54.376184940338135,81.56251907348633}, {54.376184940338135,84.49022173881531}}),mapFloor::triangle({{56.17479681968689,81.9620132446289}, {56.17479681968689,84.49022173881531}, {54.376184940338135,84.49022173881531}}),
mapFloor::triangle({{50.93224048614502,67.48740673065186}, {50.93224048614502,66.72072410583496}, {54.376184940338135,66.72072410583496}}),mapFloor::triangle({{54.376184940338135,81.56251907348633}, {50.93224048614502,82.12100863456726}, {50.93224048614502,78.22794914245605}}),mapFloor::triangle({{50.93224048614502,78.22794914245605}, {50.93224048614502,75.5698561668396}, {54.376184940338135,76.37494802474976}}),
mapFloor::triangle({{67.75274872779846,66.72072410583496}, {68.62270832061768,66.72072410583496}, {68.62270832061768,67.48740673065186}}),mapFloor::triangle({{62.988340854644775,66.72072410583496}, {67.75274872779846,66.72072410583496}, {67.75274872779846,67.48740673065186}}),mapFloor::triangle({{62.988340854644775,66.72072410583496}, {62.988340854644775,66.05746746063232}, {67.75274872779846,66.29634499549866}}),
mapFloor::triangle({{71.70406579971313,54.26681637763977}, {73.90316724777222,53.41002345085144}, {75.16726851463318,56.846749782562256}}),mapFloor::triangle({{67.75274872779846,66.29634499549866}, {62.988340854644775,66.05746746063232}, {63.22869658470154,64.32026028633118}}),mapFloor::triangle({{64.94839191436768,60.71818470954895}, {70.10618448257446,61.77237033843994}, {68.45694184303284,64.55913186073303}}),
mapFloor::triangle({{71.70406579971313,54.26681637763977}, {73.77936244010925,57.73525834083557}, {71.72134518623352,59.6733033657074}}),mapFloor::triangle({{67.96635389328003,56.98847770690918}, {71.72134518623352,59.6733033657074}, {70.10618448257446,61.77237033843994}}),mapFloor::triangle({{54.82378005981445,62.88251876831055}, {51.37982964515686,62.82649040222168}, {51.948678493499756,57.87727236747742}}),
mapFloor::triangle({{54.82378005981445,62.88251876831055}, {55.39262294769287,58.00451040267944}, {57.02692270278931,58.909064531326294}}),mapFloor::triangle({{54.376184940338135,66.72072410583496}, {54.82378005981445,62.88251876831055}, {56.19201064109802,63.28083276748657}}),mapFloor::triangle({{54.376184940338135,66.72072410583496}, {50.93224048614502,66.72072410583496}, {51.37982964515686,62.82649040222168}}),
mapFloor::triangle({{50.93224048614502,84.49022173881531}, {49.995625019073486,84.49022173881531}, {49.995625019073486,82.12100863456726}}),mapFloor::triangle({{51.37982964515686,62.82649040222168}, {49.995625019073486,62.82649040222168}, {49.995625019073486,57.87012577056885}}),mapFloor::triangle({{50.93224048614502,78.22794914245605}, {49.995625019073486,78.22794914245605}, {49.995625019073486,75.5698561668396}}),
mapFloor::triangle({{50.93224048614502,82.12100863456726}, {49.995625019073486,82.12100863456726}, {49.995625019073486,78.22794914245605}}),mapFloor::triangle({{50.93224048614502,75.5698561668396}, {49.995625019073486,75.5698561668396}, {49.995625019073486,67.48740673065186}}),mapFloor::triangle({{50.93224048614502,66.72072410583496}, {49.995625019073486,66.72072410583496}, {49.995625019073486,62.82649040222168}}),
mapFloor::triangle({{38.16927373409271,67.45674014091492}, {43.17518472671509,67.46883392333984}, {43.17518472671509,66.72072410583496}}),mapFloor::triangle({{43.17518472671509,75.08246898651123}, {43.745025992393494,75.5698561668396}, {43.745025992393494,67.45651960372925}}),mapFloor::triangle({{38.16927373409271,74.11537170410156}, {43.17518472671509,75.08246898651123}, {43.17518472671509,67.46883392333984}}),
mapFloor::triangle({{38.16927373409271,67.45674014091492}, {37.002912163734436,67.45674014091492}, {37.002912163734436,74.11537170410156}}),mapFloor::triangle({{44.39351260662079,75.5698561668396}, {44.45151090621948,67.45651960372925}, {43.745025992393494,67.45651960372925}}),mapFloor::triangle({{61.84046268463135,67.48740673065186}, {62.988340854644775,67.48740673065186}, {62.988340854644775,74.15121793746948}}),
mapFloor::triangle({{61.84046268463135,74.15121793746948}, {56.80861473083496,75.08246898651123}, {56.80861473083496,67.48740673065186}}),mapFloor::triangle({{61.84046268463135,67.48740673065186}, {56.80861473083496,67.48740673065186}, {56.80861473083496,66.72072410583496}}),mapFloor::triangle({{50.93224048614502,67.48740673065186}, {49.995625019073486,67.48740673065186}, {49.995625019073486,66.72072410583496}}),
mapFloor::triangle({{62.988340854644775,74.15121793746948}, {62.988340854644775,67.48740673065186}, {67.75274872779846,67.48740673065186}}),mapFloor::triangle({{55.59239983558655,67.48740673065186}, {56.80861473083496,67.48740673065186}, {56.80861473083496,75.08246898651123}}),mapFloor::triangle({{67.75274872779846,67.48740673065186}, {68.62270832061768,67.48740673065186}, {68.62270832061768,75.08246898651123}}),
mapFloor::triangle({{50.93224048614502,75.5698561668396}, {50.93224048614502,67.48740673065186}, {54.376184940338135,67.48740673065186}}),mapFloor::triangle({{49.05902445316315,67.48740673065186}, {49.995625019073486,67.48740673065186}, {49.995625019073486,66.72072410583496}}),mapFloor::triangle({{45.61507701873779,67.48740673065186}, {44.45151090621948,67.45651960372925}, {44.39351260662079,75.5698561668396}}),
mapFloor::triangle({{49.05902445316315,75.5698561668396}, {49.05902445316315,67.48740673065186}, {45.61507701873779,67.48740673065186}}),mapFloor::triangle({{54.376184940338135,67.48740673065186}, {55.59239983558655,67.48740673065186}, {55.59239983558655,75.5698561668396}}),mapFloor::triangle({{54.376184940338135,67.48740673065186}, {54.376184940338135,66.72072410583496}, {55.53506016731262,66.72072410583496}}),
mapFloor::triangle({{57.424843311309814,44.845110177993774}, {57.424843311309814,33.126845955848694}, {50.0607430934906,33.53111445903778}}),mapFloor::triangle({{52.67741680145264,34.739333391189575}, {52.67741680145264,22.175197303295135}, {47.38111197948456,22.175197303295135}}),mapFloor::triangle({{33.49521458148956,34.00563597679138}, {40.737539529800415,34.00563597679138}, {40.737536549568176,18.81553828716278}}),
mapFloor::triangle({{41.4729118347168,20.00558078289032}, {41.4729118347168,16.936278343200684}, {33.495211601257324,16.936278343200684}}),mapFloor::triangle({{58.924174308776855,32.99248218536377}, {66.57431721687317,32.99248218536377}, {66.57431721687317,18.8155397772789}}),mapFloor::triangle({{66.33628010749817,20.00558227300644}, {66.33628010749817,16.936279833316803}, {58.358579874038696,16.936279833316803}}),
mapFloor::triangle({{39.87639844417572,23.18582981824875}, {39.87639844417572,22.022084891796112}, {47.76158928871155,22.022084891796112}}),mapFloor::triangle({{56.115370988845825,18.92993450164795}, {56.115370988845825,20.442597568035126}, {47.02724814414978,20.06998062133789}}),mapFloor::triangle({{58.06261897087097,22.282661497592926}, {56.115370988845825,22.726435959339142}, {56.115370988845825,20.442597568035126}}),
mapFloor::triangle({{58.06261897087097,20.442597568035126}, {56.115370988845825,20.442597568035126}, {56.115370988845825,18.92993450164795}}),mapFloor::triangle({{56.115370988845825,18.92993450164795}, {52.2092342376709,18.975795805454254}, {52.2092342376709,16.46430790424347}}),mapFloor::triangle({{47.77179956436157,18.905532360076904}, {43.865662813186646,18.95996332168579}, {43.865662813186646,16.387002170085907}}),
mapFloor::triangle({{56.115370988845825,20.442597568035126}, {56.115370988845825,22.726435959339142}, {47.02724814414978,20.06998062133789}}),mapFloor::triangle({{56.115370988845825,22.726435959339142}, {45.72237730026245,22.417402267456055}, {47.02724814414978,20.06998062133789}}),mapFloor::triangle({{44.085848331451416,20.06998062133789}, {40.07319211959839,20.442597568035126}, {40.07319211959839,18.975795805454254}}),
mapFloor::triangle({{33.49521458148956,35.63179075717926}, {42.45138168334961,35.63179075717926}, {40.737539529800415,34.00563597679138}}),mapFloor::triangle({{42.45138168334961,35.63179075717926}, {45.00461220741272,36.51348054409027}, {45.00461220741272,34.88732576370239}}),mapFloor::triangle({{50.0607430934906,44.845110177993774}, {50.0607430934906,33.53111445903778}, {42.69664287567139,34.30318832397461}}),
mapFloor::triangle({{58.924174308776855,32.99248218536377}, {58.924174308776855,35.43517589569092}, {66.57431721687317,35.43517589569092}}),mapFloor::triangle({{58.924174308776855,35.43517589569092}, {58.924174308776855,32.99248218536377}, {55.20962476730347,33.278149366378784}}),mapFloor::triangle({{68.96175146102905,26.974374055862427}, {71.23281955718994,27.752569317817688}, {71.23281955718994,23.889872431755066}}),
mapFloor::triangle({{71.23281955718994,27.752569317817688}, {74.1666853427887,28.722018003463745}, {74.1666853427887,25.101536512374878}}),mapFloor::triangle({{74.1666853427887,28.722018003463745}, {77.87865996360779,28.972378373146057}, {77.87865996360779,25.603997707366943}}),mapFloor::triangle({{77.87865996360779,25.603997707366943}, {77.87865996360779,28.972378373146057}, {81.475830078125,28.819304704666138}}),
mapFloor::triangle({{81.475830078125,28.819304704666138}, {84.30764675140381,29.150956869125366}, {84.30764675140381,25.476360321044922}}),mapFloor::triangle({{40.07319211959839,22.417402267456055}, {40.07319211959839,20.442597568035126}, {44.085848331451416,20.06998062133789}}),mapFloor::triangle({{44.085848331451416,20.06998062133789}, {45.5565482378006,21.540680527687073}, {45.72237730026245,22.417402267456055}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "CHAPELSTEP"})})),
													}), {}, { 5000,5000 }, { Map<string, string>({
											pair<string,string>({"song1",to_string(CHAPEL_WAV_1) + " " + "MusicVolume"}),
											pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
											}) });
		maps["ChapelRight1"] = mapInstance("ChapelRight1", CHAPELRIGHT_1, { 70,15 /*2.5 42 */}, puzzleContainer.getWaterPuzzle1({50,35}) + List<mapObject>({
			mapObject::getOnetimeTrigger("ChapelRightCorridor+PAIR",List<mapFloor::triangle>({mapFloor::triangle({{43.884652853012085,83.46079587936401}, {15.895740687847137,91.99681282043457}, {43.884652853012085,91.99681878089905}}),mapFloor::triangle({{43.884652853012085,83.46079587936401}, {15.895740687847137,83.46079587936401}, {15.895740687847137,91.99681282043457}}),}), ""),
			mapObject("RightWingStatue", true, false, false, "","0",0,0,"","","CENTRE",{56, 7}, false, List<mapFloor::triangle>({{mapFloor::triangle({{58.94955396652222,9.21408161520958}, {54.66281771659851,6.213817372918129}, {54.66281771659851,9.21408161520958}}),mapFloor::triangle({{58.94955396652222,9.21408161520958}, {58.94955396652222,6.213817372918129}, {54.66281771659851,6.213817372918129}}),}}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Inspect Statue"}),
																		pair<string, string>({"copy","RightWingStatue"}),
																		pair<string, string>({"cutscene","RightWingFountainCheck1"}),
																		pair<string, string>({"x","3"}),
																		pair<string, string>({"y","-5"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_GoUpstairs", true, false, false, "","0",0,0,"","","CENTRE",{70, 15}, false, List<mapFloor::triangle>({mapFloor::triangle({{68.29286813735962,12.501510977745056}, {72.39501476287842,15.081840753555298}, {72.39501476287842,12.501510977745056}}),mapFloor::triangle({{68.29286813735962,12.501510977745056}, {68.29286813735962,15.081840753555298}, {72.39501476287842,15.081840753555298}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Upstairs"}),
																		pair<string, string>({"copy","AT_GoUpstairs"}),
																		pair<string, string>({"areaTransition","ChapelRight2"}),
																		pair<string, string>({"audio","CHAPELMULTISTEP"}),
																		pair<string, string>({"playerPosX","89"}),
																		pair<string, string>({"playerPosY","6.6"}),
																		pair<string, string>({"direction","STAND_FRONT"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_DoorToCorridor", true, false, false, "","0",0,0,"","","CENTRE",{3, 17}, false, List<mapFloor::triangle>({mapFloor::triangle({{4.868385940790176,19.548895955085754}, {2.1298933774232864,18.80180388689041}, {2.1298933774232864,19.548895955085754}}),mapFloor::triangle({{4.868385940790176,19.548895955085754}, {4.868385940790176,18.80180388689041}, {2.1298933774232864,18.80180388689041}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter"}),
																		pair<string, string>({"copy","AT_DoorToCorridor"}),
																		pair<string, string>({"cutscene","RightWingDoorToCorridor"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_CorridorToMain", true, false, false, "","0",0,0,"","","CENTRE",{7, 87}, false, List<mapFloor::triangle>({mapFloor::triangle({{8.092284947633743,85.75624227523804}, {6.611502170562744,89.70136642456055}, {8.09229090809822,89.70137238502502}}),mapFloor::triangle({{8.092284947633743,85.75624227523804}, {6.611502170562744,85.75624227523804}, {6.611502170562744,89.70136642456055}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter"}),
																		pair<string, string>({"copy","AT_CorridorToMain"}),
																		pair<string, string>({"areaTransition","ChapelRight1"}),
																		pair<string, string>({"audio","CHAPELDOOR"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"playerPosX","3"}),
																		pair<string, string>({"playerPosY","19"}),
																		pair<string, string>({"direction","STAND_FRONT"}),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("WATERDOOROPEN", true, true, false, imageLookup.getSequenceAsString("WATERDOOR","OPEN_FRONT"),"1",45,imageLookup.layerDefaults["PLAYER"] -1,"1.0","2.0","CENTRE",{67, 22.5}, false, {}, Map<string, string>({
				pair<string, string>("don'tLoadIfNot", "WaterPuzzleFinished")})),
			mapObject("WATERDOORARCHONLY", true, true, false, imageLookup.getSequenceAsString("WATERDOORARCHONLY","STAND_FRONT"),"1",45,imageLookup.layerDefaults["ENVIRONMENT"] + 1,"1.0","2.0","CENTRE",{67, 22.5}, true, {}, Map<string, string>({})),
			mapObject("WATERDOORCLOSED", true, true, false, imageLookup.getSequenceAsString("WATERDOOR","CLOSED_FRONT"),"1",45,imageLookup.layerDefaults["PLAYER"] - 1,"1.0","2.0","CENTRE",{67, 22.5}, true, List<mapFloor::triangle>({mapFloor::triangle({{67.79491901397705,24.772989749908447}, {66.10853672027588,20.65746784210205}, {65.95546007156372,23.473992943763733}}),mapFloor::triangle({{67.79491901397705,24.772989749908447}, {70.38183808326721,25.201591849327087}, {66.10853672027588,20.65746784210205}}),}), Map<string, string>({
				pair<string, string>("don'tLoadIf", "WaterPuzzleFinished")})),
			mapObject("WATERROOMOFF", false, true, false, imageLookup.getSequenceAsString("WATERROOM","OFF_FRONT"),"1",90,imageLookup.layerDefaults["MAP"] + 1,"1.0","2.0","CENTRE",{50, 50}, false, {}, Map<string, string>({
					pair<string, string>("don'tLoadIf", "WaterPuzzleActivated"),
				})),
			mapObject("WATERROOMON", false, true, false, imageLookup.getSequenceAsString("WATERROOM","ON_FRONT"),"1",90,imageLookup.layerDefaults["MAP"] + 1,"1.0","2.0","CENTRE",{50, 50}, false, {}, Map<string, string>({
					pair<string, string>("don'tLoadIfNot", "WaterPuzzleActivated"),
				})),
			mapObject::getTreasureChest("ChapelRight1Chest1", {25,5},"NORMAL2_FRONT","Tome of Cleave Armour"),
				mapObject::getTreasureChest("ChapelRight1Chest2", {86,6},"NORMAL2_FRONT","Carving of a Heptad"),
				mapObject::getTreasureChest("ChapelRight1Chest3", {98,10},"NORMAL2_FRONT","Offering to Djall"),
				mapObject::getTreasureChest("ChapelRight1Chest4", {93,45},"NORMAL2_FRONT","Token to Al-Quam"),
				mapObject::getTreasureChest("ChapelRight1Chest5", {62.5,2.5},"NORMAL2_FRONT","Tome of Backstab"),
				mapObject::getTreasureChest("ChapelRight1Chest6", {52,2.5},"NORMAL2_FRONT","Tome of Stone Strike"),
				mapObject::getTreasureChest("ChapelRight1Chest7", {21,5},"NORMAL2_FRONT","Tome of Night Fracture"),
				// chests in the secret area
				mapObject::getTreasureChest("ChapelRight1Chest8", {73,93},"NORMAL2_FRONT","Tome of Healing Rain"),
				mapObject::getTreasureChest("ChapelRight1Chest9", {78.5,93},"NORMAL2_FRONT","Tome of Revitalise"),
				mapObject::getTreasureChest("ChapelRight1Chest10", {84,93},"NORMAL2_FRONT","Tome of Thoughtful Prayer"),
			mapObject("Lamp1", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{8, 39}, false, {}, {}),
			mapObject("Lamp2", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{17, 39}, false, {}, {}),
			mapObject("Lamp3", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{6, 23}, false, {}, {}),
			mapObject("Lamp4", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{9.2, 23}, false, {}, {}),
			mapObject("Lamp5", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{1.2, 18}, false, {}, {}),
			mapObject("Lamp6", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{4.8, 18}, false, {}, {}),
			mapObject("Lamp7", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{10, 10.5}, false, {}, {}),
			mapObject("Lamp8", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{17, 10.5}, false, {}, {}),
			mapObject("Lamp9", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{21, 10}, false, {}, {}),
			mapObject("Lamp10", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{36, 6}, false, {}, {}),
			mapObject("Lamp11", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{70, 27}, false, {}, {}),
			mapObject("Lamp12", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{21, 28}, false, {}, {}),
				mapObject("Lamp13", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{86, 55}, false, {}, {}),
				mapObject("Lamp14", false, true, false, imageLookup.getSequenceAsString("ChapelFloorLamp1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{67, 12}, false, {}, {}),
			mapObject("FilmGrain2", false, true, true, imageLookup.getSequenceAsString("FilmGrain2","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("GoingUpObstruction", false, false, false, "","0",0,0,"1.0","1.0","CENTRE",{0, 0}, false, List<mapFloor::triangle>({mapFloor::triangle({{28.859907388687134,15.32842069864273}, {6.54805526137352,16.799533367156982}, {28.859907388687134,16.799533367156982}}),mapFloor::triangle({{31.548160314559937,18.09774935245514}, {21.140572428703308,19.121621549129486}, {31.548160314559937,19.121621549129486}}),mapFloor::triangle({{32.51437842845917,18.988297879695892}, {31.153520941734314,14.3336221575737}, {31.153520941734314,18.988297879695892}}),
	mapFloor::triangle({{17.191220819950104,18.410058319568634}, {9.56823006272316,19.691012799739838}, {17.191220819950104,19.691012799739838}}),mapFloor::triangle({{6.825210899114609,18.988296389579773}, {5.721496790647507,14.33362066745758}, {5.721496790647507,18.988296389579773}}),mapFloor::triangle({{17.46615171432495,20.91662883758545}, {17.188996076583862,18.172478675842285}, {17.188996076583862,20.91662883758545}}),
	mapFloor::triangle({{21.207749843597412,20.91662883758545}, {20.930594205856323,18.172478675842285}, {20.930594205856323,20.91662883758545}}),mapFloor::triangle({{17.257435619831085,21.090461313724518}, {21.202699840068817,19.89494562149048}, {17.257435619831085,19.89494562149048}}),mapFloor::triangle({{28.859907388687134,15.32842069864273}, {6.54805526137352,15.32842069864273}, {6.54805526137352,16.799533367156982}}),
	mapFloor::triangle({{31.548160314559937,18.09774935245514}, {21.140572428703308,18.09774935245514}, {21.140572428703308,19.121621549129486}}),mapFloor::triangle({{32.51437842845917,18.988297879695892}, {32.51437842845917,14.3336221575737}, {31.153520941734314,14.3336221575737}}),mapFloor::triangle({{17.191220819950104,18.410058319568634}, {9.56823006272316,18.410058319568634}, {9.56823006272316,19.691012799739838}}),
	mapFloor::triangle({{6.825210899114609,18.988296389579773}, {6.825210899114609,14.33362066745758}, {5.721496790647507,14.33362066745758}}),mapFloor::triangle({{17.46615171432495,20.91662883758545}, {17.46615171432495,18.172478675842285}, {17.188996076583862,18.172478675842285}}),mapFloor::triangle({{21.207749843597412,20.91662883758545}, {21.207749843597412,18.172478675842285}, {20.930594205856323,18.172478675842285}}),
	mapFloor::triangle({{17.257435619831085,21.090461313724518}, {21.202699840068817,21.090461313724518}, {21.202699840068817,19.89494562149048}}),}), List<pair<string,string>>({})),
			mapObject("GoingUp1", false, false, false, "","0",0,0,"1.0","1.0","CENTRE",{0, 0}, false, List<mapFloor::triangle>({mapFloor::triangle({{9.455583989620209,22.694189846515656}, {6.599082797765732,24.271899461746216}, {9.455583989620209,24.271899461746216}}),mapFloor::triangle({{31.092330813407898,12.223661690950394}, {28.89198064804077,13.46537321805954}, {31.092330813407898,13.46537321805954}}),mapFloor::triangle({{9.455583989620209,22.694189846515656}, {6.599082797765732,22.694189846515656}, {6.599082797765732,24.271899461746216}}),
				mapFloor::triangle({{31.092330813407898,12.223661690950394}, {28.89198064804077,12.223661690950394}, {28.89198064804077,13.46537321805954}}),}), List<pair<string,string>>({
					pair<string, string>({"GoingUp","1"}),
					pair<string, string>({"image","ChapelRight1Layer2"}),
					pair<string, string>({"obstruction", "GoingUpObstruction"}),
					pair<string, string>({"audioSwap", "ChapelFloorStoneOrWood=WOODCREAKY=CHAPELSTEP"}),
				})),
			mapObject("GoingUp2", false, false, false, "","0",0,0,"1.0","1.0","CENTRE",{0, 0}, false, List<mapFloor::triangle>({mapFloor::triangle({{9.455583989620209,21.232357621192932}, {6.599082797765732,22.694189846515656}, {9.455583989620209,22.694189846515656}}),mapFloor::triangle({{31.092330813407898,13.46537321805954}, {28.89198064804077,15.055710077285767}, {31.092330813407898,15.055710077285767}}),mapFloor::triangle({{9.455583989620209,21.232357621192932}, {6.599082797765732,21.232357621192932}, {6.599082797765732,22.694189846515656}}),
	mapFloor::triangle({{31.092330813407898,13.46537321805954}, {28.89198064804077,13.46537321805954}, {28.89198064804077,15.055710077285767}}),}), List<pair<string,string>>({
					pair<string, string>({"GoingUp","2"}),
					pair<string, string>({"image","ChapelRight1Layer2"}),
					pair<string, string>({"obstruction", "GoingUpObstruction"}),
					pair<string, string>({"audioSwap", "ChapelFloorStoneOrWood=WOODCREAKY=CHAPELSTEP"}),
				})),
			mapObject("ChapelRight1Top", false, true, false, imageLookup.getSequenceAsString("ChapelRight1Top","STAND_FRONT"),"0",0,imageLookup.layerDefaults["ENVIRONMENT"],"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("ChapelRight1Layer2", false, true, false, imageLookup.getSequenceAsString("ChapelRight1Layer2","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("ChapelRight1Layer3", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTLAYER3","STAND_FRONT"), "1", 45, imageLookup.layerDefaults["PLAYER"] + 2, "1.0", "2.0", "CENTRE", {50,50},false, {},{}),
			mapObject("Statue", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTSTATUE","STAND_FRONT"), "0", 0, imageLookup.layerDefaults["PLAYER"], "1.0", "2.0", "BOTTOMCENTRE", {56.92,4.2},false, List<mapFloor::triangle>(),{}),
			mapObject("Pulpit", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTPULPIT","STAND_FRONT"), "0", 0, imageLookup.layerDefaults["PLAYER"], "1.0", "2.0", "CENTRE", {86.6,14.1},false, {},{}),
			mapObject("Letter1", true, false, false, "","0",0,0,"","","CENTRE",{5, 38}, false, List<mapFloor::triangle>({mapFloor::triangle({{5.886439606547356,40.592700242996216}, {3.6629196256399155,38.5809063911438}, {3.6629196256399155,40.592700242996216}}),mapFloor::triangle({{5.886439606547356,40.592700242996216}, {5.886439606547356,38.5809063911438}, {3.6629196256399155,38.5809063911438}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Note"}),
																		pair<string, string>({"copy","Letter1"}),
																		pair<string, string>({"cutscene","RightWingLetter1"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("Letter2", true, false, false, "","0",0,0,"","","CENTRE",{11, 3}, false, List<mapFloor::triangle>({mapFloor::triangle({{14.080880582332611,6.756356358528137}, {9.695471078157425,4.553110525012016}, {9.695471078157425,6.756356358528137}}),mapFloor::triangle({{14.080880582332611,6.756356358528137}, {14.080880582332611,4.553110525012016}, {9.695471078157425,4.553110525012016}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Note"}),
																		pair<string, string>({"copy","Letter2"}),
																		pair<string, string>({"cutscene","RightWingLetter2"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("Letter3", true, false, false, "","0",0,0,"","","CENTRE",{15, 3}, false, List<mapFloor::triangle>({mapFloor::triangle({{18.565794825553894,6.756356358528137}, {14.180386066436768,4.553110897541046}, {14.180386066436768,6.756356358528137}}),mapFloor::triangle({{18.565794825553894,6.756356358528137}, {18.565794825553894,4.553110897541046}, {14.180386066436768,4.553110897541046}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Document"}),
																		pair<string, string>({"copy","Letter3"}),
																		pair<string, string>({"cutscene","RightWingLetter3"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("Letter4", true, false, false, "", "0", 0, 0, "", "", "CENTRE", { 19, 19 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{20.822320878505707,18.975870311260223}, {18.454352021217346,18.335318565368652}, {17.628709971904755,18.975870311260223}}),mapFloor::triangle({{19.77422684431076,21.89245969057083}, {17.595024406909943,19.616422057151794}, {18.6768040060997,21.89245969057083}}),mapFloor::triangle({{20.85600644350052,19.616422057151794}, {17.628709971904755,18.975870311260223}, {17.595024406909943,19.616422057151794}}),
mapFloor::triangle({{20.822320878505707,18.975870311260223}, {19.996678829193115,18.335318565368652}, {18.454352021217346,18.335318565368652}}),mapFloor::triangle({{19.77422684431076,21.89245969057083}, {20.85600644350052,19.616422057151794}, {17.595024406909943,19.616422057151794}}),mapFloor::triangle({{20.85600644350052,19.616422057151794}, {20.822320878505707,18.975870311260223}, {17.628709971904755,18.975870311260223}}), }), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Note"}),
																		pair<string, string>({"copy","Letter4"}),
																		pair<string, string>({"cutscene","RightWingLetter4"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("Letter5", true, false, false, "", "0", 0, 0, "", "", "CENTRE", { 39, 1.5 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{41.11878573894501,5.7086847722530365}, {37.61686980724335,4.212566092610359}, {37.61686980724335,5.7086847722530365}}),mapFloor::triangle({{41.11878573894501,5.7086847722530365}, {41.11878573894501,4.212566092610359}, {37.61686980724335,4.212566092610359}}) }), Map<string, string>({
																			pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Document"}),
																			pair<string, string>({"copy","Letter5"}),
																			pair<string, string>({"cutscene","RightWingLetter5"}),
																			pair<string, string>({"x","0"}),
																			pair<string, string>({"y","0"}),
																			pair<string, string>({"h","50"}),
																			pair<string, string>({"w","50"}),
																			pair<string, string>("colour", "WHITE"),
																			pair<string, string>("uniqueID", mapPopupTextID),
																			pair<string, string>("anchorStyle", "TOPLEFT"),
																			pair<string, string>("shadowColour", "BLACK"),
																			pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																			pair<string, string>({"format","LightText_20"}) })),
			mapObject("Letter6", true, false, false, "", "0", 0, 0, "", "", "CENTRE", { 43, 1.5 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{44.610267877578735,5.7086847722530365}, {41.20502471923828,4.212566092610359}, {41.20502471923828,5.7086847722530365}}),mapFloor::triangle({{44.610267877578735,5.7086847722530365}, {44.610267877578735,4.212566092610359}, {41.20502471923828,4.212566092610359}}), }), Map<string, string>({
				pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Document"}),
				pair<string, string>({"copy","Letter6"}),
				pair<string, string>({"cutscene","RightWingLetter6"}),
				pair<string, string>({"x","0"}),
				pair<string, string>({"y","0"}),
				pair<string, string>({"h","50"}),
				pair<string, string>({"w","50"}),
				pair<string, string>("colour", "WHITE"),
				pair<string, string>("uniqueID", mapPopupTextID),
				pair<string, string>("anchorStyle", "TOPLEFT"),
				pair<string, string>("shadowColour", "BLACK"),
				pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
				pair<string, string>({"format","LightText_20"}) })),
			mapObject("Letter7", true, false, false, "", "0", 0, 0, "", "", "CENTRE", { 86.2, 13.5 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{87.29946613311768,13.440898060798645}, {85.15644073486328,11.799115687608719}, {85.15644073486328,13.440898060798645}}),mapFloor::triangle({{87.29946613311768,13.440898060798645}, {87.29946613311768,11.799115687608719}, {85.15644073486328,11.799115687608719}}), }), Map<string, string>({
				pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Letter"}),
				pair<string, string>({"copy","Letter7"}),
				pair<string, string>({"cutscene","RightWingLetter7"}),
				pair<string, string>({"x","0"}),
				pair<string, string>({"y","0"}),
				pair<string, string>({"h","50"}),
				pair<string, string>({"w","50"}),
				pair<string, string>("colour", "WHITE"),
				pair<string, string>("uniqueID", mapPopupTextID),
				pair<string, string>("anchorStyle", "TOPLEFT"),
				pair<string, string>("shadowColour", "BLACK"),
				pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
				pair<string, string>({"format","LightText_20"}) })),
			mapObject("AT_ChapelMain", true, false, false, "","0",0,0,"1","1.0","CENTRE",{1, 42}, false, List<mapFloor::triangle>({mapFloor::triangle({{1.7091596499085426,44.20192539691925}, {-0.004104943946003914,40.115031599998474}, {-0.004105037078261375,44.20192539691925}}),mapFloor::triangle({{1.7091596499085426,44.20192539691925}, {1.7091598361730576,40.115031599998474}, {-0.004104943946003914,40.115031599998474}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Return to Entry Hall"}),
												pair<string, string>({"copy","AT_ChapelMain"}),
												pair<string, string>({"areaTransition","ChapelEntryHall"}),
												pair<string, string>({"direction","STAND_LEFT"}),
												pair<string, string>({"playerPosX","73"}),
												pair<string, string>({"playerPosY","55"}),
												pair<string, string>({"audio","CHAPELDOOR"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}),})),
			mapObject("AT_MainToSecret", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 93.7, 7 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{95.4187273979187,9.208487719297409}, {91.75046682357788,10.57279035449028}, {91.75046682357788,9.208487719297409}}),mapFloor::triangle({{95.4187273979187,9.208487719297409}, {95.4187273979187,10.57279035449028}, {91.75046682357788,10.57279035449028}}), }), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter"}),
																		pair<string, string>({"copy","AT_MainToSecret"}),
																		pair<string, string>({"cutscene","RightWingDoorToSecret"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"}) })),
			mapObject("AT_SecretToMain", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 78, 98 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{80.23961186408997,98.64625334739685}, {76.12479329109192,100.01055002212524}, {76.12479329109192,98.64625334739685}}),mapFloor::triangle({{80.23961186408997,98.64625334739685}, {80.23961186408997,100.01055002212524}, {76.12479329109192,100.01055002212524}}), }), Map<string, string>({
																			pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Leave"}),
																			pair<string, string>({"copy","AT_SecretToMain"}),
																			pair<string, string>({"areaTransition","ChapelRight1"}),
																			pair<string, string>({"direction","STAND_FRONT"}),
																			pair<string, string>({"playerPosX","94.5"}),
																			pair<string, string>({"playerPosY","10.5"}),
																			pair<string, string>({"audio","CHAPELMULTISTEP"}),
																			pair<string, string>({"x","0"}),
																			pair<string, string>({"y","0"}),
																			pair<string, string>({"h","50"}),
																			pair<string, string>({"w","50"}),
																			pair<string, string>("colour", "WHITE"),
																			pair<string, string>("uniqueID", mapPopupTextID),
																			pair<string, string>("anchorStyle", "TOPLEFT"),
																			pair<string, string>("shadowColour", "BLACK"),
																			pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																			pair<string, string>({"format","LightText_20"}), })),
			mapObject("AT_CorridorToTank", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 54, 88 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{53.16888689994812,85.75624227523804}, {51.10576152801514,89.70137238502502}, {51.10575556755066,85.75624227523804}}),mapFloor::triangle({{53.16888689994812,85.75624227523804}, {53.1688928604126,89.70137238502502}, {51.10576152801514,89.70137238502502}}), }), Map<string, string>({
				pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter Water Room"}),
				pair<string, string>({"copy","AT_CorridorToTank"}),
				pair<string, string>({"areaTransition","ChapelRight2"}),
				pair<string, string>({"direction","STAND_RIGHT"}),
				pair<string, string>({"playerPosX","4"}),
				pair<string, string>({"playerPosY","94"}),
				pair<string, string>({"audio","CHAPELMULTISTEP"}),
				pair<string, string>({"x","0"}),
				pair<string, string>({"y","0"}),
				pair<string, string>({"h","50"}),
				pair<string, string>({"w","50"}),
				pair<string, string>("colour", "WHITE"),
				pair<string, string>("uniqueID", mapPopupTextID),
				pair<string, string>("anchorStyle", "TOPLEFT"),
				pair<string, string>("shadowColour", "BLACK"),
				pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
				pair<string, string>({"format","LightText_20"}), })),
			mapObject("ToolsOnBench", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 32, 27 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{33.45235288143158,25.694987177848816}, {31.798970699310303,29.75674867630005}, {33.45235288143158,29.75674867630005}}),mapFloor::triangle({{33.45235288143158,25.694987177848816}, {31.798970699310303,25.694987177848816}, {31.798970699310303,29.75674867630005}}), }), Map<string, string>({
																					pair<string, string>("message","$LANGUAGE$_Map Pop Up Text_Check Tool Bench"),
																					pair<string, string>("copy","ToolsOnBench"),
																					pair<string, string>("cutscene","ToolsOnBenchCorrect"),
																					pair<string, string>("x","5"),
																					pair<string, string>("y","0"),
																					pair<string, string>("h","50"),
																					pair<string, string>("w","50"),
																					pair<string, string>("colour", "WHITE"),
																					pair<string, string>("uniqueID", mapPopupTextID),
																					pair<string, string>("anchorStyle", "TOPLEFT"),
																					pair<string, string>("shadowColour", "BLACK"),
																					pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																					pair<string, string>("format","LightText_20"), }))
			}),
			List<mapFloor>({
			mapFloor("ChapelFloorWood", List<mapFloor::triangle>({mapFloor::triangle({{86.97203993797302,93.82975697517395}, {69.99981999397278,99.92440342903137}, {86.97203993797302,99.92440938949585}}),mapFloor::triangle({{86.97203993797302,93.82975697517395}, {69.99981999397278,93.82975697517395}, {69.99981999397278,99.92440342903137}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
			mapFloor("ChapelFloorStone", List<mapFloor::triangle>({mapFloor::triangle({{9.692805260419846,39.87107276916504}, {-0.004105020343558863,44.31262910366058}, {9.692805260419846,44.31394934654236}}),mapFloor::triangle({{16.238251328468323,39.875590801239014}, {9.692805260419846,44.31394934654236}, {16.238251328468323,44.314610958099365}}),mapFloor::triangle({{18.46397966146469,39.88010883331299}, {16.238251328468323,44.314610958099365}, {18.46397966146469,44.31527256965637}}),
mapFloor::triangle({{9.692806005477905,32.09443390369415}, {16.238251328468323,24.322311580181122}, {9.692806005477905,24.317793548107147}}),mapFloor::triangle({{9.692805260419846,39.87107276916504}, {16.238251328468323,32.09895193576813}, {9.692806005477905,32.09443390369415}}),mapFloor::triangle({{6.530769914388657,24.317793548107147}, {-0.004104524850845337,32.09443390369415}, {6.530769914388657,32.09443390369415}}),
mapFloor::triangle({{9.692806005477905,24.317793548107147}, {6.530769914388657,32.09443390369415}, {9.692806005477905,32.09443390369415}}),mapFloor::triangle({{6.530769914388657,24.317793548107147}, {-0.004104286199435592,18.86223554611206}, {-0.004104524850845337,24.317793548107147}}),mapFloor::triangle({{9.692806005477905,24.317793548107147}, {16.238251328468323,19.806402921676636}, {9.692806005477905,19.80188488960266}}),
mapFloor::triangle({{9.692806005477905,16.005899012088776}, {18.530678749084473,5.861721187829971}, {9.692806750535965,5.857203155755997}}),mapFloor::triangle({{17.349079251289368,24.322311580181122}, {18.459908664226532,21.046066284179688}, {17.349079251289368,19.806402921676636}}),mapFloor::triangle({{17.349079251289368,32.09895193576813}, {18.459907174110413,24.322311580181122}, {17.349079251289368,24.322311580181122}}),
mapFloor::triangle({{16.238251328468323,32.09895193576813}, {17.349079251289368,24.322311580181122}, {16.238251328468323,24.322311580181122}}),mapFloor::triangle({{16.238251328468323,24.322311580181122}, {17.349079251289368,19.806402921676636}, {16.238251328468323,19.806402921676636}}),mapFloor::triangle({{19.963444769382477,16.01041704416275}, {26.97160542011261,9.01166945695877}, {19.963444769382477,9.01166945695877}}),
mapFloor::triangle({{21.219564974308014,24.642997980117798}, {24.503058195114136,29.75674867630005}, {26.97160542011261,25.694987177848816}}),mapFloor::triangle({{26.97160542011261,19.89494562149048}, {21.219564974308014,24.642997980117798}, {26.97160542011261,25.694987177848816}}),mapFloor::triangle({{20.029211044311523,21.046066284179688}, {21.219564974308014,24.642997980117798}, {20.933127403259277,19.89494562149048}}),
mapFloor::triangle({{26.97160542011261,25.694987177848816}, {32.925960421562195,29.75674867630005}, {32.925960421562195,25.694987177848816}}),mapFloor::triangle({{19.963444769382477,9.01166945695877}, {26.97160542011261,4.605276882648468}, {19.963444769382477,4.605276510119438}}),mapFloor::triangle({{44.638168811798096,9.01167020201683}, {47.84111976623535,4.605277627706528}, {44.638168811798096,4.605277627706528}}),
mapFloor::triangle({{35.61835289001465,9.01166945695877}, {44.638168811798096,4.605277627706528}, {35.61835289001465,4.605277255177498}}),mapFloor::triangle({{47.84111976623535,4.605277627706528}, {44.638168811798096,1.4417212456464767}, {44.638168811798096,4.605277627706528}}),mapFloor::triangle({{26.97160542011261,9.01166945695877}, {28.441134095191956,4.605276882648468}, {26.97160542011261,4.605276882648468}}),
mapFloor::triangle({{31.626394391059875,9.01166945695877}, {28.441134095191956,12.952755391597748}, {31.626394391059875,12.952756881713867}}),mapFloor::triangle({{44.638168811798096,9.01167020201683}, {47.84111976623535,12.952756881713867}, {47.84111976623535,9.01167020201683}}),mapFloor::triangle({{59.89586114883423,12.952756881713867}, {64.6380066871643,9.01167094707489}, {58.06513428688049,7.278957217931747}}),
mapFloor::triangle({{47.84111976623535,9.01167020201683}, {54.29689884185791,12.952756881713867}, {55.66410422325134,7.278957962989807}}),mapFloor::triangle({{54.29689884185791,12.952756881713867}, {58.06513428688049,7.278957217931747}, {55.66410422325134,7.278957962989807}}),mapFloor::triangle({{49.35427010059357,6.844031810760498}, {47.50262498855591,9.01167094707489}, {49.35427010059357,9.01167094707489}}),
mapFloor::triangle({{53.83763909339905,1.5355270355939865}, {49.35427010059357,6.844031810760498}, {55.60700297355652,5.883525311946869}}),mapFloor::triangle({{58.06513428688049,7.278957217931747}, {64.66643810272217,6.8201810121536255}, {58.15420150756836,5.868171155452728}}),mapFloor::triangle({{55.66410422325134,7.278957962989807}, {49.35427010059357,6.844031810760498}, {49.35427010059357,9.01167094707489}}),
mapFloor::triangle({{60.61218976974487,1.453267503529787}, {64.66643810272217,6.8201810121536255}, {64.7360622882843,1.453267689794302}}),mapFloor::triangle({{58.15420150756836,5.868171155452728}, {53.83763909339905,1.5355270355939865}, {55.60700297355652,5.883525311946869}}),mapFloor::triangle({{31.626394391059875,9.01166945695877}, {35.61835289001465,12.952756881713867}, {35.61835289001465,9.01166945695877}}),
mapFloor::triangle({{28.441134095191956,9.01166945695877}, {31.626394391059875,4.605276882648468}, {28.441134095191956,4.605276882648468}}),mapFloor::triangle({{31.626394391059875,12.952756881713867}, {35.61835289001465,19.13439780473709}, {35.61835289001465,12.952756881713867}}),mapFloor::triangle({{35.61835289001465,12.952756881713867}, {44.0299391746521,19.13439780473709}, {44.0299391746521,12.952756881713867}}),
mapFloor::triangle({{35.61835289001465,9.01166945695877}, {44.0299391746521,12.952756881713867}, {44.638168811798096,9.01167020201683}}),mapFloor::triangle({{31.626394391059875,9.01166945695877}, {35.61835289001465,4.605277255177498}, {31.626394391059875,4.605276882648468}}),mapFloor::triangle({{31.626394391059875,19.89494562149048}, {35.61835289001465,22.75586873292923}, {35.61835289001465,19.13439780473709}}),
mapFloor::triangle({{31.626394391059875,19.89494562149048}, {28.346529603004456,22.75586873292923}, {31.626394391059875,22.75586873292923}}),mapFloor::triangle({{44.23523247241974,19.290319085121155}, {37.59697377681732,23.708607256412506}, {44.14079487323761,23.708607256412506}}),mapFloor::triangle({{44.14079487323761,23.708607256412506}, {37.993669509887695,25.427579879760742}, {44.53749060630798,25.427579879760742}}),
mapFloor::triangle({{65.91168642044067,45.041364431381226}, {37.993669509887695,25.427579879760742}, {37.993669509887695,45.22719383239746}}),mapFloor::triangle({{67.75387525558472,27.632737159729004}, {65.95546007156372,23.473992943763733}, {62.26857304573059,23.75909686088562}}),mapFloor::triangle({{67.79491901397705,24.772989749908447}, {66.10853672027588,20.65746784210205}, {65.95546007156372,23.473992943763733}}),
mapFloor::triangle({{66.5147602558136,23.735828697681427}, {72.79825806617737,12.731172144412994}, {66.5147602558136,12.731172144412994}}),mapFloor::triangle({{68.67315173149109,15.776905417442322}, {74.59683418273926,29.35398817062378}, {74.59683418273926,15.776903927326202}}),mapFloor::triangle({{84.87533330917358,15.776905417442322}, {87.64468431472778,29.35398817062378}, {87.64468431472778,15.776903927326202}}),
mapFloor::triangle({{98.01170825958252,15.776905417442322}, {99.87488389015198,29.35398817062378}, {99.87488389015198,15.776903927326202}}),mapFloor::triangle({{100.01084804534912,27.31577455997467}, {70.262610912323,29.17895019054413}, {100.01084804534912,29.17895019054413}}),mapFloor::triangle({{100.01051425933838,18.297722935676575}, {74.59683418273926,15.776903927326202}, {74.55582022666931,18.297722935676575}}),
mapFloor::triangle({{82.17763304710388,5.593329668045044}, {87.43307590484619,13.447986543178558}, {82.17763304710388,13.447986543178558}}),mapFloor::triangle({{77.7030348777771,13.448338210582733}, {85.05905270576477,16.437755525112152}, {77.7030348777771,16.437755525112152}}),mapFloor::triangle({{87.27898597717285,9.324761480093002}, {99.89020228385925,16.329263150691986}, {87.27898597717285,16.329263150691986}}),
mapFloor::triangle({{84.3005895614624,29.089322686195374}, {88.21942806243896,42.96439290046692}, {88.21942806243896,29.089322686195374}}),mapFloor::triangle({{95.39399147033691,42.96439290046692}, {77.12602615356445,56.8394660949707}, {95.39399743080139,56.8394660949707}}),mapFloor::triangle({{53.16888689994812,85.75624227523804}, {6.611502170562744,89.70136642456055}, {53.1688928604126,89.70137238502502}}),
mapFloor::triangle({{44.0299391746521,19.13439780473709}, {37.45991885662079,19.290319085121155}, {44.23523247241974,19.290319085121155}}),mapFloor::triangle({{9.692805260419846,39.87107276916504}, {-0.004104826075490564,39.86203968524933}, {-0.004105020343558863,44.31262910366058}}),mapFloor::triangle({{16.238251328468323,39.875590801239014}, {9.692805260419846,39.87107276916504}, {9.692805260419846,44.31394934654236}}),
mapFloor::triangle({{18.46397966146469,39.88010883331299}, {16.238251328468323,39.875590801239014}, {16.238251328468323,44.314610958099365}}),mapFloor::triangle({{9.692806005477905,32.09443390369415}, {16.238251328468323,32.09895193576813}, {16.238251328468323,24.322311580181122}}),mapFloor::triangle({{9.692805260419846,39.87107276916504}, {16.238251328468323,39.875590801239014}, {16.238251328468323,32.09895193576813}}),
mapFloor::triangle({{6.530769914388657,24.317793548107147}, {-0.004104524850845337,24.317793548107147}, {-0.004104524850845337,32.09443390369415}}),mapFloor::triangle({{9.692806005477905,24.317793548107147}, {6.530769914388657,24.317793548107147}, {6.530769914388657,32.09443390369415}}),mapFloor::triangle({{6.530769914388657,24.317793548107147}, {6.530769914388657,18.86223554611206}, {-0.004104286199435592,18.86223554611206}}),
mapFloor::triangle({{9.692806005477905,24.317793548107147}, {16.238251328468323,24.322311580181122}, {16.238251328468323,19.806402921676636}}),mapFloor::triangle({{9.692806005477905,16.005899012088776}, {18.530678749084473,16.01041704416275}, {18.530678749084473,5.861721187829971}}),mapFloor::triangle({{17.349079251289368,24.322311580181122}, {18.459907174110413,24.322311580181122}, {18.459908664226532,21.046066284179688}}),
mapFloor::triangle({{17.349079251289368,32.09895193576813}, {18.459907174110413,32.09895193576813}, {18.459907174110413,24.322311580181122}}),mapFloor::triangle({{16.238251328468323,32.09895193576813}, {17.349079251289368,32.09895193576813}, {17.349079251289368,24.322311580181122}}),mapFloor::triangle({{16.238251328468323,24.322311580181122}, {17.349079251289368,24.322311580181122}, {17.349079251289368,19.806402921676636}}),
mapFloor::triangle({{19.963444769382477,16.01041704416275}, {26.97160542011261,16.01041704416275}, {26.97160542011261,9.01166945695877}}),mapFloor::triangle({{26.97160542011261,19.89494562149048}, {20.933127403259277,19.89494562149048}, {21.219564974308014,24.642997980117798}}),mapFloor::triangle({{20.029211044311523,21.046066284179688}, {19.9409618973732,24.642997980117798}, {21.219564974308014,24.642997980117798}}),
mapFloor::triangle({{26.97160542011261,25.694987177848816}, {24.503058195114136,29.75674867630005}, {32.925960421562195,29.75674867630005}}),mapFloor::triangle({{19.963444769382477,9.01166945695877}, {26.97160542011261,9.01166945695877}, {26.97160542011261,4.605276882648468}}),mapFloor::triangle({{44.638168811798096,9.01167020201683}, {47.84111976623535,9.01167020201683}, {47.84111976623535,4.605277627706528}}),
mapFloor::triangle({{35.61835289001465,9.01166945695877}, {44.638168811798096,9.01167020201683}, {44.638168811798096,4.605277627706528}}),mapFloor::triangle({{47.84111976623535,4.605277627706528}, {47.84111976623535,1.4417212456464767}, {44.638168811798096,1.4417212456464767}}),mapFloor::triangle({{26.97160542011261,9.01166945695877}, {28.441134095191956,9.01166945695877}, {28.441134095191956,4.605276882648468}}),
mapFloor::triangle({{31.626394391059875,9.01166945695877}, {28.441134095191956,9.01166945695877}, {28.441134095191956,12.952755391597748}}),mapFloor::triangle({{44.638168811798096,9.01167020201683}, {44.0299391746521,12.952756881713867}, {47.84111976623535,12.952756881713867}}),mapFloor::triangle({{59.89586114883423,12.952756881713867}, {64.6380066871643,12.952756881713867}, {64.6380066871643,9.01167094707489}}),
mapFloor::triangle({{47.84111976623535,9.01167020201683}, {47.84111976623535,12.952756881713867}, {54.29689884185791,12.952756881713867}}),mapFloor::triangle({{54.29689884185791,12.952756881713867}, {59.89586114883423,12.952756881713867}, {58.06513428688049,7.278957217931747}}),mapFloor::triangle({{49.35427010059357,6.844031810760498}, {47.50262498855591,6.844031810760498}, {47.50262498855591,9.01167094707489}}),
mapFloor::triangle({{53.83763909339905,1.5355270355939865}, {49.35427010059357,1.5355270355939865}, {49.35427010059357,6.844031810760498}}),mapFloor::triangle({{58.06513428688049,7.278957217931747}, {64.6380066871643,9.01167094707489}, {64.66643810272217,6.8201810121536255}}),mapFloor::triangle({{55.66410422325134,7.278957962989807}, {55.60700297355652,5.883525311946869}, {49.35427010059357,6.844031810760498}}),
mapFloor::triangle({{60.61218976974487,1.453267503529787}, {58.15420150756836,5.868171155452728}, {64.66643810272217,6.8201810121536255}}),mapFloor::triangle({{58.15420150756836,5.868171155452728}, {60.61218976974487,1.453267503529787}, {53.83763909339905,1.5355270355939865}}),mapFloor::triangle({{31.626394391059875,9.01166945695877}, {31.626394391059875,12.952756881713867}, {35.61835289001465,12.952756881713867}}),
mapFloor::triangle({{28.441134095191956,9.01166945695877}, {31.626394391059875,9.01166945695877}, {31.626394391059875,4.605276882648468}}),mapFloor::triangle({{31.626394391059875,12.952756881713867}, {31.626394391059875,19.89494562149048}, {35.61835289001465,19.13439780473709}}),mapFloor::triangle({{35.61835289001465,12.952756881713867}, {35.61835289001465,19.13439780473709}, {44.0299391746521,19.13439780473709}}),
mapFloor::triangle({{35.61835289001465,9.01166945695877}, {35.61835289001465,12.952756881713867}, {44.0299391746521,12.952756881713867}}),mapFloor::triangle({{31.626394391059875,9.01166945695877}, {35.61835289001465,9.01166945695877}, {35.61835289001465,4.605277255177498}}),mapFloor::triangle({{31.626394391059875,19.89494562149048}, {31.626394391059875,22.75586873292923}, {35.61835289001465,22.75586873292923}}),
mapFloor::triangle({{31.626394391059875,19.89494562149048}, {28.346529603004456,19.89494562149048}, {28.346529603004456,22.75586873292923}}),mapFloor::triangle({{44.23523247241974,19.290319085121155}, {37.45991885662079,19.290319085121155}, {37.59697377681732,23.708607256412506}}),mapFloor::triangle({{44.14079487323761,23.708607256412506}, {37.59697377681732,23.708607256412506}, {37.993669509887695,25.427579879760742}}),
mapFloor::triangle({{65.91168642044067,45.041364431381226}, {65.91168642044067,25.427579879760742}, {37.993669509887695,25.427579879760742}}),mapFloor::triangle({{67.75387525558472,27.632737159729004}, {67.79491901397705,24.772989749908447}, {65.95546007156372,23.473992943763733}}),mapFloor::triangle({{67.79491901397705,24.772989749908447}, {70.38183808326721,25.201591849327087}, {66.10853672027588,20.65746784210205}}),
mapFloor::triangle({{66.5147602558136,23.735828697681427}, {72.79825806617737,23.735828697681427}, {72.79825806617737,12.731172144412994}}),mapFloor::triangle({{68.67315173149109,15.776905417442322}, {68.67315173149109,29.35398817062378}, {74.59683418273926,29.35398817062378}}),mapFloor::triangle({{84.87533330917358,15.776905417442322}, {84.87533330917358,29.35398817062378}, {87.64468431472778,29.35398817062378}}),
mapFloor::triangle({{98.01170825958252,15.776905417442322}, {98.01170825958252,29.35398817062378}, {99.87488389015198,29.35398817062378}}),mapFloor::triangle({{100.01084804534912,27.31577455997467}, {70.262610912323,27.31577455997467}, {70.262610912323,29.17895019054413}}),mapFloor::triangle({{100.01051425933838,18.297722935676575}, {100.01051425933838,15.693190693855286}, {74.59683418273926,15.776903927326202}}),
mapFloor::triangle({{82.17763304710388,5.593329668045044}, {87.43307590484619,5.593328177928925}, {87.43307590484619,13.447986543178558}}),mapFloor::triangle({{77.7030348777771,13.448338210582733}, {85.05905270576477,9.193995594978333}, {85.05905270576477,16.437755525112152}}),mapFloor::triangle({{87.27898597717285,9.324761480093002}, {99.89020228385925,9.324760735034943}, {99.89020228385925,16.329263150691986}}),
mapFloor::triangle({{84.3005895614624,29.089322686195374}, {84.3005895614624,42.96439290046692}, {88.21942806243896,42.96439290046692}}),mapFloor::triangle({{95.39399147033691,42.96439290046692}, {77.12602019309998,42.96439290046692}, {77.12602615356445,56.8394660949707}}),mapFloor::triangle({{53.16888689994812,85.75624227523804}, {6.611502170562744,85.75624227523804}, {6.611502170562744,89.70136642456055}}),
mapFloor::triangle({{44.0299391746521,19.13439780473709}, {35.61835289001465,19.13439780473709}, {37.45991885662079,19.290319085121155}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "CHAPELSTEP"})})),
			mapFloor("ChapelFloorStoneOrWood", List<mapFloor::triangle>({mapFloor::triangle({{9.269341826438904,16.00787043571472}, {6.873767077922821,19.80188488960266}, {9.269341826438904,19.803854823112488}}),mapFloor::triangle({{31.498736143112183,16.01041704416275}, {28.715652227401733,19.805826246738434}, {31.498736143112183,19.806402921676636}}),mapFloor::triangle({{28.715652227401733,16.00984036922455}, {31.498736143112183,12.943169474601746}, {28.715652227401733,12.942591309547424}}),
mapFloor::triangle({{28.715652227401733,16.00984036922455}, {9.269341826438904,19.803854823112488}, {28.715652227401733,19.805826246738434}}),mapFloor::triangle({{9.269341826438904,19.803854823112488}, {6.873767077922821,24.327275156974792}, {9.269341826438904,24.32924509048462}}),mapFloor::triangle({{9.269341826438904,16.00787043571472}, {6.873767077922821,16.005899012088776}, {6.873767077922821,19.80188488960266}}),
mapFloor::triangle({{31.498736143112183,16.01041704416275}, {28.715652227401733,16.00984036922455}, {28.715652227401733,19.805826246738434}}),mapFloor::triangle({{28.715652227401733,16.00984036922455}, {31.498736143112183,16.01041704416275}, {31.498736143112183,12.943169474601746}}),mapFloor::triangle({{28.715652227401733,16.00984036922455}, {9.269341826438904,16.00787043571472}, {9.269341826438904,19.803854823112488}}),
mapFloor::triangle({{9.269341826438904,19.803854823112488}, {6.873767077922821,19.80188488960266}, {6.873767077922821,24.327275156974792}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "CHAPELSTEP"})})),
				}), {}, { 5000, 5000 }, { Map<string, string>({
					pair<string,string>({"song1",to_string(CHAPEL_WAV_1) + " " + "MusicVolume"}),
					pair<string, string>({ "LoadingScreenImage",to_string(LOADINGSCREEN_1) }),
					}) });
		maps["ChapelRight2"] = mapInstance("ChapelRight2", CHAPELRIGHT_2, { 35, 95 /*1, 97*/}, List<mapObject>({
			mapObject("DormLetter3", true, false, false, "","0",0,0,"","","CENTRE",{21, 66}, false, List<mapFloor::triangle>({mapFloor::triangle({{20.846445858478546,67.14304685592651}, {22.54289835691452,66.47959351539612}, {20.846445858478546,66.47899746894836}}),mapFloor::triangle({{20.846445858478546,67.14304685592651}, {22.54289835691452,67.14248657226562}, {22.54289835691452,66.47959351539612}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Note"}),
																		pair<string, string>({"copy","DormLetter3"}),
																		pair<string, string>({"cutscene","DormLetter3"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("DormLetter2", true, false, false, "","0",0,0,"","","CENTRE",{33.3, 60}, false, List<mapFloor::triangle>({mapFloor::triangle({{31.082195043563843,60.52393913269043}, {34.177446365356445,58.49584937095642}, {31.082195043563843,58.493053913116455}}),mapFloor::triangle({{31.082195043563843,60.52393913269043}, {34.177446365356445,60.52132844924927}, {34.177446365356445,58.49584937095642}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Note"}),
																		pair<string, string>({"copy","DormLetter2"}),
																		pair<string, string>({"cutscene","DormLetter2"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("DormLetter1", true, false, false, "","0",0,0,"","","CENTRE",{22.5, 56.5}, false, List<mapFloor::triangle>({mapFloor::triangle({{20.79804241657257,60.52393913269043}, {23.893295228481293,58.49584937095642}, {20.79804241657257,58.493053913116455}}),mapFloor::triangle({{20.79804241657257,60.52393913269043}, {23.893295228481293,60.52132844924927}, {23.893295228481293,58.49584937095642}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Read Note"}),
																		pair<string, string>({"copy","DormLetter1"}),
																		pair<string, string>({"cutscene","DormLetter1"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("FilmGrain2", false, true, true, imageLookup.getSequenceAsString("FilmGrain2","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject::getOnetimeTrigger("VisionRoom2",List<mapFloor::triangle>({mapFloor::triangle({{61.107903718948364,53.51046323776245}, {63.45706582069397,45.00362575054169}, {61.107903718948364,45.00361382961273}}),mapFloor::triangle({{61.107903718948364,53.51046323776245}, {63.45706582069397,53.51046323776245}, {63.45706582069397,45.00362575054169}}),}), ""),
			mapObject::getOnetimeTrigger("VisionRoom",List<mapFloor::triangle>({mapFloor::triangle({{43.5202956199646,53.51046323776245}, {45.869457721710205,45.00362575054169}, {43.5202956199646,45.00361382961273}}),mapFloor::triangle({{43.5202956199646,53.51046323776245}, {45.869457721710205,53.51046323776245}, {45.869457721710205,45.00362575054169}}),}), ""),
			mapObject::getOnetimeTrigger("OudinCutscenePlanks",List<mapFloor::triangle>({mapFloor::triangle({{80.12887835502625,5.909644067287445}, {81.0120165348053,8.167190104722977}, {80.12887835502625,8.167190104722977}}),mapFloor::triangle({{80.12887835502625,5.909644067287445}, {81.0120165348053,5.909644067287445}, {81.0120165348053,8.167190104722977}}),}), ""),
			mapObject::getOnetimeTrigger("PlankFight1",List<mapFloor::triangle>({mapFloor::triangle({{68.70834827423096,15.179626643657684}, {70.48789858818054,7.950266450643539}, {70.64704298973083,15.186618268489838}}),mapFloor::triangle({{68.70834827423096,15.179626643657684}, {68.51587891578674,8.026860654354095}, {70.48789858818054,7.950266450643539}}),}), ""),
			mapObject::getOnetimeTrigger("PlankFight2",List<mapFloor::triangle>({mapFloor::triangle({{65.02983570098877,18.457326292991638}, {59.11948084831238,19.491136074066162}, {59.19397473335266,18.127799034118652}}),mapFloor::triangle({{65.02983570098877,18.457326292991638}, {64.95534181594849,19.820663332939148}, {59.11948084831238,19.491136074066162}}),}), ""),
			mapObject::getOnetimeTrigger("PlankFight3",List<mapFloor::triangle>({mapFloor::triangle({{41.65359437465668,9.228577464818954}, {47.48260676860809,7.8788042068481445}, {40.40452539920807,7.878804951906204}}),mapFloor::triangle({{41.65359437465668,9.228577464818954}, {51.104170083999634,9.228577464818954}, {47.48260676860809,7.8788042068481445}}),}), ""),
			mapObject("VISIONITEM$PLAYER1$", true, true, false, imageLookup.getSequenceAsString("VISIONITEM$PLAYER1$", "STAND_FRONT"),"0",1,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{47.3, 43.3}, false, List<mapFloor::triangle>({mapFloor::triangle({{44.496023654937744,45.985448360443115}, {50.037455558776855,44.25798058509827}, {44.496023654937744,44.25798058509827}}),mapFloor::triangle({{44.496023654937744,45.985448360443115}, {50.037455558776855,45.985448360443115}, {50.037455558776855,44.25798058509827}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Check Altar"}),
																		pair<string, string>({"copy","VISIONITEM$PLAYER1$"}),
																		pair<string, string>({"cutscene","VisionItem"}),
																		pair<string, string>({"x","-5"}),
																		pair<string, string>({"y","-6"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_Rope2Dorms", true, false, false, "","0",0,0,"","","CENTRE",{65, 45}, false, List<mapFloor::triangle>({mapFloor::triangle({{62.19095587730408,53.51046323776245}, {70.83097696304321,45.00362575054169}, {62.19095587730408,45.00361382961273}}),mapFloor::triangle({{62.19095587730408,53.51046323776245}, {70.83097696304321,53.51046323776245}, {70.83097696304321,45.00362575054169}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Downstairs"}),
																		pair<string, string>({"copy","AT_Rope2Dorms"}),
																		pair<string, string>({"cutscene","Rope2Dorm"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_Dorms2Basement", true, false, false, "","0",0,0,"","","CENTRE",{41, 75}, false, List<mapFloor::triangle>({mapFloor::triangle({{34.958067536354065,76.90592408180237}, {41.075873374938965,72.03199863433838}, {34.958067536354065,72.03199863433838}}),mapFloor::triangle({{34.958067536354065,76.90592408180237}, {41.075873374938965,76.90592408180237}, {41.075873374938965,72.03199863433838}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Downstairs"}),
																		pair<string, string>({"copy","AT_Dorms2Basement"}),
																		pair<string, string>({"cutscene","Dorm2Basement"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_Basement2Dorms", true, false, false, "","0",0,0,"","","CENTRE",{31, 85}, false, List<mapFloor::triangle>({mapFloor::triangle({{31.25210404396057,94.58560347557068}, {28.746023774147034,84.47902202606201}, {24.66292679309845,87.48431205749512}}),mapFloor::triangle({{31.25210404396057,94.58560347557068}, {35.38517355918884,91.46900773048401}, {28.746023774147034,84.47902202606201}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Upstairs"}),
																		pair<string, string>({"copy","AT_Basement2Dorms"}),
																		pair<string, string>({"cutscene","Basement2Dorms"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_Dorms2Rope", true, false, false, "","0",0,0,"","","CENTRE",{15, 58}, false, List<mapFloor::triangle>({mapFloor::triangle({{10.926849395036697,63.61709237098694}, {19.42375898361206,56.03576898574829}, {10.926849395036697,55.11023998260498}}),mapFloor::triangle({{10.926849395036697,63.61709237098694}, {19.42375898361206,64.5426094532013}, {19.42375898361206,56.03576898574829}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Upstairs"}),
																		pair<string, string>({"copy","AT_Dorms2Rope"}),
																		pair<string, string>({"cutscene","Dorms2Rope"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_Rope2", true, false, false, "","0",0,0,"","","CENTRE",{34, 48}, false, List<mapFloor::triangle>({mapFloor::triangle({{33.66786539554596,50.27298331260681}, {36.017027497291565,48.24110269546509}, {33.66786539554596,48.24109971523285}}),mapFloor::triangle({{33.66786539554596,50.27298331260681}, {36.017027497291565,50.27298331260681}, {36.017027497291565,48.24110269546509}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Climb Rope"}),
																		pair<string, string>({"copy","AT_Rope2"}),
																		pair<string, string>({"cutscene","Rope2"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_Rope1", true, false, false, "","0",0,0,"","","CENTRE",{22, 31}, false, List<mapFloor::triangle>({mapFloor::triangle({{19.681711494922638,30.9594064950943}, {21.223294734954834,34.29582118988037}, {22.579868137836456,33.19517970085144}}),mapFloor::triangle({{18.325138092041016,32.06005096435547}, {18.01324486732483,29.53585386276245}, {16.656671464443207,30.63649833202362}}),mapFloor::triangle({{19.681711494922638,30.9594064950943}, {22.590045630931854,26.841816306114197}, {18.01324486732483,29.53585386276245}}),
mapFloor::triangle({{19.681711494922638,30.9594064950943}, {18.325138092041016,32.06005096435547}, {21.223294734954834,34.29582118988037}}),mapFloor::triangle({{18.325138092041016,32.06005096435547}, {19.681711494922638,30.9594064950943}, {18.01324486732483,29.53585386276245}}),mapFloor::triangle({{19.681711494922638,30.9594064950943}, {24.258512258529663,28.265368938446045}, {22.590045630931854,26.841816306114197}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Climb Rope"}),
																		pair<string, string>({"copy","AT_Rope1"}),
																		pair<string, string>({"cutscene","Rope1"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_DoorToRope1", true, false, false, "","0",0,0,"","","CENTRE",{3.5, 4}, false, List<mapFloor::triangle>({mapFloor::triangle({{4.063872992992401,5.639103800058365}, {1.2612391263246536,7.179854065179825}, {4.063872992992401,7.179854065179825}}),mapFloor::triangle({{4.063872992992401,5.639103800058365}, {1.2612391263246536,5.639103800058365}, {1.2612391263246536,7.179854065179825}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter"}),
																		pair<string, string>({"copy","AT_DoorToRope1"}),
																		pair<string, string>({"cutscene","Rope0ToRope1"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_DoorToRope0", true, false, false, "","0",0,0,"","","CENTRE",{12, 42}, false, List<mapFloor::triangle>({mapFloor::triangle({{10.693641752004623,41.22710824012756}, {9.566019475460052,45.380547642707825}, {12.756949663162231,43.143653869628906}}),mapFloor::triangle({{10.693641752004623,41.22710824012756}, {7.502712309360504,43.46400201320648}, {9.566019475460052,45.380547642707825}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter"}),
																		pair<string, string>({"copy","AT_DoorToRope0"}),
																		pair<string, string>({"cutscene","Rope1ToRope0"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_TankToCorridor", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 4, 95 }, false, List<mapFloor::triangle>({mapFloor::triangle({{3.962678089737892,98.38550090789795}, {-0.054016709327697754,96.34490013122559}, {2.3440688848495483,100.01065731048584}}),mapFloor::triangle({{2.3440688848495483,100.01065731048584}, {-0.054016709327697754,96.34490013122559}, {-0.028504477813839912,100.0783920288086}}),mapFloor::triangle({{3.962678089737892,98.38550090789795}, {0.4185187630355358,95.72229981422424}, {-0.054016709327697754,96.34490013122559}}),}), Map<string, string>({
																					pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Return to Corridor"}),
																					pair<string, string>({"copy","AT_TankToCorridor"}),
																					pair<string, string>({"areaTransition","ChapelRight1"}),
																					pair<string, string>({"direction","STAND_LEFT"}),
																					pair<string, string>({"playerPosX","50"}),
																					pair<string, string>({"playerPosY","88"}),
																					pair<string, string>({"audio","WOODENSTAIRS"}),
																					pair<string, string>({"x","0"}),
																					pair<string, string>({"y","0"}),
																					pair<string, string>({"h","50"}),
																					pair<string, string>({"w","50"}),
																					pair<string, string>("colour", "WHITE"),
																					pair<string, string>("uniqueID", mapPopupTextID),
																					pair<string, string>("anchorStyle", "TOPLEFT"),
																					pair<string, string>("shadowColour", "BLACK"),
																					pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																					pair<string, string>({"format","LightText_20"}), })),
			mapObject("AT_ToDownstairs", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 89, 5 }, false, List<mapFloor::triangle>({mapFloor::triangle({{86.2735390663147,6.080254167318344}, {91.00373387336731,6.080254539847374}, {86.46487593650818,5.293634533882141}}),mapFloor::triangle({{86.46487593650818,5.293634533882141}, {91.00373387336731,6.080254539847374}, {87.27275729179382,4.230634868144989}}),mapFloor::triangle({{88.66528868675232,3.433385118842125}, {87.27275729179382,4.230634868144989}, {91.00373387336731,6.080254539847374}}),
mapFloor::triangle({{89.7495448589325,3.1144849956035614}, {88.66528868675232,3.433385118842125}, {91.00373387336731,6.080254539847374}}),mapFloor::triangle({{90.96136093139648,3.0081847682595253}, {89.7495448589325,3.1144849956035614}, {91.00373387336731,6.080254539847374}}),}), Map<string, string>({
					pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Downstairs"}),
					pair<string, string>({"copy","AT_ToDownstairs"}),
					pair<string, string>({"areaTransition","ChapelRight1"}),
					pair<string, string>({"direction","STAND_FRONT"}),
					pair<string, string>({"playerPosX","70"}),
					pair<string, string>({"playerPosY","15"}),
					pair<string, string>({"audio","CHAPELMULTISTEP"}),
					pair<string, string>({"x","0"}),
					pair<string, string>({"y","0"}),
					pair<string, string>({"h","50"}),
					pair<string, string>({"w","50"}),
					pair<string, string>("colour", "WHITE"),
					pair<string, string>("uniqueID", mapPopupTextID),
					pair<string, string>("anchorStyle", "TOPLEFT"),
					pair<string, string>("shadowColour", "BLACK"),
					pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
					pair<string, string>({"format","LightText_20"}), })),
			mapObject("AT_Basement2Outside", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 89, 98 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{81.942880153656,100.01970529556274}, {88.06068301200867,98.09967875480652}, {81.942880153656,98.09967875480652}}),mapFloor::triangle({{81.942880153656,100.01970529556274}, {88.06068301200867,100.01970529556274}, {88.06068301200867,98.09967875480652}}), }), Map<string, string>({
							pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Outside"}),
							pair<string, string>({"copy","AT_Basement2Outside"}),
							pair<string, string>({"areaTransition","EstateOutside1"}),
							pair<string, string>({"direction","STAND_FRONT"}),
							pair<string, string>({"playerPosX","85"}),
							pair<string, string>({"playerPosY","68"}),
							pair<string, string>({"audio","CHAPELMULTISTEP"}),
							pair<string, string>({"x","0"}),
							pair<string, string>({"y","0"}),
							pair<string, string>({"h","50"}),
							pair<string, string>({"w","50"}),
							pair<string, string>("colour", "WHITE"),
							pair<string, string>("uniqueID", mapPopupTextID),
							pair<string, string>("anchorStyle", "TOPLEFT"),
							pair<string, string>("shadowColour", "BLACK"),
							pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
							pair<string, string>({"format","LightText_20"}), })),
			mapObject("AT_Basement2Chapel", true, false, false, "", "0", 0, 0, "1", "1.0", "CENTRE", { 55, 61 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{54.69415783882141,65.38719534873962}, {56.12926483154297,62.50157356262207}, {53.70870232582092,63.34375739097595}}),mapFloor::triangle({{54.69415783882141,65.38719534873962}, {56.87544345855713,64.68793153762817}, {56.12926483154297,62.50157356262207}}), }), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Through The Gap"}),
										pair<string, string>({"copy","AT_Basement2Chapel"}),
										pair<string, string>({"areaTransition","ChapelEntryHall"}),
										pair<string, string>({"direction","STAND_LEFT"}),
										pair<string, string>({"playerPosX","84"}),
										pair<string, string>({"playerPosY","27"}),
										pair<string, string>({"audio","DUSTY"}),
										pair<string, string>({"x","0"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>("don'tLoadIfNot", "OudinHostileTriggered"),
										pair<string, string>({"format","LightText_20"}), })),
				mapObject("ChapelRight2Top", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHT_2_TOP","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"]+1,"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
					mapObject("Lamp1", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTWALLLAMP1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{1.2, 91}, false, {}, {}),
					mapObject("Lamp2", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTWALLLAMP1","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{5.6, 88}, false, {}, {}),
					mapObject("Lamp3Dungeon", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTWALLLAMP2", "STAND_FRONT"), "1", 90, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0", "CENTRE", { 33.25, 86.25 }, false, {}, {}),
					mapObject("Lamp4Dungeon", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTWALLLAMP1", "STAND_FRONT"), "1", 90, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0", "CENTRE", { 37.3, 86.75 }, false, {}, {}),
					mapObject("Lamp5Dungeon", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTWALLLAMP1", "STAND_FRONT"), "1", 90, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0", "CENTRE", { 41.25, 83.75 }, false, {}, {}),
					mapObject("Lamp6NearGate", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTWALLLAMP2", "STAND_FRONT"), "1", 90, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0", "CENTRE", { 83.5, 62 }, false, {}, {}),
					mapObject("Lamp7NearGate", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTWALLLAMP2", "STAND_FRONT"), "1", 90, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0", "CENTRE", { 89.5, 66 }, false, {}, {}),
					mapObject("Lamp8NearGate", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTWALLLAMP2", "STAND_FRONT"), "1", 90, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0", "CENTRE", { 95.5, 70 }, false, {}, {}),
					mapObject("Lamp9NearGate", false, true, false, imageLookup.getSequenceAsString("CHAPELRIGHTWALLLAMP2", "STAND_FRONT"), "1", 90, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0", "CENTRE", { 100, 74 }, false, {}, {}),
					mapObject("CandlesOnFloor", false, true, false, imageLookup.getSequenceAsString("CandlesOnFloor", "STAND_FRONT"), "1", 90, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0", "CENTRE", { 37, 98 }, false, {}, {}),
					mapObject("CandlesOnFloor2", false, true, false, imageLookup.getSequenceAsString("CandlesOnFloor", "STAND_FRONT"), "1", 90, imageLookup.layerDefaults["PLAYER"], "1.0", "1.0", "CENTRE", { 73, 72 }, false, {}, {}),
					
					// chest on planks
					mapObject::getTreasureChest("PlanksChest1", { 50,6 }, "NORMAL2_FRONT", "Tome of Ward Against Weapons"),
					mapObject::getTreasureChest("PlanksChest2", { 65,21 }, "NORMAL3_FRONT", "Tome of Atrophy"),
					mapObject::getTreasureChest("PlanksChest3", { 7,7 }, "NORMAL3_FRONT", "Tome of Vampiric Strike"),
					// chests in dorms area
					mapObject::getTreasureChest("DormsChest1", { 26,70 }, "NORMAL3_FRONT", "Chaworth's Gown"),
					// chests in dungeon area
					mapObject::getTreasureChest("DungeonChest1", { 55,82 }, "NORMAL3_FRONT", "Artemisia Fragment"),
					mapObject::getTreasureChest("DungeonChest2", { 67,96.5 }, "NORMAL2_FRONT", "Tome of Clobber"),
					mapObject::getTreasureChest("DungeonChest3", { 73,85 }, "NORMAL3_FRONT", "Saint-Pol's Robes"),
					mapObject::getTreasureChest("DungeonChest3", { 55,70.25, }, "NORMAL2_FRONT", "Luxembourg's Wedding Dress"),
					mapObject("WaterTank", true, false, false, "","",0,0,"","","CENTRE",{17.5, 85}, false, List<mapFloor::triangle>({mapFloor::triangle({{17.487728595733643,86.07631921768188}, {17.24386066198349,84.02194380760193}, {18.598751723766327,84.96009707450867}}),mapFloor::triangle({{17.487728595733643,86.07631921768188}, {16.04340970516205,84.98351573944092}, {17.24386066198349,84.02194380760193}}),}), Map<string, string>({
										pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_WaterTank1"}),
										pair<string, string>({"copy","WaterTank"}),
										pair<string, string>({"cutscene","WaterTank1"}),
										pair<string, string>({"x","0"}),
										pair<string, string>({"y","0"}),
										pair<string, string>({"h","50"}),
										pair<string, string>({"w","50"}),
										pair<string, string>("colour", "WHITE"),
										pair<string, string>("uniqueID", mapPopupTextID),
										pair<string, string>("anchorStyle", "TOPLEFT"),
										pair<string, string>("shadowColour", "BLACK"),
										pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
										pair<string, string>("format","LightText_20")
						})),
				mapObject("BasementLockedGate", true, false, false, "", "", 0, 0, "", "", "CENTRE", { 96, 75 }, false, List<mapFloor::triangle>({ mapFloor::triangle({{92.8013265132904,78.24148535728455}, {94.16841864585876,73.41222167015076}, {90.34680724143982,76.39229893684387}}),mapFloor::triangle({{92.8013265132904,78.24148535728455}, {97.11310863494873,75.31771063804626}, {94.16841864585876,73.41222167015076}}), }), Map<string, string>({
											pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Enter"}),
											pair<string, string>({"copy","BasementLockedGate"}),
											pair<string, string>({"cutscene","BasementLockedGate"}),
											pair<string, string>({"x","0"}),
											pair<string, string>({"y","0"}),
											pair<string, string>({"h","50"}),
											pair<string, string>({"w","50"}),
											pair<string, string>("colour", "WHITE"),
											pair<string, string>("uniqueID", mapPopupTextID),
											pair<string, string>("anchorStyle", "TOPLEFT"),
											pair<string, string>("shadowColour", "BLACK"),
											pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
											pair<string, string>("format","LightText_20")
						})),
				mapObject::getOnetimeTrigger("HearingOudinInTrouble", List<mapFloor::triangle>({ mapFloor::triangle({{57.846784591674805,67.32290387153625}, {62.01513409614563,62.12791204452515}, {56.861329078674316,62.74899244308472}}),mapFloor::triangle({{56.861329078674316,62.74899244308472}, {53.63735556602478,68.74298453330994}, {57.846784591674805,67.32290387153625}}),mapFloor::triangle({{57.846784591674805,67.32290387153625}, {62.76131272315979,66.24997854232788}, {62.01513409614563,62.12791204452515}}),
mapFloor::triangle({{56.861329078674316,62.74899244308472}, {50.700706243515015,64.61068987846375}, {53.63735556602478,68.74298453330994}}), }), "OudinHostileTriggered"),

				}), List<mapFloor>({
					mapFloor("Wood", List<mapFloor::triangle>({mapFloor::triangle({{9.493985772132874,87.54488825798035}, {9.016329050064087,84.67978239059448}, {7.505679875612259,85.79267859458923}}),mapFloor::triangle({{9.155822545289993,88.33597302436829}, {11.433076113462448,86.96985840797424}, {9.493985772132874,87.54488825798035}}),mapFloor::triangle({{9.191329032182693,90.99655747413635}, {9.155822545289993,88.33597302436829}, {7.529071718454361,89.67112302780151}}),
mapFloor::triangle({{4.940975829958916,94.11181807518005}, {7.529071718454361,89.67112302780151}, {3.278718888759613,92.78638958930969}}),mapFloor::triangle({{-0.1642280607484281,96.52124047279358}, {4.940975829958916,94.11181807518005}, {3.278718888759613,92.78638958930969}}),mapFloor::triangle({{10.387811809778214,91.95059537887573}, {11.433076113462448,89.45897221565247}, {9.191329032182693,90.99655747413635}}),
mapFloor::triangle({{6.137458980083466,95.06585597991943}, {9.191329032182693,90.99655747413635}, {4.940975829958916,94.11181807518005}}),mapFloor::triangle({{1.0736546479165554,98.36049675941467}, {-0.16422824701294303,100.19974708557129}, {1.964675448834896,99.68438148498535}}),mapFloor::triangle({{1.0736546479165554,98.36049675941467}, {6.137458980083466,95.06585597991943}, {4.940975829958916,94.11181807518005}}),
mapFloor::triangle({{15.539567172527313,84.572833776474}, {13.974820077419281,81.61145448684692}, {11.450512707233429,83.17350149154663}}),mapFloor::triangle({{11.433076113462448,86.96985840797424}, {9.905903786420822,84.12930965423584}, {9.016329050064087,84.67978239059448}}),mapFloor::triangle({{12.42564395070076,86.40402555465698}, {11.450512707233429,83.17350149154663}, {9.905903786420822,84.12930965423584}}),
mapFloor::triangle({{14.369134604930878,89.00465965270996}, {14.80610966682434,86.75195574760437}, {13.083843886852264,87.36836314201355}}),mapFloor::triangle({{12.42564395070076,86.40402555465698}, {15.475788712501526,85.68897247314453}, {15.539567172527313,84.572833776474}}),mapFloor::triangle({{14.80610966682434,86.75195574760437}, {12.754744291305542,86.8861973285675}, {13.083843886852264,87.36836314201355}}),
mapFloor::triangle({{9.678222239017487,11.657509207725525}, {16.658097505569458,9.499693661928177}, {9.729807078838348,8.434387296438217}}),mapFloor::triangle({{16.658097505569458,9.499693661928177}, {17.23211705684662,11.114884167909622}, {17.23211705684662,9.985534101724625}}),mapFloor::triangle({{17.23211705684662,11.114884167909622}, {22.462068498134613,11.184839904308319}, {17.23211705684662,9.985534101724625}}),
mapFloor::triangle({{22.462068498134613,12.314189970493317}, {36.997660994529724,11.298520117998123}, {22.462068498134613,11.184839904308319}}),mapFloor::triangle({{36.997660994529724,11.298520117998123}, {41.65359437465668,9.228577464818954}, {40.40452539920807,7.878804951906204}}),mapFloor::triangle({{41.65359437465668,9.228577464818954}, {47.48260676860809,7.8788042068481445}, {40.40452539920807,7.878804951906204}}),
mapFloor::triangle({{47.48260676860809,7.8788042068481445}, {51.104170083999634,6.25622496008873}, {47.38049805164337,6.271447986364365}}),mapFloor::triangle({{38.310506939888,12.965062260627747}, {36.88574135303497,19.282427430152893}, {36.422812938690186,13.028842210769653}}),mapFloor::triangle({{38.630759716033936,19.098614156246185}, {46.08475863933563,18.576307594776154}, {46.029627323150635,19.434675574302673}}),
mapFloor::triangle({{40.926241874694824,19.693732261657715}, {50.36156177520752,20.643940567970276}, {40.87111055850983,20.552100241184235}}),mapFloor::triangle({{42.68245995044708,19.802047312259674}, {50.45495629310608,18.620842695236206}, {52.181243896484375,19.982948899269104}}),mapFloor::triangle({{52.181243896484375,19.982948899269104}, {58.32059383392334,17.525197565555573}, {59.027671813964844,19.25002783536911}}),
mapFloor::triangle({{57.51412510871887,18.802034854888916}, {55.09960055351257,12.681028246879578}, {58.86468291282654,18.47674250602722}}),mapFloor::triangle({{59.49323773384094,17.0124351978302}, {58.02296996116638,21.0317000746727}, {57.69343972206116,17.013567686080933}}),mapFloor::triangle({{64.58166837692261,17.44939535856247}, {66.0962700843811,22.061438858509064}, {63.548743724823,22.06304222345352}}),
mapFloor::triangle({{65.02983570098877,18.457326292991638}, {59.11948084831238,19.491136074066162}, {59.19397473335266,18.127799034118652}}),mapFloor::triangle({{55.09960055351257,12.681028246879578}, {54.15341854095459,6.743111461400986}, {55.53921461105347,6.840159744024277}}),mapFloor::triangle({{53.654247522354126,8.948808163404465}, {63.104087114334106,6.958241015672684}, {53.62784266471863,6.7915283143520355}}),
mapFloor::triangle({{63.623422384262085,15.199962258338928}, {61.84849739074707,6.990198791027069}, {63.82547616958618,6.970824301242828}}),mapFloor::triangle({{70.587557554245,12.7971813082695}, {62.41549849510193,15.151576697826385}, {62.35601305961609,12.76213824748993}}),mapFloor::triangle({{68.70834827423096,15.179626643657684}, {70.48789858818054,7.950266450643539}, {70.64704298973083,15.186618268489838}}),
mapFloor::triangle({{77.56028175354004,6.39900267124176}, {69.50210332870483,10.082194954156876}, {69.05787587165833,8.155674487352371}}),mapFloor::triangle({{78.00450921058655,8.325523138046265}, {80.65778017044067,6.190067902207375}, {80.67271113395691,8.16708579659462}}),mapFloor::triangle({{18.01994889974594,31.62578046321869}, {17.901860177516937,29.763782024383545}, {19.049745798110962,30.826357007026672}}),
mapFloor::triangle({{21.60133123397827,34.057825803756714}, {21.2141215801239,32.829880714416504}, {22.419126331806183,33.27840268611908}}),mapFloor::triangle({{20.551060140132904,33.344608545303345}, {19.049745798110962,30.826357007026672}, {21.2141215801239,32.829880714416504}}),mapFloor::triangle({{19.049745798110962,30.826357007026672}, {22.76187092065811,26.71511173248291}, {23.357853293418884,28.123894333839417}}),
mapFloor::triangle({{22.65160232782364,34.77104604244232}, {22.419126331806183,33.27840268611908}, {23.875898122787476,33.820635080337524}}),mapFloor::triangle({{23.875898122787476,33.820635080337524}, {26.815837621688843,30.579254031181335}, {28.817039728164673,30.789822340011597}}),mapFloor::triangle({{22.76187092065811,26.71511173248291}, {26.815837621688843,30.579254031181335}, {23.357853293418884,28.123894333839417}}),
mapFloor::triangle({{28.817039728164673,30.789822340011597}, {27.79485285282135,25.961998105049133}, {33.85002315044403,30.03670871257782}}),mapFloor::triangle({{19.379572570323944,60.57247519493103}, {20.223911106586456,63.55483531951904}, {20.746755599975586,60.57247519493103}}),mapFloor::triangle({{19.379572570323944,59.46836471557617}, {20.940320193767548,58.74128341674805}, {19.379572570323944,58.74128341674805}}),
mapFloor::triangle({{20.746755599975586,60.57247519493103}, {24.558421969413757,63.55483531951904}, {24.558421969413757,60.57247519493103}}),mapFloor::triangle({{20.940320193767548,59.46836471557617}, {24.558421969413757,60.57247519493103}, {24.558421969413757,59.46836471557617}}),mapFloor::triangle({{19.379572570323944,59.46836471557617}, {20.746755599975586,60.57247519493103}, {20.940320193767548,59.46836471557617}}),
mapFloor::triangle({{36.699843406677246,63.55483531951904}, {40.90569615364075,59.01045799255371}, {35.45660376548767,60.14348864555359}}),mapFloor::triangle({{24.558421969413757,60.57247519493103}, {30.007514357566833,63.55483531951904}, {30.007511377334595,59.43850874900818}}),mapFloor::triangle({{30.007514357566833,63.55483531951904}, {35.45660376548767,60.14348864555359}, {30.007511377334595,59.43850874900818}}),
mapFloor::triangle({{24.859899282455444,69.87454295158386}, {30.007514357566833,72.1498429775238}, {29.706037044525146,69.87453699111938}}),mapFloor::triangle({{24.558421969413757,63.55483531951904}, {29.706037044525146,66.45466685295105}, {30.007514357566833,63.55483531951904}}),mapFloor::triangle({{24.859899282455444,66.45466685295105}, {29.706037044525146,69.87453699111938}, {29.706037044525146,66.45466685295105}}),
mapFloor::triangle({{24.859899282455444,66.45466685295105}, {21.64473533630371,69.87454295158386}, {24.859899282455444,69.87454295158386}}),mapFloor::triangle({{29.706037044525146,69.87453699111938}, {33.24161469936371,66.45466685295105}, {29.706037044525146,66.45466685295105}}),mapFloor::triangle({{36.699843406677246,68.74446868896484}, {40.90569615364075,72.09119200706482}, {40.90569615364075,68.74446868896484}}),
mapFloor::triangle({{36.699843406677246,63.55483531951904}, {40.90569615364075,68.74446868896484}, {40.90569615364075,63.55483531951904}}),mapFloor::triangle({{36.699843406677246,68.74446868896484}, {35.04533767700195,72.09119200706482}, {36.699843406677246,72.09119200706482}}),mapFloor::triangle({{9.493985772132874,87.54488825798035}, {11.433076113462448,86.96985840797424}, {9.016329050064087,84.67978239059448}}),
mapFloor::triangle({{9.155822545289993,88.33597302436829}, {11.433076113462448,89.45897221565247}, {11.433076113462448,86.96985840797424}}),mapFloor::triangle({{9.191329032182693,90.99655747413635}, {11.433076113462448,89.45897221565247}, {9.155822545289993,88.33597302436829}}),mapFloor::triangle({{4.940975829958916,94.11181807518005}, {9.191329032182693,90.99655747413635}, {7.529071718454361,89.67112302780151}}),
mapFloor::triangle({{-0.1642280607484281,96.52124047279358}, {1.0736546479165554,98.36049675941467}, {4.940975829958916,94.11181807518005}}),mapFloor::triangle({{10.387811809778214,91.95059537887573}, {12.828822433948517,90.27634263038635}, {11.433076113462448,89.45897221565247}}),mapFloor::triangle({{6.137458980083466,95.06585597991943}, {10.387811809778214,91.95059537887573}, {9.191329032182693,90.99655747413635}}),
mapFloor::triangle({{1.0736546479165554,98.36049675941467}, {-0.1642280607484281,96.52124047279358}, {-0.16422824701294303,100.19974708557129}}),mapFloor::triangle({{1.0736546479165554,98.36049675941467}, {1.964675448834896,99.68438148498535}, {6.137458980083466,95.06585597991943}}),mapFloor::triangle({{15.539567172527313,84.572833776474}, {16.965650022029877,83.8158905506134}, {13.974820077419281,81.61145448684692}}),
mapFloor::triangle({{11.433076113462448,86.96985840797424}, {12.42564395070076,86.40402555465698}, {9.905903786420822,84.12930965423584}}),mapFloor::triangle({{12.42564395070076,86.40402555465698}, {15.539567172527313,84.572833776474}, {11.450512707233429,83.17350149154663}}),mapFloor::triangle({{14.369134604930878,89.00465965270996}, {16.202442348003387,87.87553310394287}, {14.80610966682434,86.75195574760437}}),
mapFloor::triangle({{12.42564395070076,86.40402555465698}, {12.754744291305542,86.8861973285675}, {15.475788712501526,85.68897247314453}}),mapFloor::triangle({{14.80610966682434,86.75195574760437}, {15.475788712501526,85.68897247314453}, {12.754744291305542,86.8861973285675}}),mapFloor::triangle({{9.678222239017487,11.657509207725525}, {16.658097505569458,11.932969838380814}, {16.658097505569458,9.499693661928177}}),
mapFloor::triangle({{16.658097505569458,9.499693661928177}, {16.658097505569458,11.932969838380814}, {17.23211705684662,11.114884167909622}}),mapFloor::triangle({{17.23211705684662,11.114884167909622}, {22.462068498134613,12.314189970493317}, {22.462068498134613,11.184839904308319}}),mapFloor::triangle({{22.462068498134613,12.314189970493317}, {38.310506939888,13.232937455177307}, {36.997660994529724,11.298520117998123}}),
mapFloor::triangle({{36.997660994529724,11.298520117998123}, {38.310506939888,13.232937455177307}, {41.65359437465668,9.228577464818954}}),mapFloor::triangle({{41.65359437465668,9.228577464818954}, {51.104170083999634,9.228577464818954}, {47.48260676860809,7.8788042068481445}}),mapFloor::triangle({{47.48260676860809,7.8788042068481445}, {51.104170083999634,9.228577464818954}, {51.104170083999634,6.25622496008873}}),
mapFloor::triangle({{38.310506939888,12.965062260627747}, {38.77343535423279,19.218647480010986}, {36.88574135303497,19.282427430152893}}),mapFloor::triangle({{38.630759716033936,19.098614156246185}, {38.60505521297455,18.238861858844757}, {46.08475863933563,18.576307594776154}}),mapFloor::triangle({{40.926241874694824,19.693732261657715}, {50.41669011116028,19.785572588443756}, {50.36156177520752,20.643940567970276}}),
mapFloor::triangle({{42.68245995044708,19.802047312259674}, {42.73758828639984,18.47144365310669}, {50.45495629310608,18.620842695236206}}),mapFloor::triangle({{52.181243896484375,19.982948899269104}, {50.45495629310608,18.620842695236206}, {58.32059383392334,17.525197565555573}}),mapFloor::triangle({{57.51412510871887,18.802034854888916}, {53.711336851119995,12.731629610061646}, {55.09960055351257,12.681028246879578}}),
mapFloor::triangle({{59.49323773384094,17.0124351978302}, {59.82276797294617,21.030567586421967}, {58.02296996116638,21.0317000746727}}),mapFloor::triangle({{64.58166837692261,17.44939535856247}, {66.38146638870239,17.448262870311737}, {66.0962700843811,22.061438858509064}}),mapFloor::triangle({{65.02983570098877,18.457326292991638}, {64.95534181594849,19.820663332939148}, {59.11948084831238,19.491136074066162}}),
mapFloor::triangle({{55.09960055351257,12.681028246879578}, {53.711336851119995,12.731629610061646}, {54.15341854095459,6.743111461400986}}),mapFloor::triangle({{53.654247522354126,8.948808163404465}, {63.128286600112915,8.935161679983139}, {63.104087114334106,6.958241015672684}}),mapFloor::triangle({{63.623422384262085,15.199962258338928}, {61.646443605422974,15.219336748123169}, {61.84849739074707,6.990198791027069}}),
mapFloor::triangle({{70.587557554245,12.7971813082695}, {70.64704298973083,15.186618268489838}, {62.41549849510193,15.151576697826385}}),mapFloor::triangle({{68.70834827423096,15.179626643657684}, {68.51587891578674,8.026860654354095}, {70.48789858818054,7.950266450643539}}),mapFloor::triangle({{77.56028175354004,6.39900267124176}, {78.00450921058655,8.325523138046265}, {69.50210332870483,10.082194954156876}}),
mapFloor::triangle({{78.00450921058655,8.325523138046265}, {77.56028175354004,6.39900267124176}, {80.65778017044067,6.190067902207375}}),mapFloor::triangle({{18.01994889974594,31.62578046321869}, {16.6775643825531,30.714192986488342}, {17.901860177516937,29.763782024383545}}),mapFloor::triangle({{21.60133123397827,34.057825803756714}, {20.551060140132904,33.344608545303345}, {21.2141215801239,32.829880714416504}}),
mapFloor::triangle({{20.551060140132904,33.344608545303345}, {18.01994889974594,31.62578046321869}, {19.049745798110962,30.826357007026672}}),mapFloor::triangle({{19.049745798110962,30.826357007026672}, {17.901860177516937,29.763782024383545}, {22.76187092065811,26.71511173248291}}),mapFloor::triangle({{22.65160232782364,34.77104604244232}, {21.60133123397827,34.057825803756714}, {22.419126331806183,33.27840268611908}}),
mapFloor::triangle({{23.875898122787476,33.820635080337524}, {22.419126331806183,33.27840268611908}, {26.815837621688843,30.579254031181335}}),mapFloor::triangle({{22.76187092065811,26.71511173248291}, {28.817039728164673,30.789822340011597}, {26.815837621688843,30.579254031181335}}),mapFloor::triangle({{28.817039728164673,30.789822340011597}, {22.76187092065811,26.71511173248291}, {27.79485285282135,25.961998105049133}}),
mapFloor::triangle({{19.379572570323944,60.57247519493103}, {19.379572570323944,63.55483531951904}, {20.223911106586456,63.55483531951904}}),mapFloor::triangle({{19.379572570323944,59.46836471557617}, {20.940320193767548,59.46836471557617}, {20.940320193767548,58.74128341674805}}),mapFloor::triangle({{20.746755599975586,60.57247519493103}, {20.223911106586456,63.55483531951904}, {24.558421969413757,63.55483531951904}}),
mapFloor::triangle({{20.940320193767548,59.46836471557617}, {20.746755599975586,60.57247519493103}, {24.558421969413757,60.57247519493103}}),mapFloor::triangle({{19.379572570323944,59.46836471557617}, {19.379572570323944,60.57247519493103}, {20.746755599975586,60.57247519493103}}),mapFloor::triangle({{36.699843406677246,63.55483531951904}, {40.90569615364075,63.55483531951904}, {40.90569615364075,59.01045799255371}}),
mapFloor::triangle({{24.558421969413757,60.57247519493103}, {24.558421969413757,63.55483531951904}, {30.007514357566833,63.55483531951904}}),mapFloor::triangle({{30.007514357566833,63.55483531951904}, {36.699843406677246,63.55483531951904}, {35.45660376548767,60.14348864555359}}),mapFloor::triangle({{24.859899282455444,69.87454295158386}, {24.558421969413757,72.1498429775238}, {30.007514357566833,72.1498429775238}}),
mapFloor::triangle({{24.558421969413757,63.55483531951904}, {24.859899282455444,66.45466685295105}, {29.706037044525146,66.45466685295105}}),mapFloor::triangle({{24.859899282455444,66.45466685295105}, {24.859899282455444,69.87454295158386}, {29.706037044525146,69.87453699111938}}),mapFloor::triangle({{24.859899282455444,66.45466685295105}, {21.64473533630371,66.45466685295105}, {21.64473533630371,69.87454295158386}}),
mapFloor::triangle({{29.706037044525146,69.87453699111938}, {33.24161469936371,69.87453699111938}, {33.24161469936371,66.45466685295105}}),mapFloor::triangle({{36.699843406677246,68.74446868896484}, {36.699843406677246,72.09119200706482}, {40.90569615364075,72.09119200706482}}),mapFloor::triangle({{36.699843406677246,63.55483531951904}, {36.699843406677246,68.74446868896484}, {40.90569615364075,68.74446868896484}}),
mapFloor::triangle({{36.699843406677246,68.74446868896484}, {35.04533767700195,68.74446868896484}, {35.04533767700195,72.09119200706482}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
					mapFloor("Stone", List<mapFloor::triangle>({mapFloor::triangle({{17.72996336221695,85.88326573371887}, {17.101339995861053,84.22081470489502}, {18.56410950422287,85.23366451263428}}),mapFloor::triangle({{18.56410950422287,85.23366451263428}, {18.054762482643127,86.10472083091736}, {17.72996336221695,85.88326573371887}}),mapFloor::triangle({{16.28933995962143,84.77445244789124}, {16.87391847372055,83.97787809371948}, {17.101339995861053,84.22081470489502}}),
mapFloor::triangle({{17.471599578857422,86.05304956436157}, {16.28933995962143,84.77445244789124}, {17.72996336221695,85.88326573371887}}),mapFloor::triangle({{14.916321635246277,86.57561540603638}, {17.471599578857422,86.05304956436157}, {16.312654316425323,87.69919276237488}}),mapFloor::triangle({{86.2735390663147,6.080254167318344}, {91.52420163154602,9.833603352308273}, {91.00373387336731,6.080254539847374}}),
mapFloor::triangle({{86.2735390663147,6.080254167318344}, {91.00373387336731,6.080254539847374}, {86.46487593650818,5.293634533882141}}),mapFloor::triangle({{86.46487593650818,5.293634533882141}, {91.00373387336731,6.080254539847374}, {87.27275729179382,4.230634868144989}}),mapFloor::triangle({{88.66528868675232,3.433385118842125}, {87.27275729179382,4.230634868144989}, {91.00373387336731,6.080254539847374}}),
mapFloor::triangle({{89.7495448589325,3.1144849956035614}, {88.66528868675232,3.433385118842125}, {91.00373387336731,6.080254539847374}}),mapFloor::triangle({{90.96136093139648,3.0081847682595253}, {89.7495448589325,3.1144849956035614}, {91.00373387336731,6.080254539847374}}),mapFloor::triangle({{83.38517546653748,9.833603352308273}, {80.68661689758301,6.080253794789314}, {80.68661689758301,9.833603352308273}}),
mapFloor::triangle({{83.38517546653748,9.833603352308273}, {91.52420163154602,17.764165997505188}, {91.52420163154602,9.833603352308273}}),mapFloor::triangle({{95.66583633422852,9.833603352308273}, {96.34740352630615,17.764165997505188}, {99.80747699737549,9.833603352308273}}),mapFloor::triangle({{95.66583633422852,9.833603352308273}, {99.80747699737549,5.819829553365707}, {95.20300626754761,5.896829813718796}}),
mapFloor::triangle({{95.66583633422852,9.833603352308273}, {94.52934265136719,7.843989133834839}, {93.59501600265503,9.833603352308273}}),mapFloor::triangle({{93.59501600265503,9.833603352308273}, {93.93579959869385,17.764165997505188}, {95.66583633422852,9.833603352308273}}),mapFloor::triangle({{91.52420163154602,9.833603352308273}, {92.7299976348877,17.764165997505188}, {93.59501600265503,9.833603352308273}}),
mapFloor::triangle({{93.59501600265503,9.833603352308273}, {91.52420163154602,7.75747150182724}, {91.52420163154602,9.833603352308273}}),mapFloor::triangle({{2.4656306952238083,10.347917675971985}, {9.71275269985199,6.0802508145570755}, {9.725511819124222,10.347918421030045}}),mapFloor::triangle({{0.03111714031547308,10.347917675971985}, {2.4624263867735863,6.080250442028046}, {2.4656306952238083,10.347917675971985}}),
mapFloor::triangle({{9.725511819124222,10.347918421030045}, {2.4656305089592934,15.986970067024231}, {2.4656306952238083,10.347917675971985}}),mapFloor::triangle({{9.066378325223923,44.62483525276184}, {6.107847020030022,38.36375176906586}, {3.0923396348953247,40.56798219680786}}),mapFloor::triangle({{15.067537128925323,40.259817242622375}, {16.6775643825531,30.714192986488342}, {9.093499183654785,36.202964186668396}}),
mapFloor::triangle({{9.541754424571991,37.63674199581146}, {15.067537128925323,40.259817242622375}, {9.093499183654785,36.202964186668396}}),mapFloor::triangle({{12.828297913074493,41.88040792942047}, {9.072321653366089,38.36503326892853}, {8.048928529024124,38.71713876724243}}),mapFloor::triangle({{12.08188533782959,42.420604825019836}, {8.048928529024124,38.71713876724243}, {6.107847020030022,38.36375176906586}}),
mapFloor::triangle({{13.574710488319397,41.340211033821106}, {9.541754424571991,37.63674199581146}, {9.072321653366089,38.36503326892853}}),mapFloor::triangle({{49.66898262500763,46.49096429347992}, {62.25937604904175,50.85013508796692}, {49.66898262500763,50.85013508796692}}),mapFloor::triangle({{62.25937604904175,50.85013508796692}, {70.59863805770874,48.125749826431274}, {70.59863805770874,52.48492360115051}}),
mapFloor::triangle({{32.40371346473694,46.49096429347992}, {45.04704177379608,50.85013508796692}, {32.40371346473694,50.85013508796692}}),mapFloor::triangle({{45.04704475402832,46.49096429347992}, {49.66898262500763,50.85013508796692}, {45.04704177379608,50.85013508796692}}),mapFloor::triangle({{45.04704475402832,46.49096429347992}, {49.66898262500763,44.94660794734955}, {49.66898262500763,46.49096429347992}}),
mapFloor::triangle({{19.379572570323944,58.74128341674805}, {10.97736731171608,62.00824975967407}, {10.97736731171608,57.194697856903076}}),mapFloor::triangle({{35.04533767700195,72.09119200706482}, {40.90569615364075,76.69534683227539}, {40.90569615364075,72.09119200706482}}),mapFloor::triangle({{35.43303310871124,91.61649346351624}, {37.24607825279236,98.77073168754578}, {41.29943549633026,95.77224850654602}}),
mapFloor::triangle({{28.730875253677368,84.56528782844543}, {31.324192881584167,94.56550478935242}, {35.43303310871124,91.61649346351624}}),mapFloor::triangle({{35.43303310871124,91.61649346351624}, {43.20673644542694,94.41756010055542}, {43.05960834026337,86.90773844718933}}),mapFloor::triangle({{43.20673644542694,94.41756010055542}, {48.93642067909241,86.90773844718933}, {43.05960834026337,86.90773844718933}}),
mapFloor::triangle({{48.93642067909241,96.57682776451111}, {43.20673644542694,99.99765753746033}, {51.30906701087952,99.99765753746033}}),mapFloor::triangle({{51.30906701087952,99.99765753746033}, {73.08756709098816,96.57682776451111}, {48.93642067909241,96.57682776451111}}),mapFloor::triangle({{47.851353883743286,81.23798370361328}, {45.79227268695831,78.1296968460083}, {45.79600989818573,81.23798370361328}}),
mapFloor::triangle({{45.803022384643555,87.06831336021423}, {47.84761369228363,84.34626460075378}, {45.79974710941315,84.34626460075378}}),mapFloor::triangle({{47.84761369228363,84.34626460075378}, {48.92699122428894,87.06831336021423}, {48.93026351928711,84.34626460075378}}),mapFloor::triangle({{47.84761369228363,84.34626460075378}, {45.79600989818573,81.23798370361328}, {45.79974710941315,84.34626460075378}}),
mapFloor::triangle({{45.79227268695831,78.1296968460083}, {43.99542510509491,81.23798370361328}, {45.79600989818573,81.23798370361328}}),mapFloor::triangle({{48.60720634460449,78.1296968460083}, {46.560993790626526,76.0650634765625}, {45.79227268695831,78.1296968460083}}),mapFloor::triangle({{45.79227268695831,78.1296968460083}, {46.560993790626526,76.0650634765625}, {43.99168789386749,78.1296968460083}}),
mapFloor::triangle({{49.326759576797485,76.11454129219055}, {47.00327515602112,74.98186230659485}, {46.560993790626526,76.0650634765625}}),mapFloor::triangle({{49.13387894630432,73.9694356918335}, {47.16263711452484,72.88362979888916}, {47.08295464515686,73.93274307250977}}),mapFloor::triangle({{45.39772272109985,73.75410795211792}, {47.08295464515686,73.93274307250977}, {45.514652132987976,73.35906028747559}}),
mapFloor::triangle({{45.514652132987976,73.35906028747559}, {47.16263711452484,72.88362979888916}, {45.6315815448761,72.96401262283325}}),mapFloor::triangle({{49.05419647693634,75.0185489654541}, {47.08295464515686,73.93274307250977}, {47.00327515602112,74.98186230659485}}),mapFloor::triangle({{49.21355843544006,72.92031645774841}, {50.55335760116577,73.32572937011719}, {50.89653730392456,72.33113646507263}}),
mapFloor::triangle({{49.21355843544006,72.92031645774841}, {47.7464884519577,71.74179553985596}, {47.16263711452484,72.88362979888916}}),mapFloor::triangle({{49.797409772872925,71.77848219871521}, {48.646149039268494,69.77917551994324}, {47.7464884519577,71.74179553985596}}),mapFloor::triangle({{51.05051398277283,69.23014521598816}, {51.20006203651428,71.84521555900574}, {52.0486056804657,69.73766088485718}}),
mapFloor::triangle({{50.89653730392456,72.33113646507263}, {49.797409772872925,71.77848219871521}, {49.21355843544006,72.92031645774841}}),mapFloor::triangle({{48.646149039268494,69.77917551994324}, {50.72752237319946,68.83641481399536}, {47.755202651023865,68.31777095794678}}),mapFloor::triangle({{50.72752237319946,68.83641481399536}, {49.753642082214355,67.07406044006348}, {47.755202651023865,68.31777095794678}}),
mapFloor::triangle({{52.0486056804657,69.73766088485718}, {50.72752237319946,68.83641481399536}, {51.05051398277283,69.23014521598816}}),mapFloor::triangle({{56.09090328216553,67.50091910362244}, {55.204540491104126,63.55739235877991}, {51.89011096954346,64.87532258033752}}),mapFloor::triangle({{53.366875648498535,68.47110390663147}, {51.89011096954346,64.87532258033752}, {49.753642082214355,67.07406044006348}}),
mapFloor::triangle({{65.15764594078064,69.70243453979492}, {62.13876008987427,66.3451075553894}, {62.30032444000244,69.8310136795044}}),mapFloor::triangle({{62.30032444000244,69.8310136795044}, {59.11988019943237,66.73123836517334}, {59.44300293922424,69.95959281921387}}),mapFloor::triangle({{59.4298779964447,62.56228685379028}, {59.11988019943237,66.73123836517334}, {62.13876008987427,66.3451075553894}}),
mapFloor::triangle({{65.15764594078064,62.21552491188049}, {62.13876008987427,66.3451075553894}, {65.15764594078064,65.95897674560547}}),mapFloor::triangle({{65.15764594078064,62.21552491188049}, {67.598956823349,64.82126116752625}, {67.598956823349,62.21552491188049}}),mapFloor::triangle({{67.598956823349,64.82126116752625}, {65.15764594078064,65.95897674560547}, {67.59851574897766,66.10592603683472}}),
mapFloor::triangle({{67.598956823349,62.21552491188049}, {76.47035717964172,64.54488635063171}, {78.67330312728882,61.93915009498596}}),mapFloor::triangle({{78.67330312728882,61.93915009498596}, {81.86501860618591,70.36314010620117}, {86.01043224334717,67.75740385055542}}),mapFloor::triangle({{91.97462201118469,71.907639503479}, {92.78821349143982,77.96415090560913}, {96.93363308906555,75.35841464996338}}),
mapFloor::triangle({{86.01043224334717,67.75740385055542}, {87.82920837402344,74.51337575912476}, {91.97462201118469,71.907639503479}}),mapFloor::triangle({{81.86501860618591,74.69258904457092}, {87.82920837402344,95.43773531913757}, {87.82920837402344,74.51337575912476}}),mapFloor::triangle({{87.82920837402344,97.71090745925903}, {84.48696732521057,99.98407363891602}, {87.82920837402344,99.98407363891602}}),
mapFloor::triangle({{87.82920837402344,95.43773531913757}, {83.17599296569824,96.21781706809998}, {87.82920837402344,97.71090745925903}}),mapFloor::triangle({{81.86501860618591,74.69258904457092}, {75.78132748603821,71.60633206367493}, {75.78132748603821,75.93578100204468}}),mapFloor::triangle({{81.86501860618591,74.69258904457092}, {87.82920837402344,74.51337575912476}, {81.86501860618591,70.36314010620117}}),
mapFloor::triangle({{75.78132748603821,75.93578100204468}, {71.89305424690247,71.52698040008545}, {69.962477684021,75.8564293384552}}),mapFloor::triangle({{75.78132748603821,75.93578100204468}, {69.962477684021,86.2011730670929}, {75.78132748603821,86.28052473068237}}),mapFloor::triangle({{65.86300134658813,70.60616612434387}, {67.59851574897766,66.10592603683472}, {65.15764594078064,69.70243453979492}}),
mapFloor::triangle({{65.15764594078064,65.95897674560547}, {65.15764594078064,69.70243453979492}, {67.59851574897766,66.10592603683472}}),mapFloor::triangle({{65.86300134658813,70.60616612434387}, {71.522057056427,71.02137207984924}, {70.37585377693176,68.00156831741333}}),mapFloor::triangle({{65.46623706817627,71.28947973251343}, {69.93500590324402,73.60032796859741}, {71.522057056427,71.02137207984924}}),
mapFloor::triangle({{64.55667018890381,75.65026879310608}, {64.03348445892334,71.81849479675293}, {61.344319581985474,71.6862440109253}}),mapFloor::triangle({{56.754493713378906,70.60816287994385}, {52.612632513046265,74.4379460811615}, {56.754493713378906,74.57218766212463}}),mapFloor::triangle({{60.33329367637634,71.16384506225586}, {57.78156518936157,74.60547685623169}, {61.16911768913269,75.63436031341553}}),
mapFloor::triangle({{61.344319581985474,71.6862440109253}, {61.16911768913269,75.63436031341553}, {64.55667018890381,75.65026879310608}}),mapFloor::triangle({{57.78156518936157,70.6414520740509}, {56.754493713378906,74.57218766212463}, {57.78156518936157,74.60547685623169}}),mapFloor::triangle({{56.754493713378906,74.57218766212463}, {52.612632513046265,82.88237452507019}, {56.754493713378906,82.88237452507019}}),
mapFloor::triangle({{17.72996336221695,85.88326573371887}, {16.28933995962143,84.77445244789124}, {17.101339995861053,84.22081470489502}}),mapFloor::triangle({{18.56410950422287,85.23366451263428}, {19.42778080701828,85.1303219795227}, {18.054762482643127,86.10472083091736}}),mapFloor::triangle({{16.28933995962143,84.77445244789124}, {16.01479798555374,84.58924889564514}, {16.87391847372055,83.97787809371948}}),
mapFloor::triangle({{17.471599578857422,86.05304956436157}, {16.075266897678375,84.92947220802307}, {16.28933995962143,84.77445244789124}}),mapFloor::triangle({{14.916321635246277,86.57561540603638}, {16.075266897678375,84.92947220802307}, {17.471599578857422,86.05304956436157}}),mapFloor::triangle({{86.2735390663147,6.080254167318344}, {83.38517546653748,9.833603352308273}, {91.52420163154602,9.833603352308273}}),
mapFloor::triangle({{83.38517546653748,9.833603352308273}, {86.2735390663147,6.080254167318344}, {80.68661689758301,6.080253794789314}}),mapFloor::triangle({{83.38517546653748,9.833603352308273}, {83.38517546653748,17.764165997505188}, {91.52420163154602,17.764165997505188}}),mapFloor::triangle({{95.66583633422852,9.833603352308273}, {93.93579959869385,17.764165997505188}, {96.34740352630615,17.764165997505188}}),
mapFloor::triangle({{95.66583633422852,9.833603352308273}, {99.80747699737549,9.833603352308273}, {99.80747699737549,5.819829553365707}}),mapFloor::triangle({{95.66583633422852,9.833603352308273}, {95.20300626754761,5.896829813718796}, {94.52934265136719,7.843989133834839}}),mapFloor::triangle({{93.59501600265503,9.833603352308273}, {92.7299976348877,17.764165997505188}, {93.93579959869385,17.764165997505188}}),
mapFloor::triangle({{91.52420163154602,9.833603352308273}, {91.52420163154602,17.764165997505188}, {92.7299976348877,17.764165997505188}}),mapFloor::triangle({{93.59501600265503,9.833603352308273}, {94.52934265136719,7.843989133834839}, {91.52420163154602,7.75747150182724}}),mapFloor::triangle({{2.4656306952238083,10.347917675971985}, {2.4624263867735863,6.080250442028046}, {9.71275269985199,6.0802508145570755}}),
mapFloor::triangle({{0.03111714031547308,10.347917675971985}, {0.031117163598537445,6.080250442028046}, {2.4624263867735863,6.080250442028046}}),mapFloor::triangle({{9.725511819124222,10.347918421030045}, {9.725511819124222,15.986970067024231}, {2.4656305089592934,15.986970067024231}}),mapFloor::triangle({{9.066378325223923,44.62483525276184}, {12.08188533782959,42.420604825019836}, {6.107847020030022,38.36375176906586}}),
mapFloor::triangle({{15.067537128925323,40.259817242622375}, {22.65160232782364,34.77104604244232}, {16.6775643825531,30.714192986488342}}),mapFloor::triangle({{9.541754424571991,37.63674199581146}, {14.32112455368042,40.80001413822174}, {15.067537128925323,40.259817242622375}}),mapFloor::triangle({{12.828297913074493,41.88040792942047}, {13.574710488319397,41.340211033821106}, {9.072321653366089,38.36503326892853}}),
mapFloor::triangle({{12.08188533782959,42.420604825019836}, {12.828297913074493,41.88040792942047}, {8.048928529024124,38.71713876724243}}),mapFloor::triangle({{13.574710488319397,41.340211033821106}, {14.32112455368042,40.80001413822174}, {9.541754424571991,37.63674199581146}}),mapFloor::triangle({{49.66898262500763,46.49096429347992}, {62.25937604904175,46.49096429347992}, {62.25937604904175,50.85013508796692}}),
mapFloor::triangle({{62.25937604904175,50.85013508796692}, {62.25937604904175,46.49096429347992}, {70.59863805770874,48.125749826431274}}),mapFloor::triangle({{32.40371346473694,46.49096429347992}, {45.04704475402832,46.49096429347992}, {45.04704177379608,50.85013508796692}}),mapFloor::triangle({{45.04704475402832,46.49096429347992}, {49.66898262500763,46.49096429347992}, {49.66898262500763,50.85013508796692}}),
mapFloor::triangle({{45.04704475402832,46.49096429347992}, {45.04704475402832,44.94660794734955}, {49.66898262500763,44.94660794734955}}),mapFloor::triangle({{19.379572570323944,58.74128341674805}, {19.379572570323944,63.55483531951904}, {10.97736731171608,62.00824975967407}}),mapFloor::triangle({{35.04533767700195,72.09119200706482}, {35.04533767700195,76.69534683227539}, {40.90569615364075,76.69534683227539}}),
mapFloor::triangle({{35.43303310871124,91.61649346351624}, {31.324192881584167,94.56550478935242}, {37.24607825279236,98.77073168754578}}),mapFloor::triangle({{28.730875253677368,84.56528782844543}, {24.549689888954163,87.44979500770569}, {31.324192881584167,94.56550478935242}}),mapFloor::triangle({{35.43303310871124,91.61649346351624}, {41.29943549633026,95.77224850654602}, {43.20673644542694,94.41756010055542}}),
mapFloor::triangle({{43.20673644542694,94.41756010055542}, {48.93642067909241,96.57682776451111}, {48.93642067909241,86.90773844718933}}),mapFloor::triangle({{48.93642067909241,96.57682776451111}, {43.20673644542694,94.41756010055542}, {43.20673644542694,99.99765753746033}}),mapFloor::triangle({{51.30906701087952,99.99765753746033}, {73.08756709098816,99.99765753746033}, {73.08756709098816,96.57682776451111}}),
mapFloor::triangle({{47.851353883743286,81.23798370361328}, {48.60720634460449,78.1296968460083}, {45.79227268695831,78.1296968460083}}),mapFloor::triangle({{45.803022384643555,87.06831336021423}, {47.84434139728546,87.06831336021423}, {47.84761369228363,84.34626460075378}}),mapFloor::triangle({{47.84761369228363,84.34626460075378}, {47.84434139728546,87.06831336021423}, {48.92699122428894,87.06831336021423}}),
mapFloor::triangle({{47.84761369228363,84.34626460075378}, {47.851353883743286,81.23798370361328}, {45.79600989818573,81.23798370361328}}),mapFloor::triangle({{45.79227268695831,78.1296968460083}, {43.99168789386749,78.1296968460083}, {43.99542510509491,81.23798370361328}}),mapFloor::triangle({{48.60720634460449,78.1296968460083}, {49.326759576797485,76.11454129219055}, {46.560993790626526,76.0650634765625}}),
mapFloor::triangle({{49.326759576797485,76.11454129219055}, {49.05419647693634,75.0185489654541}, {47.00327515602112,74.98186230659485}}),mapFloor::triangle({{49.13387894630432,73.9694356918335}, {49.21355843544006,72.92031645774841}, {47.16263711452484,72.88362979888916}}),mapFloor::triangle({{45.39772272109985,73.75410795211792}, {47.00327515602112,74.98186230659485}, {47.08295464515686,73.93274307250977}}),
mapFloor::triangle({{45.514652132987976,73.35906028747559}, {47.08295464515686,73.93274307250977}, {47.16263711452484,72.88362979888916}}),mapFloor::triangle({{49.05419647693634,75.0185489654541}, {49.13387894630432,73.9694356918335}, {47.08295464515686,73.93274307250977}}),mapFloor::triangle({{49.21355843544006,72.92031645774841}, {49.13387894630432,73.9694356918335}, {50.55335760116577,73.32572937011719}}),
mapFloor::triangle({{49.21355843544006,72.92031645774841}, {49.797409772872925,71.77848219871521}, {47.7464884519577,71.74179553985596}}),mapFloor::triangle({{49.797409772872925,71.77848219871521}, {51.05051398277283,69.23014521598816}, {48.646149039268494,69.77917551994324}}),mapFloor::triangle({{51.05051398277283,69.23014521598816}, {49.797409772872925,71.77848219871521}, {51.20006203651428,71.84521555900574}}),
mapFloor::triangle({{50.89653730392456,72.33113646507263}, {51.20006203651428,71.84521555900574}, {49.797409772872925,71.77848219871521}}),mapFloor::triangle({{48.646149039268494,69.77917551994324}, {51.05051398277283,69.23014521598816}, {50.72752237319946,68.83641481399536}}),mapFloor::triangle({{50.72752237319946,68.83641481399536}, {53.366875648498535,68.47110390663147}, {49.753642082214355,67.07406044006348}}),
mapFloor::triangle({{52.0486056804657,69.73766088485718}, {53.366875648498535,68.47110390663147}, {50.72752237319946,68.83641481399536}}),mapFloor::triangle({{56.09090328216553,67.50091910362244}, {59.11988019943237,66.73123836517334}, {55.204540491104126,63.55739235877991}}),mapFloor::triangle({{53.366875648498535,68.47110390663147}, {56.09090328216553,67.50091910362244}, {51.89011096954346,64.87532258033752}}),
mapFloor::triangle({{65.15764594078064,69.70243453979492}, {65.15764594078064,65.95897674560547}, {62.13876008987427,66.3451075553894}}),mapFloor::triangle({{62.30032444000244,69.8310136795044}, {62.13876008987427,66.3451075553894}, {59.11988019943237,66.73123836517334}}),mapFloor::triangle({{59.4298779964447,62.56228685379028}, {55.204540491104126,63.55739235877991}, {59.11988019943237,66.73123836517334}}),
mapFloor::triangle({{65.15764594078064,62.21552491188049}, {59.4298779964447,62.56228685379028}, {62.13876008987427,66.3451075553894}}),mapFloor::triangle({{65.15764594078064,62.21552491188049}, {65.15764594078064,65.95897674560547}, {67.598956823349,64.82126116752625}}),mapFloor::triangle({{67.598956823349,62.21552491188049}, {67.598956823349,64.82126116752625}, {76.47035717964172,64.54488635063171}}),
mapFloor::triangle({{78.67330312728882,61.93915009498596}, {76.47035717964172,64.54488635063171}, {81.86501860618591,70.36314010620117}}),mapFloor::triangle({{91.97462201118469,71.907639503479}, {87.82920837402344,74.51337575912476}, {92.78821349143982,77.96415090560913}}),mapFloor::triangle({{86.01043224334717,67.75740385055542}, {81.86501860618591,70.36314010620117}, {87.82920837402344,74.51337575912476}}),
mapFloor::triangle({{81.86501860618591,74.69258904457092}, {81.86501860618591,95.36430835723877}, {87.82920837402344,95.43773531913757}}),mapFloor::triangle({{87.82920837402344,97.71090745925903}, {83.17599296569824,96.21781706809998}, {84.48696732521057,99.98407363891602}}),mapFloor::triangle({{87.82920837402344,95.43773531913757}, {81.86501860618591,95.36430835723877}, {83.17599296569824,96.21781706809998}}),
mapFloor::triangle({{81.86501860618591,74.69258904457092}, {81.86501860618591,70.36314010620117}, {75.78132748603821,71.60633206367493}}),mapFloor::triangle({{75.78132748603821,75.93578100204468}, {75.78132748603821,71.60633206367493}, {71.89305424690247,71.52698040008545}}),mapFloor::triangle({{75.78132748603821,75.93578100204468}, {69.962477684021,75.8564293384552}, {69.962477684021,86.2011730670929}}),
mapFloor::triangle({{65.86300134658813,70.60616612434387}, {70.37585377693176,68.00156831741333}, {67.59851574897766,66.10592603683472}}),mapFloor::triangle({{65.86300134658813,70.60616612434387}, {65.46623706817627,71.28947973251343}, {71.522057056427,71.02137207984924}}),mapFloor::triangle({{65.46623706817627,71.28947973251343}, {64.03348445892334,71.81849479675293}, {69.93500590324402,73.60032796859741}}),
mapFloor::triangle({{64.55667018890381,75.65026879310608}, {69.93500590324402,73.60032796859741}, {64.03348445892334,71.81849479675293}}),mapFloor::triangle({{56.754493713378906,70.60816287994385}, {52.612632513046265,70.47392129898071}, {52.612632513046265,74.4379460811615}}),mapFloor::triangle({{60.33329367637634,71.16384506225586}, {57.78156518936157,70.6414520740509}, {57.78156518936157,74.60547685623169}}),
mapFloor::triangle({{61.344319581985474,71.6862440109253}, {60.33329367637634,71.16384506225586}, {61.16911768913269,75.63436031341553}}),mapFloor::triangle({{57.78156518936157,70.6414520740509}, {56.754493713378906,70.60816287994385}, {56.754493713378906,74.57218766212463}}),mapFloor::triangle({{56.754493713378906,74.57218766212463}, {52.612632513046265,74.4379460811615}, {52.612632513046265,82.88237452507019}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "CHAPELSTEP"})})), }),
					{}, { 5000,5000 }, Map<string, string>({
							pair<string,string>({"song1",to_string(CHAPEL_WAV_1) + " " + "MusicVolume"}),
							pair<string, string>({ "LoadingScreenImage",to_string(LOADINGSCREEN_1) }),
						}));
		maps["ChapelLeft1"] = mapInstance("ChapelLeft1", CHAPELLEFT1, { 97,54 }, List<mapObject>({
			mapObject("BrazierFire", false, true, false, imageLookup.getSequenceAsString("BrazierFire","ON_FRONT"),"1",90,15,"1.0","1","CENTRE",{46.4, 56.8}, false, {}, Map<string, string>({
					pair<string, string>("don'tLoadIfNot", "BrazierActivated"),
				})),
			mapObject("FireLeftOFF", false, true, false, imageLookup.getSequenceAsString("ChapelLeftFire","OFF_FRONT"),"1",90,0,"1.0","3.3","CENTRE",{8, 46}, false, {}, Map<string, string>({
					pair<string, string>("don'tLoadIf", "FireSpigotLeftActivated"),
				})),
			mapObject("FireLeftON", false, true, false, imageLookup.getSequenceAsString("ChapelLeftFire","ON_FRONT"),"1",90,0,"1.0","3.3","CENTRE",{8, 46}, false, {}, Map<string, string>({
					pair<string, string>("don'tLoadIfNot", "FireSpigotLeftActivated"),
				})),
			mapObject("FireTopLeftOFF", false, true, false, imageLookup.getSequenceAsString("ChapelLeftFire","OFF_FRONT"),"1",90,0,"1.0","3.3","CENTRE",{8, 19}, false, {}, Map<string, string>({
					pair<string, string>("don'tLoadIf", "FireSpigotTopLeftActivated"),
				})),
			mapObject("FireTopLeftON", false, true, false, imageLookup.getSequenceAsString("ChapelLeftFire","ON_FRONT"),"1",90,0,"1.0","3.3","CENTRE",{8, 19}, false, {}, Map<string, string>({
					pair<string, string>("don'tLoadIfNot", "FireSpigotTopLeftActivated"),
				})),
				mapObject("FireTopRightOFF", false, true, false, imageLookup.getSequenceAsString("ChapelLeftFire","OFF_FRONT"),"1",90,0,"1.0","3.3","CENTRE",{92, 5}, false, {}, Map<string, string>({
					pair<string, string>("don'tLoadIf", "FireSpigotTopLeftActivated"),
				})),
			mapObject("FireTopRightON", false, true, false, imageLookup.getSequenceAsString("ChapelLeftFire","ON_FRONT"),"1",90,0,"1.0","3.3","CENTRE",{92, 5}, false, {}, Map<string, string>({
					pair<string, string>("don'tLoadIfNot", "FireSpigotTopLeftActivated"),
				})),
			puzzleContainer.getFireSpigot("FireSpigotLeft", {9.5,52}),
			puzzleContainer.getFireSpigot("FireSpigotTopLeft", {9.5,26.5}),
			puzzleContainer.getFireSpigot("FireSpigotTopRight", {90.5,13.5}),
			puzzleContainer.getFireSpigot("BrokenSpigot", {32,14}),
			mapObject("FilmGrain2", false, true, true, imageLookup.getSequenceAsString("FilmGrain2","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("ChapelLeft1Top", false, true, false, imageLookup.getSequenceAsString("CHAPELLEFT1TOP","STAND_FRONT"),"1",90,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("AT_ChapelLeft2Main", true, false, false, "","0",0,0,"","","CENTRE",{95, 50}, false, List<mapFloor::triangle>({mapFloor::triangle({{100.01071691513062,52.04950571060181}, {97.45782613754272,56.87217116355896}, 
{100.01071691513062,56.87217116355896}}),mapFloor::triangle({{100.01071691513062,52.04950571060181}, {97.45782613754272,52.04950571060181}, {97.45782613754272,56.87217116355896}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Return to Entry Hall"}),
																		pair<string, string>({"copy","AT_ChapelLeft2Main"}),
																		pair<string, string>({"areaTransition","ChapelEntryHall"}),
																		pair<string, string>({"audio","CHAPELMULTISTEP"}),
																		pair<string, string>({"playerPosX","26"}),
																		pair<string, string>({"playerPosY","55"}),
																		pair<string, string>({"direction","STAND_RIGHT"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_GoOutside", true, false, false, "","0",0,0,"","","CENTRE",{58, 7.6}, false, List<mapFloor::triangle>({mapFloor::triangle({{55.55739998817444,10.840822756290436}, {60.3468120098114,9.310831129550934}, {55.55739998817444,9.310830384492874}}),mapFloor::triangle({{55.55739998817444,10.840822756290436}, {60.3468120098114,10.840823501348495}, {60.3468120098114,9.310831129550934}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Outside"}),
																		pair<string, string>({"copy","AT_GoOutside"}),
																		pair<string, string>({"areaTransition","ChapelLeft1"}),
																		pair<string, string>({"audio","CHAPELMULTISTEP"}),
																		pair<string, string>({"playerPosX","50"}),
																		pair<string, string>({"playerPosY","88.8"}),
																		pair<string, string>({"direction","STAND_FRONT"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
			mapObject("AT_GoInside", true, false, false, "","0",0,0,"","","CENTRE",{58, 7.6}, false, List<mapFloor::triangle>({mapFloor::triangle({{51.94462537765503,88.31487894058228}, {48.49110543727875,86.78488731384277}, {48.49110543727875,88.31487894058228}}),mapFloor::triangle({{51.94462537765503,88.31487894058228}, {51.94462537765503,86.78488731384277}, {48.49110543727875,86.78488731384277}}),}), Map<string, string>({
																		pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Inside"}),
																		pair<string, string>({"copy","AT_GoInside"}),
																		pair<string, string>({"areaTransition","ChapelLeft1"}),
																		pair<string, string>({"audio","CHAPELMULTISTEP"}),
																		pair<string, string>({"playerPosX","57.8"}),
																		pair<string, string>({"playerPosY","11.8"}),
																		pair<string, string>({"direction","STAND_FRONT"}),
																		pair<string, string>({"x","0"}),
																		pair<string, string>({"y","0"}),
																		pair<string, string>({"h","50"}),
																		pair<string, string>({"w","50"}),
																		pair<string, string>("colour", "WHITE"),
																		pair<string, string>("uniqueID", mapPopupTextID),
																		pair<string, string>("anchorStyle", "TOPLEFT"),
																		pair<string, string>("shadowColour", "BLACK"),
																		pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																		pair<string, string>({"format","LightText_20"})})),
		mapObject("SpigotPuzzle", true, false, false, "","0",0,0,"1","1.0","CENTRE",{13, 86}, false, List<mapFloor::triangle>({mapFloor::triangle({{12.168928235769272,87.29502558708191}, {15.5882328748703,86.13399267196655}, {12.168928235769272,86.13399267196655}}),mapFloor::triangle({{12.168928235769272,87.29502558708191}, {15.5882328748703,87.29502558708191}, {15.5882328748703,86.13399267196655}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Inspect the Machine"}),
												pair<string, string>({"copy","SpigotPuzzle"}),
												pair<string, string>({"cutscene","SpigotPuzzle"}),
												pair<string, string>({"x","-3"}),
												pair<string, string>({"y","-10"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}),})),
			mapObject("Brazier", true, false, false, "","0",0,0,"1","1.0","CENTRE",{46, 56}, false, List<mapFloor::triangle>({mapFloor::triangle({{43.95310580730438,56.509798765182495}, {45.241549611091614,54.99170422554016}, {45.53069770336151,55.69227933883667}}),mapFloor::triangle({{47.28521406650543,55.70804476737976}, {45.241549611091614,54.99170422554016}, {47.614747285842896,55.01302480697632}}),mapFloor::triangle({{43.93697381019592,58.49132537841797}, {44.59981024265289,60.57726740837097}, {43.085843324661255,58.77775549888611}}),
mapFloor::triangle({{48.78099262714386,58.65432024002075}, {47.87997305393219,60.57726740837097}, {47.481295466423035,59.82171297073364}}),mapFloor::triangle({{48.78099262714386,56.73311948776245}, {47.614747285842896,55.01302480697632}, {47.28521406650543,55.70804476737976}}),mapFloor::triangle({{43.95310580730438,56.509798765182495}, {43.085843324661255,58.77775549888611}, {43.10766160488129,56.097495555877686}}),
mapFloor::triangle({{45.05625665187836,59.82171297073364}, {47.87997305393219,60.57726740837097}, {44.59981024265289,60.57726740837097}}),mapFloor::triangle({{48.78099262714386,58.65432024002075}, {49.63797330856323,56.39956593513489}, {49.63797330856323,58.9982271194458}}),mapFloor::triangle({{43.95310580730438,56.509798765182495}, {43.10766160488129,56.097495555877686}, {45.241549611091614,54.99170422554016}}),
mapFloor::triangle({{47.28521406650543,55.70804476737976}, {45.53069770336151,55.69227933883667}, {45.241549611091614,54.99170422554016}}),mapFloor::triangle({{43.93697381019592,58.49132537841797}, {45.05625665187836,59.82171297073364}, {44.59981024265289,60.57726740837097}}),mapFloor::triangle({{48.78099262714386,58.65432024002075}, {49.63797330856323,58.9982271194458}, {47.87997305393219,60.57726740837097}}),
mapFloor::triangle({{48.78099262714386,56.73311948776245}, {49.63797330856323,56.39956593513489}, {47.614747285842896,55.01302480697632}}),mapFloor::triangle({{43.95310580730438,56.509798765182495}, {43.93697381019592,58.49132537841797}, {43.085843324661255,58.77775549888611}}),mapFloor::triangle({{45.05625665187836,59.82171297073364}, {47.481295466423035,59.82171297073364}, {47.87997305393219,60.57726740837097}}),
mapFloor::triangle({{48.78099262714386,58.65432024002075}, {48.78099262714386,56.73311948776245}, {49.63797330856323,56.39956593513489}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Inspect the Brazier"}),
												pair<string, string>({"copy","Brazier"}),
												pair<string, string>({"cutscene","Brazier"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}),})),
			mapObject("BookNearEntrance", true, false, false, "","0",0,0,"1","1.0","CENTRE",{66, 52}, false, List<mapFloor::triangle>({mapFloor::triangle({{69.43117380142212,55.27215600013733}, {64.2694354057312,53.64850163459778}, {64.2694354057312,55.27215600013733}}),mapFloor::triangle({{69.43117380142212,55.27215600013733}, {69.43117380142212,53.64850163459778}, {64.2694354057312,53.64850163459778}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Inspect the Bookshelf"}),
												pair<string, string>({"copy","BookNearEntrance"}),
												pair<string, string>({"cutscene","ChapelLeft1Book1"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}),})),
			mapObject::getOnetimeTrigger("VisionLCutscene",List<mapFloor::triangle>({mapFloor::triangle({{42.650651931762695,50.75727701187134}, {49.97949004173279,49.26294386386871}, {49.97949004173279,50.75727701187134}}),mapFloor::triangle({{42.650651931762695,50.75727701187134}, {42.650651931762695,49.26294386386871}, {49.97949004173279,49.26294386386871}}),}), ""),
			mapObject("BookNearLeftSpigot", true, false, false, "","0",0,0,"1","1.0","CENTRE",{26, 53}, false, List<mapFloor::triangle>({mapFloor::triangle({{27.718916535377502,54.48126792907715}, {25.611796975135803,55.549657344818115}, {25.611796975135803,54.48126792907715}}),mapFloor::triangle({{27.718916535377502,54.26763296127319}, {25.611796975135803,54.48126792907715}, {25.611796975135803,54.26763296127319}}),mapFloor::triangle({{25.611796975135803,55.549657344818115}, {25.107598304748535,54.48126792907715}, {25.611796975135803,54.48126792907715}}),
mapFloor::triangle({{27.718916535377502,54.48126792907715}, {27.718916535377502,55.83024024963379}, {25.611796975135803,55.549657344818115}}),mapFloor::triangle({{27.718916535377502,54.26763296127319}, {27.718916535377502,54.48126792907715}, {25.611796975135803,54.48126792907715}}),mapFloor::triangle({{25.611796975135803,55.549657344818115}, {25.107598304748535,55.549657344818115}, {25.107598304748535,54.48126792907715}}),}), Map<string, string>({
												pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Look at the Pages"}),
												pair<string, string>({"copy","BookNearLeftSpigot"}),
												pair<string, string>({"cutscene","BookNearLeftSpigot"}),
												pair<string, string>({"x","0"}),
												pair<string, string>({"y","0"}),
												pair<string, string>({"h","50"}),
												pair<string, string>({"w","50"}),
												pair<string, string>("colour", "WHITE"),
												pair<string, string>("uniqueID", mapPopupTextID),
												pair<string, string>("anchorStyle", "TOPLEFT"),
												pair<string, string>("shadowColour", "BLACK"),
												pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
												pair<string, string>({"format","LightText_20"}),})),
			mapObject::getTreasureChest("GardenChest1", { 75,95 }, "NORMAL2_FRONT", "Valois Dress"),
			mapObject::getTreasureChest("GardenChest2", { 31,87 }, "NORMAL2_FRONT", "Djall's Gloves"),
			mapObject::getTreasureChest("GardenChest3", { 26,87 }, "NORMAL2_FRONT", "Iyarri's Gloves"),
			}), List<mapFloor>({
			mapFloor("Stone", List<mapFloor::triangle>({mapFloor::triangle({{51.397138833999634,55.08511662483215}, {49.06984269618988,59.112030267715454}, {51.397138833999634,60.30580997467041}}),mapFloor::triangle({{58.10555815696716,58.491355180740356}, {51.397138833999634,55.229026079177856}, {51.397138833999634,60.16190052032471}}),mapFloor::triangle({{63.462620973587036,53.6686897277832}, {58.10555815696716,58.491355180740356}, {63.462620973587036,58.491355180740356}}),
mapFloor::triangle({{70.58494091033936,54.658931493759155}, {63.2685124874115,58.491355180740356}, {70.58494687080383,58.491355180740356}}),mapFloor::triangle({{81.47326111793518,56.87217116355896}, {74.82188940048218,53.6686897277832}, {74.82188940048218,58.491355180740356}}),mapFloor::triangle({{93.28494071960449,53.03974747657776}, {88.9845609664917,56.87217116355896}, {93.28494668006897,56.87217116355896}}),
mapFloor::triangle({{74.82188940048218,53.6686897277832}, {69.8934257030487,58.491355180740356}, {74.82188940048218,58.491355180740356}}),mapFloor::triangle({{100.01071691513062,52.04950571060181}, {92.51437187194824,56.87217116355896}, {100.01071691513062,56.87217116355896}}),mapFloor::triangle({{89.26350474357605,52.04950571060181}, {81.47326111793518,56.87217116355896}, {89.26350474357605,56.87217116355896}}),
mapFloor::triangle({{49.06984269618988,59.112030267715454}, {49.94296729564667,61.270153522491455}, {51.397138833999634,60.30580997467041}}),mapFloor::triangle({{47.615671157836914,60.0763738155365}, {42.94251203536987,61.270153522491455}, {49.94296729564667,61.270153522491455}}),mapFloor::triangle({{42.94251203536987,61.270153522491455}, {43.64732503890991,58.92834663391113}, {41.243427991867065,60.12212634086609}}),
mapFloor::triangle({{43.64732503890991,58.92834663391113}, {41.243427991867065,55.25030493736267}, {41.243427991867065,60.12212634086609}}),mapFloor::triangle({{42.75291562080383,54.15993332862854}, {46.4335560798645,55.071258544921875}, {46.42649292945862,54.15993332862854}}),mapFloor::triangle({{43.68955194950104,56.85683488845825}, {44.505926966667175,55.45368790626526}, {42.9114431142807,55.925655364990234}}),
mapFloor::triangle({{46.4335560798645,55.071258544921875}, {49.907657504081726,54.15993332862854}, {46.42649292945862,54.15993332862854}}),mapFloor::triangle({{46.4335560798645,55.071258544921875}, {49.06984269618988,56.27889633178711}, {49.96459484100342,55.071258544921875}}),mapFloor::triangle({{49.06984269618988,56.27889633178711}, {51.397138833999634,55.08511662483215}, {49.96459484100342,55.071258544921875}}),
mapFloor::triangle({{41.243427991867065,55.25030493736267}, {34.63623225688934,58.56128931045532}, {41.243427991867065,60.12212634086609}}),mapFloor::triangle({{49.907657504081726,54.15993332862854}, {42.841362953186035,35.96198856830597}, {42.75291562080383,54.15993332862854}}),mapFloor::triangle({{42.94251203536987,61.270153522491455}, {49.94296729564667,64.4202709197998}, {49.94296729564667,61.270153522491455}}),
mapFloor::triangle({{23.3922079205513,53.689467906951904}, {17.77019500732422,58.56128931045532}, {23.3922079205513,58.56128931045532}}),mapFloor::triangle({{17.77019500732422,51.26824975013733}, {1.2017056345939636,61.90307140350342}, {17.77019500732422,61.90307140350342}}),mapFloor::triangle({{16.408637166023254,51.45869255065918}, {2.5632627308368683,50.347697734832764}, {2.5632627308368683,51.45869255065918}}),
mapFloor::triangle({{34.63623225688934,53.689467906951904}, {29.938504099845886,58.56128931045532}, {34.63623225688934,58.56128931045532}}),mapFloor::triangle({{29.938504099845886,54.25580143928528}, {23.3922079205513,58.56128931045532}, {29.938504099845886,58.56128931045532}}),mapFloor::triangle({{49.820783734321594,35.96198856830597}, {42.841362953186035,31.387105584144592}, {42.841362953186035,35.96198856830597}}),
mapFloor::triangle({{42.841362953186035,31.387105584144592}, {18.413612246513367,35.96198856830597}, {42.841362953186035,35.96198856830597}}),mapFloor::triangle({{2.583356946706772,31.387105584144592}, {1.1612254194915295,25.103741884231567}, {1.161225140094757,31.387105584144592}}),mapFloor::triangle({{16.535261273384094,25.103741884231567}, {2.5833576917648315,24.124911427497864}, {2.5833576917648315,25.103741884231567}}),
mapFloor::triangle({{18.413612246513367,31.387105584144592}, {16.535261273384094,25.103741884231567}, {16.535261273384094,31.387105584144592}}),mapFloor::triangle({{2.583356946706772,31.387105584144592}, {1.161225140094757,35.96198856830597}, {2.583356946706772,35.96198856830597}}),mapFloor::triangle({{16.535261273384094,25.103741884231567}, {2.583356946706772,31.387105584144592}, {16.535261273384094,31.387105584144592}}),
mapFloor::triangle({{2.583356946706772,35.96198856830597}, {16.535261273384094,31.387105584144592}, {2.583356946706772,31.387105584144592}}),mapFloor::triangle({{18.413612246513367,31.387105584144592}, {16.535261273384094,35.96198856830597}, {18.413612246513367,35.96198856830597}}),mapFloor::triangle({{49.820783734321594,21.012625098228455}, {42.841362953186035,10.638145357370377}, {42.841362953186035,21.012625098228455}}),
mapFloor::triangle({{49.820783734321594,31.387105584144592}, {42.841362953186035,21.012625098228455}, {42.841362953186035,31.387105584144592}}),mapFloor::triangle({{24.964992702007294,10.638144612312317}, {23.68238717317581,21.012623608112335}, {24.964992702007294,21.012623608112335}}),mapFloor::triangle({{65.07577896118164,10.638146102428436}, {46.61824107170105,9.513507038354874}, {46.61824107170105,10.638145357370377}}),
mapFloor::triangle({{98.53933453559875,21.012626588344574}, {99.77153539657593,10.638147592544556}, {98.53933453559875,10.638147592544556}}),mapFloor::triangle({{42.841362953186035,10.638145357370377}, {38.89254927635193,21.012625098228455}, {42.841362953186035,21.012625098228455}}),mapFloor::triangle({{38.89254927635193,10.638145357370377}, {24.964992702007294,21.012623608112335}, {38.89254927635193,21.012625098228455}}),
mapFloor::triangle({{38.89254927635193,10.638145357370377}, {24.964992702007294,9.749218076467514}, {24.964992702007294,10.638144612312317}}),mapFloor::triangle({{49.820783734321594,21.012625098228455}, {84.58559513092041,10.638146847486496}, {49.820783734321594,10.638145357370377}}),mapFloor::triangle({{84.58559513092041,21.012625098228455}, {98.53933453559875,10.638147592544556}, {84.58559513092041,10.638146847486496}}),
mapFloor::triangle({{98.53933453559875,10.638147592544556}, {84.58559513092041,9.748807549476624}, {84.58559513092041,10.638146847486496}}),mapFloor::triangle({{51.397138833999634,55.08511662483215}, {49.06984269618988,56.27889633178711}, {49.06984269618988,59.112030267715454}}),mapFloor::triangle({{58.10555815696716,58.491355180740356}, {58.10555815696716,53.6686897277832}, {51.397138833999634,55.229026079177856}}),
mapFloor::triangle({{63.462620973587036,53.6686897277832}, {58.10555815696716,53.6686897277832}, {58.10555815696716,58.491355180740356}}),mapFloor::triangle({{70.58494091033936,54.658931493759155}, {63.26850652694702,54.658931493759155}, {63.2685124874115,58.491355180740356}}),mapFloor::triangle({{81.47326111793518,56.87217116355896}, {81.47326111793518,52.04950571060181}, {74.82188940048218,53.6686897277832}}),
mapFloor::triangle({{93.28494071960449,53.03974747657776}, {88.98455500602722,53.03974747657776}, {88.9845609664917,56.87217116355896}}),mapFloor::triangle({{74.82188940048218,53.6686897277832}, {69.8934257030487,53.6686897277832}, {69.8934257030487,58.491355180740356}}),mapFloor::triangle({{100.01071691513062,52.04950571060181}, {92.51437187194824,52.04950571060181}, {92.51437187194824,56.87217116355896}}),
mapFloor::triangle({{89.26350474357605,52.04950571060181}, {81.47326111793518,52.04950571060181}, {81.47326111793518,56.87217116355896}}),mapFloor::triangle({{49.06984269618988,59.112030267715454}, {47.615671157836914,60.0763738155365}, {49.94296729564667,61.270153522491455}}),mapFloor::triangle({{47.615671157836914,60.0763738155365}, {44.90240812301636,60.0763738155365}, {42.94251203536987,61.270153522491455}}),
mapFloor::triangle({{42.94251203536987,61.270153522491455}, {44.90240812301636,60.0763738155365}, {43.64732503890991,58.92834663391113}}),mapFloor::triangle({{43.64732503890991,58.92834663391113}, {43.64732503890991,55.2432656288147}, {41.243427991867065,55.25030493736267}}),mapFloor::triangle({{42.75291562080383,54.15993332862854}, {42.70734786987305,56.14250302314758}, {46.4335560798645,55.071258544921875}}),
mapFloor::triangle({{46.4335560798645,55.071258544921875}, {49.96459484100342,55.071258544921875}, {49.907657504081726,54.15993332862854}}),mapFloor::triangle({{41.243427991867065,55.25030493736267}, {34.63623225688934,53.689467906951904}, {34.63623225688934,58.56128931045532}}),mapFloor::triangle({{49.907657504081726,54.15993332862854}, {49.820783734321594,35.96198856830597}, {42.841362953186035,35.96198856830597}}),
mapFloor::triangle({{42.94251203536987,61.270153522491455}, {42.94251203536987,64.4202709197998}, {49.94296729564667,64.4202709197998}}),mapFloor::triangle({{23.3922079205513,53.689467906951904}, {17.77019500732422,53.689467906951904}, {17.77019500732422,58.56128931045532}}),mapFloor::triangle({{17.77019500732422,51.26824975013733}, {1.2017056345939636,51.26824975013733}, {1.2017056345939636,61.90307140350342}}),
mapFloor::triangle({{16.408637166023254,51.45869255065918}, {16.408637166023254,50.347697734832764}, {2.5632627308368683,50.347697734832764}}),mapFloor::triangle({{34.63623225688934,53.689467906951904}, {29.938504099845886,53.689467906951904}, {29.938504099845886,58.56128931045532}}),mapFloor::triangle({{29.938504099845886,54.25580143928528}, {23.3922079205513,54.25580143928528}, {23.3922079205513,58.56128931045532}}),
mapFloor::triangle({{49.820783734321594,35.96198856830597}, {49.820783734321594,31.387105584144592}, {42.841362953186035,31.387105584144592}}),mapFloor::triangle({{42.841362953186035,31.387105584144592}, {18.413612246513367,31.387105584144592}, {18.413612246513367,35.96198856830597}}),mapFloor::triangle({{2.583356946706772,31.387105584144592}, {2.5833576917648315,25.103741884231567}, {1.1612254194915295,25.103741884231567}}),
mapFloor::triangle({{16.535261273384094,25.103741884231567}, {16.535261273384094,24.124911427497864}, {2.5833576917648315,24.124911427497864}}),mapFloor::triangle({{18.413612246513367,31.387105584144592}, {18.413612246513367,25.103741884231567}, {16.535261273384094,25.103741884231567}}),mapFloor::triangle({{2.583356946706772,31.387105584144592}, {1.161225140094757,31.387105584144592}, {1.161225140094757,35.96198856830597}}),
mapFloor::triangle({{16.535261273384094,25.103741884231567}, {2.5833576917648315,25.103741884231567}, {2.583356946706772,31.387105584144592}}),mapFloor::triangle({{2.583356946706772,35.96198856830597}, {16.535261273384094,35.96198856830597}, {16.535261273384094,31.387105584144592}}),mapFloor::triangle({{18.413612246513367,31.387105584144592}, {16.535261273384094,31.387105584144592}, {16.535261273384094,35.96198856830597}}),
mapFloor::triangle({{49.820783734321594,21.012625098228455}, {49.820783734321594,10.638145357370377}, {42.841362953186035,10.638145357370377}}),mapFloor::triangle({{49.820783734321594,31.387105584144592}, {49.820783734321594,21.012625098228455}, {42.841362953186035,21.012625098228455}}),mapFloor::triangle({{24.964992702007294,10.638144612312317}, {23.68238717317581,10.638144612312317}, {23.68238717317581,21.012623608112335}}),
mapFloor::triangle({{65.07577896118164,10.638146102428436}, {65.07577896118164,9.513507783412933}, {46.61824107170105,9.513507038354874}}),mapFloor::triangle({{98.53933453559875,21.012626588344574}, {99.77153539657593,21.012626588344574}, {99.77153539657593,10.638147592544556}}),mapFloor::triangle({{42.841362953186035,10.638145357370377}, {38.89254927635193,10.638145357370377}, {38.89254927635193,21.012625098228455}}),
mapFloor::triangle({{38.89254927635193,10.638145357370377}, {24.964992702007294,10.638144612312317}, {24.964992702007294,21.012623608112335}}),mapFloor::triangle({{38.89254927635193,10.638145357370377}, {38.89254927635193,9.749218821525574}, {24.964992702007294,9.749218076467514}}),mapFloor::triangle({{49.820783734321594,21.012625098228455}, {84.58559513092041,21.012625098228455}, {84.58559513092041,10.638146847486496}}),
mapFloor::triangle({{84.58559513092041,21.012625098228455}, {98.53933453559875,21.012626588344574}, {98.53933453559875,10.638147592544556}}),mapFloor::triangle({{98.53933453559875,10.638147592544556}, {98.53933453559875,9.748808294534683}, {84.58559513092041,9.748807549476624}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "CHAPELSTEP"})})),
			mapFloor("Grass", List<mapFloor::triangle>({mapFloor::triangle({{15.886299312114716,99.97643232345581}, {19.757145643234253,94.74225044250488}, {15.886299312114716,94.74225044250488}}),mapFloor::triangle({{57.95767903327942,87.73446083068848}, {42.78216361999512,88.7547254562378}, {42.78216361999512,87.73446083068848}}),mapFloor::triangle({{7.139986008405685,94.83333826065063}, {15.886299312114716,90.77512621879578}, {7.139986008405685,89.5991563796997}}),
mapFloor::triangle({{75.22393465042114,94.83333826065063}, {16.508764028549194,96.23841643333435}, {16.508764028549194,94.83333826065063}}),mapFloor::triangle({{46.51873707771301,86.92187666893005}, {54.51710820198059,87.73446083068848}, {46.222737431526184,87.73446083068848}}),mapFloor::triangle({{65.52441120147705,89.5991563796997}, {6.809242814779282,94.83333826065063}, {6.809242814779282,89.5991563796997}}),
mapFloor::triangle({{32.9508513212204,86.72259449958801}, {20.49853503704071,90.29130339622498}, {20.49853503704071,86.72259449958801}}),mapFloor::triangle({{35.700634121894836,88.4772002696991}, {32.49426782131195,91.3922369480133}, {32.49426782131195,88.4772002696991}}),mapFloor::triangle({{35.700634121894836,87.82352805137634}, {32.49426782131195,88.4772002696991}, {32.49426782131195,87.82352805137634}}),
mapFloor::triangle({{37.002360820770264,88.67153525352478}, {38.30409049987793,91.3922369480133}, {37.002360820770264,91.3922369480133}}),mapFloor::triangle({{35.700634121894836,91.3922369480133}, {37.002360820770264,88.67153525352478}, {37.002360820770264,91.3922369480133}}),mapFloor::triangle({{38.30409049987793,88.4772002696991}, {65.48651456832886,91.3922369480133}, {38.30409049987793,91.3922369480133}}),
mapFloor::triangle({{15.886299312114716,99.97643232345581}, {19.757145643234253,99.97643232345581}, {19.757145643234253,94.74225044250488}}),mapFloor::triangle({{57.95767903327942,87.73446083068848}, {57.95767903327942,88.7547254562378}, {42.78216361999512,88.7547254562378}}),mapFloor::triangle({{7.139986008405685,94.83333826065063}, {15.886299312114716,96.0093080997467}, {15.886299312114716,90.77512621879578}}),
mapFloor::triangle({{75.22393465042114,94.83333826065063}, {75.22393465042114,96.23841643333435}, {16.508764028549194,96.23841643333435}}),mapFloor::triangle({{46.51873707771301,86.92187666893005}, {54.221111536026,86.92187666893005}, {54.51710820198059,87.73446083068848}}),mapFloor::triangle({{65.52441120147705,89.5991563796997}, {65.52441120147705,94.83333826065063}, {6.809242814779282,94.83333826065063}}),
mapFloor::triangle({{32.9508513212204,86.72259449958801}, {32.9508513212204,90.29130339622498}, {20.49853503704071,90.29130339622498}}),mapFloor::triangle({{35.700634121894836,88.4772002696991}, {35.700634121894836,91.3922369480133}, {32.49426782131195,91.3922369480133}}),mapFloor::triangle({{35.700634121894836,87.82352805137634}, {35.700634121894836,88.4772002696991}, {32.49426782131195,88.4772002696991}}),
mapFloor::triangle({{37.002360820770264,88.67153525352478}, {38.30409049987793,88.4772002696991}, {38.30409049987793,91.3922369480133}}),mapFloor::triangle({{35.700634121894836,91.3922369480133}, {35.700634121894836,88.4772002696991}, {37.002360820770264,88.67153525352478}}),mapFloor::triangle({{38.30409049987793,88.4772002696991}, {65.48651456832886,88.4772002696991}, {65.48651456832886,91.3922369480133}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "GRASS"})})),
			mapFloor("Wooden", List<mapFloor::triangle>({mapFloor::triangle({{15.886299312114716,96.34596705436707}, {0.8225799538195133,98.39903116226196}, {15.886299312114716,99.97643232345581}}),mapFloor::triangle({{6.422234326601028,94.76856589317322}, {0.9637381881475449,98.39903116226196}, {6.422234326601028,98.39903116226196}}),mapFloor::triangle({{6.422234326601028,94.76856589317322}, {0.9637385606765747,86.13182306289673}, {0.9637381881475449,94.76856589317322}}),
mapFloor::triangle({{2.6299305260181427,85.79351902008057}, {12.333574891090393,88.10955286026001}, {2.3198202252388,87.97622919082642}}),mapFloor::triangle({{3.471720963716507,85.31798720359802}, {12.333574891090393,85.92684268951416}, {2.6299305260181427,85.79351902008057}}),mapFloor::triangle({{15.886299312114716,96.34596705436707}, {0.8225799538195133,94.76856589317322}, {0.8225799538195133,98.39903116226196}}),
mapFloor::triangle({{6.422234326601028,94.76856589317322}, {0.9637381881475449,94.76856589317322}, {0.9637381881475449,98.39903116226196}}),mapFloor::triangle({{6.422234326601028,94.76856589317322}, {6.422235071659088,86.13182306289673}, {0.9637385606765747,86.13182306289673}}),mapFloor::triangle({{2.6299305260181427,85.79351902008057}, {12.333574891090393,85.92684268951416}, {12.333574891090393,88.10955286026001}}),
mapFloor::triangle({{3.471720963716507,85.31798720359802}, {12.333574891090393,85.45131087303162}, {12.333574891090393,85.92684268951416}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "WOOD"})})),
			mapFloor("StoneOutside", List<mapFloor::triangle>({mapFloor::triangle({{12.168928235769272,87.29502558708191}, {15.5882328748703,86.13399267196655}, {12.168928235769272,86.13399267196655}}),mapFloor::triangle({{12.168928235769272,87.29502558708191}, {15.5882328748703,87.29502558708191}, {15.5882328748703,86.13399267196655}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "STONE"})})),
			}), {}, { 5000,5000 }, Map<string, string>({
					pair<string,string>({"song1",to_string(CHAPEL_WAV_1) + " " + "MusicVolume"}),
					pair<string, string>({ "LoadingScreenImage",to_string(LOADINGSCREEN_1) }),
				}));
		maps["TavernUpstairsNight"] = mapInstance("TavernUpstairsNight", EMPTYMAP, { 50,50 }, List<mapObject>({
			mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("Tavern1AsObject1", false, true, false, imageLookup.getSequenceAsString("TAVERN1_UPSTAIRS","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("Tavern1AsObject2", false, true, false, imageLookup.getSequenceAsString("TAVERN1_UPSTAIRS","ON_TOP"),"0",100,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			}), {}, {}, { 5000,5000 }, { Map<string, string>({
					pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
					pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
					}) });
		maps["TavernUpstairsDay"] = mapInstance("TavernUpstairsDay", EMPTYMAP, { 50,50 }, List<mapObject>({
			mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("Tavern1AsObject1", false, true, false, imageLookup.getSequenceAsString("TAVERN1_UPSTAIRSDAY","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("Tavern1AsObject2", false, true, false, imageLookup.getSequenceAsString("TAVERN1_UPSTAIRS","ON_TOP"),"0",100,imageLookup.layerDefaults["PLAYER"] + 2,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("TavernBed", false, true, false, imageLookup.getSequenceAsString("TAVERNBED","STAND_FRONT"),"0",100,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("AT_Downstairs", true, false, false, "","0",0,0,"1","1.0","CENTRE",{58, 53}, false, List<mapFloor::triangle>({mapFloor::triangle({{57.8132688999176,52.938079833984375}, {56.194424629211426,57.698220014572144}, {56.194424629211426,52.938079833984375}}),mapFloor::triangle({{57.8132688999176,52.938079833984375}, {57.8132688999176,57.698220014572144}, {56.194424629211426,57.698220014572144}}),}), Map<string, string>({
																	pair<string, string>({"message","$LANGUAGE$_Map Pop Up Text_Go Downstairs"}),
																	pair<string, string>({"copy","AT_Downstairs"}),
																	pair<string, string>({"areaTransition","Tavern1"}),
																	pair<string, string>({"direction","STAND_FRONT"}),
																	pair<string, string>({"playerPosX","44"}),
																	pair<string, string>({"playerPosY","53"}),
																	pair<string, string>({"audio","WOODENSTAIRS"}),
																	pair<string, string>({"x","0"}),
																	pair<string, string>({"y","0"}),
																	pair<string, string>({"h","50"}),
																	pair<string, string>({"w","50"}),
																	pair<string, string>("colour", "WHITE"),
																	pair<string, string>("uniqueID", mapPopupTextID),
																	pair<string, string>("anchorStyle", "TOPLEFT"),
																	pair<string, string>("shadowColour", "BLACK"),
																	pair<string, string>("layer", to_string(imageLookup.layerDefaults["TEXTONMAP"])),
																	pair<string, string>({"format","LightText_20"}),
																		})),
			}), List<mapFloor>({
				mapFloor("ChapelFloorWood", List<mapFloor::triangle>({mapFloor::triangle({{47.900742292404175,49.360862374305725}, {48.69390428066254,51.066774129867554}, {48.69390428066254,48.657289147377014}}),mapFloor::triangle({{50.768011808395386,51.09173655509949}, {51.87022089958191,48.69551956653595}, {50.768011808395386,48.68225455284119}}),mapFloor::triangle({{48.69390428066254,51.066774129867554}, {50.768011808395386,48.68225455284119}, {48.69390428066254,48.657289147377014}}),
mapFloor::triangle({{51.87022089958191,51.10500454902649}, {50.768011808395386,53.45005393028259}, {51.87022089958191,53.463321924209595}}),mapFloor::triangle({{57.8132688999176,48.032355308532715}, {56.194424629211426,52.938079833984375}, {56.194424629211426,48.032355308532715}}),mapFloor::triangle({{55.86037635803223,48.032355308532715}, {52.17651724815369,52.938079833984375}, {52.17651724815369,48.032355308532715}}),
mapFloor::triangle({{55.86037635803223,52.938079833984375}, {52.17651724815369,57.1873664855957}, {52.17651724815369,52.938079833984375}}),mapFloor::triangle({{56.194424629211426,48.032355308532715}, {55.86037635803223,52.938079833984375}, {55.86037635803223,48.032355308532715}}),mapFloor::triangle({{57.8132688999176,52.938079833984375}, {56.194424629211426,57.698220014572144}, {56.194424629211426,52.938079833984375}}),
mapFloor::triangle({{51.65793299674988,50.36550760269165}, {52.59343385696411,48.71167838573456}, {51.65793299674988,48.71167838573456}}),mapFloor::triangle({{47.900742292404175,49.360862374305725}, {47.900742292404175,51.066774129867554}, {48.69390428066254,51.066774129867554}}),mapFloor::triangle({{50.768011808395386,51.09173655509949}, {51.87022089958191,51.10500454902649}, {51.87022089958191,48.69551956653595}}),
mapFloor::triangle({{48.69390428066254,51.066774129867554}, {50.768011808395386,51.09173655509949}, {50.768011808395386,48.68225455284119}}),mapFloor::triangle({{51.87022089958191,51.10500454902649}, {50.768011808395386,51.09173655509949}, {50.768011808395386,53.45005393028259}}),mapFloor::triangle({{57.8132688999176,48.032355308532715}, {57.8132688999176,52.938079833984375}, {56.194424629211426,52.938079833984375}}),
mapFloor::triangle({{55.86037635803223,48.032355308532715}, {55.86037635803223,52.938079833984375}, {52.17651724815369,52.938079833984375}}),mapFloor::triangle({{55.86037635803223,52.938079833984375}, {55.86037635803223,57.1873664855957}, {52.17651724815369,57.1873664855957}}),mapFloor::triangle({{56.194424629211426,48.032355308532715}, {56.194424629211426,52.938079833984375}, {55.86037635803223,52.938079833984375}}),
mapFloor::triangle({{57.8132688999176,52.938079833984375}, {57.8132688999176,57.698220014572144}, {56.194424629211426,57.698220014572144}}),mapFloor::triangle({{51.65793299674988,50.36550760269165}, {52.59343385696411,50.36550760269165}, {52.59343385696411,48.71167838573456}}),}),true,Map<string, string>({pair<string, string>({"audio", "1"}), pair<string, string>({"audio source", "FLOORBOARD"})})),
				}), {}, { 5000,5000 }, { Map<string, string>({
					pair<string,string>({"song1",to_string(FIREPLACE_WAV_1) + " " + "AmbienceVolume"}),
					pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
					}) });
		maps["Vatican"] = mapInstance("Vatican", EMPTYMAP, { 50,50 }, List<mapObject>({
			mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("VATICAN", false, true, false, imageLookup.getSequenceAsString("VATICAN","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			}), {}, {}, { 5000,5000 }, { Map<string, string>({
					pair<string,string>({"song1",to_string(VATICANAMBIENCE_WAV) + " " + "AmbienceVolume"}),
					pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
					}) });
		maps["TianshunHouse"] = mapInstance("TianshunHouse", EMPTYMAP, { 50,50 }, List<mapObject>({
			mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("TianshunHouse", false, true, false, imageLookup.getSequenceAsString("TianshunHouse","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			}), {}, {}, { 5000,5000 }, { Map<string, string>({
					//pair<string,string>({"song1",to_string(VATICANAMBIENCE_WAV) + " " + "AmbienceVolume"}),
					pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
					}) });
		maps["ViennaUniversity"] = mapInstance("ViennaUniversity", EMPTYMAP, { 50,50 }, List<mapObject>({
			mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("ViennaUniversity", false, true, false, imageLookup.getSequenceAsString("ViennaUniversity","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("Peuerbach", false, true, false, imageLookup.getSequenceAsString("Peuerbach","STAND_FRONT"),"1",500,imageLookup.layerDefaults["MAP"] + 2,"1.0","1.0","CENTRE",{50, 49.5}, false, {}, {}),
			mapObject("ViennaUniversity2", false, true, false, imageLookup.getSequenceAsString("ViennaUniversity2","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 3,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			}), {}, {}, { 5000,5000 }, { Map<string, string>({
				pair<string,string>({"song2",to_string(CLOCK1_WAV) + " " + "AmbienceVolume"}),
				pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
				}) });
		maps["SpainTavern"] = mapInstance("SpainTavern", EMPTYMAP, { 50,50 }, List<mapObject>({
			mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("SPAINTAVERN1", false, true, false, imageLookup.getSequenceAsString("SPAINTAVERN1","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("Fernando", false, true, false, imageLookup.getSequenceAsString("Fernando","STAND_LEFT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1","0.95","CENTRE",{49.5, 52.3}, false, {}, {}),
			// to right 1
			mapObject("Man1", false, true, false, imageLookup.getSequenceAsString("TAVERNMAN","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1","1.2","CENTRE",{51.3, 52.3}, false, {}, {}),
			mapObject("Man2", false, true, false, imageLookup.getSequenceAsString("EnragedMagician","STAND_BACK"),"1",500,imageLookup.layerDefaults["PLAYER"],"1","1.2","CENTRE",{51.3, 53.5}, false, {}, {}),
			// to right 2
			mapObject("Woman1", false, true, false, imageLookup.getSequenceAsString("TAVERNWOMAN","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1","1.2","CENTRE",{55, 51}, false, {}, {}),
			mapObject("Woman2", false, true, false, imageLookup.getSequenceAsString("ELDERLYF2","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1","0.95","CENTRE",{55, 52.5}, false, {}, {}),
			// to right 3
			mapObject("Man3", false, true, false, imageLookup.getSequenceAsString("NPCMAN1","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1","1.0","CENTRE",{53, 52}, false, {}, {}),
			// to left 1
			mapObject("Man4", false, true, false, imageLookup.getSequenceAsString("NPCMAN2","STAND_FRONT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1.0","1","CENTRE",{46, 51.2}, false, {}, {}),
			mapObject("Woman3", false, true, false, imageLookup.getSequenceAsString("TAVERNWOMAN","STAND_RIGHT"),"1",500,imageLookup.layerDefaults["PLAYER"],"1","1.2","CENTRE",{44.5, 51.2}, false, {}, {}),
			mapObject("Man5", false, true, false, imageLookup.getSequenceAsString("TAVERNMAN","STAND_BACK"),"1",500,imageLookup.layerDefaults["PLAYER"],"1","1.2","CENTRE",{45.5, 52.5}, false, {}, {}),
			}), {}, {}, { 5000,5000 }, { Map<string, string>({
				//pair<string,string>({"song2",to_string(CLOCK1_WAV) + " " + "AmbienceVolume"}),
				pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
				}) });
		maps["CairoPalace"] = mapInstance("CairoPalace", EMPTYMAP, { 50,50 }, List<mapObject>({
			mapObject("FilmGrain1", false, true, true, imageLookup.getSequenceAsString("FilmGrain1","STAND_FRONT"),"1",1,imageLookup.layerDefaults["WEATHER"],"1.0","1.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("CairoPalace", false, true, false, imageLookup.getSequenceAsString("CairoPalace","STAND_FRONT"),"0",0,imageLookup.layerDefaults["MAP"] + 1,"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("CairoPalaceTop", false, true, false, imageLookup.getSequenceAsString("CairoPalaceTop","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"] + 1,"1.0","2.0","CENTRE",{50, 50}, false, {}, {}),
			mapObject("PalaceVisitor", false, true, false, imageLookup.getSequenceAsString("PalaceVisitor","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"],"1.0","1.0","CENTRE",{50, 45.5}, false, {}, {}),
			mapObject("Shadow PalaceVisitor", false, true, false, imageLookup.getSequenceAsString("Shadow PalaceVisitor","STAND_FRONT"),"0",0,imageLookup.layerDefaults["PLAYER"] - 1,"0.5","1.0","CENTRE",{50, 45.5}, false, {}, {}),
			}), {}, {}, { 5000,5000 }, { Map<string, string>({
				//pair<string,string>({"song1",to_string(VATICANAMBIENCE_WAV) + " " + "AmbienceVolume"}),
				pair<string,string>({"LoadingScreenImage",to_string(LOADINGSCREEN_1)}),
				}) });
	}


	void loadMap(string mapName) {
		defineAllMaps();
		currentMap = maps[mapName];
		playerOnMap.position = currentMap.playerStartPosition;
		mapSize = currentMap.imageSize;
		resolutionAsFloat = { mapSize.first * 100.0f / actualRenderSizeAsFloat.first, mapSize.second * 100.0f / actualRenderSizeAsFloat.second };
	}
	mapObject& getThisMapObject(string name) {
		for (mapObject& obj : currentMap.objects.internalList) {
			if (obj.name == name) {
				return *&obj;
			}
		}
	}
	map<string, pair<float, float>> getUpdatedMapImagePositions() {
		map<string, pair<float, float>> result;
		if (perspective == "FOLLOW_PLAYER") {
			activeCamera.position = playerOnMap.position;
		}
		result["player image position"] = { 50.0f,50.0f };
		result["map position"] = { 50.0f + (50.0f - activeCamera.position.first) * resolutionAsFloat.first / 100.0f , 50.0f + (50.0f - activeCamera.position.second) * resolutionAsFloat.second / 100.0f };

		float xLimitMax; float xLimitMin; float yLimitMax; float yLimitMin;

		if (mapSize.first == 5000) {
			xLimitMax = 0.04 * mapSize.first; // 0.03944
			xLimitMin = -97.5;
			yLimitMax = 0.06944 * mapSize.second;
			yLimitMin = -245.549377;
		}
		else {
			xLimitMax = 98.6f;
			xLimitMin = 1.6f;
			yLimitMax = 173.6f;
			yLimitMin = -73.6f;
		}

		bool hold = true;


		if (hold) {
			if (result["map position"].first > xLimitMax) {
				float difference = result["map position"].first - xLimitMax;
				result["map position"].first = xLimitMax;
				result["player image position"].first = 50.0f - difference;
			}
			if (result["map position"].first < xLimitMin) {
				float difference = result["map position"].first;
				result["map position"].first = xLimitMin;
				result["player image position"].first = (difference + 50.0f) * -1;
			}
			if (result["map position"].second > yLimitMax) {
				float difference = result["map position"].second;
				result["map position"].second = yLimitMax;
				result["player image position"].second = 50 + (yLimitMax - difference);
			}
			if (result["map position"].second < yLimitMin) {
				float difference = result["map position"].second;
				result["map position"].second = yLimitMin;
				result["player image position"].second = 50 - (yLimitMin * -1 + difference);
			}
		}
		result["absolute map position"] = {
			actualRenderSizeAsFloat.first * result["map position"].first / 100.0f,
			actualRenderSizeAsFloat.second * result["map position"].second / 100.0f,
		};
		result["absolute map top left"] = {
			result["absolute map position"].second - (mapSize.second / 2),
			result["absolute map position"].first - (mapSize.first / 2),
		};

		if (perspective == "FOLLOW_CAMERA") {
			float absoluteXPos = result["absolute map top left"].second + (mapSize.first / 100 * playerOnMap.position.first);
			float absoluteYPos = result["absolute map top left"].first + (mapSize.second / 100 * playerOnMap.position.second);
			float percentXPos = absoluteXPos * 100.0f / actualRenderSizeAsFloat.first;
			float percentYPos = absoluteYPos * 100.0f / actualRenderSizeAsFloat.second;
			float percentOffsetX = 0.0;
			float percentOffsetY = 0.0;
			pair<float, float> objectPosition = {
				percentXPos,
				percentYPos,
			};
			result["player image position"] = objectPosition;
		}

		for (auto x : currentMap.objects.internalList) {
			if (x.tracksToPlayer) { result[x.name] = { 50.0f, 50.0f }; }
			if (!x.tracksToPlayer) {
				float absoluteXPos = result["absolute map top left"].second + (mapSize.first / 100 * x.positionOnMap.first);
				float absoluteYPos = result["absolute map top left"].first + (mapSize.second / 100 * x.positionOnMap.second);
				float percentXPos = absoluteXPos * 100.0f / actualRenderSizeAsFloat.first;
				float percentYPos = absoluteYPos * 100.0f / actualRenderSizeAsFloat.second;
				float percentOffsetX = 0.0;
				float percentOffsetY = 0.0;
				pair<float, float> objectPosition = {
					percentXPos,
					percentYPos,
				};

				result[x.name] = objectPosition;

				if (x.data.getKeys().contains("copy")) {
					percentOffsetX = stof(x.data["x"]);
					percentOffsetY = stof(x.data["y"]);
				}
				result[x.name + "_text"] = { percentXPos + percentOffsetX, percentYPos + percentOffsetY };
			}
		}

		return result;
	}
	bool isThisDestinationWalkable(pair<float, float> position) {
		bool walkable = false;
		bool obstructed = false;
		for (auto x : currentMap.walkables.internalList) {
			if (x.traversable and x.isSteppedOn(position)) {
				walkable = true;
			}
		}
		for (auto x : currentMap.objects.internalList) {
			if (x.obstruction and x.isSteppedOn(position)) {
				obstructed = true;
			}
		}
		return (walkable and not obstructed);
	}
	void playWalkingAudio() {
		List<mapFloor> currentlySteppedOn = getCurrentlySteppedOn();
		for (auto walkable : currentlySteppedOn.internalList) {
			if (walkable.hasAudio()) {
				walkable.playAudio();
				return;
			}
		}
	}
	void tryToMovePlayer(string direction, float unitOfMovement) {
		pair<float, float> proposedPosition;
		pair<float, float> toMove;
		float unit = unitOfMovement;
		if (direction == "BACK") { toMove.second -= unit; }
		if (direction == "FRONT") { toMove.second += unit; }
		if (direction == "LEFT") { toMove.first -= unit; }
		if (direction == "RIGHT") { toMove.first += unit; }
		proposedPosition.first = TChange(playerOnMap.position.first, toMove.first, 0.0f, 100.0f);
		proposedPosition.second = TChange(playerOnMap.position.second, toMove.second, 0.0f, 100.0f);
		if (isThisDestinationWalkable(proposedPosition)) {
			playerOnMap.position.first = TChange(playerOnMap.position.first, toMove.first, 0.0f, 100.0f);
			playerOnMap.position.second = TChange(playerOnMap.position.second, toMove.second, 0.0f, 100.0f);
		}
	}
	string debug() {
		map<string, pair<float, float>> updatedImagePositions = getUpdatedMapImagePositions();
		string result = "Player Position On Map: " + to_string(playerOnMap.position.first) + ", " + to_string(playerOnMap.position.second);
		result += "\nStepped On: ";
		List<mapFloor> steppedOn = getCurrentlySteppedOn();
		List<mapObject> objectsSteppedOn = getObjectsInRange();
		List<mapObject> nearby = getObjectsThatAreClose();
		for (auto const& x : steppedOn.internalList) {
			result += x.uniqueID;
		}
		result += "\nIn range of these objects: ";
		for (auto const& x : nearby.internalList) {
			result += x.name;
		}
		result += "\nStepped on these objects: ";
		for (auto const& x : objectsSteppedOn.internalList) {
			result += x.name;
		}
		result += "\nMap Image Position: " + to_string(updatedImagePositions["map position"].first) + " " + to_string(updatedImagePositions["map position"].second);
		return result;
	}
	List<mapFloor> getCurrentlySteppedOn() {
		List<mapFloor> result;
		for (auto x : currentMap.walkables.internalList) {
			if (x.isSteppedOn(playerOnMap.position)) {
				result.push_back(x);
			}
		}
		return result;
	}
	List<mapObject> getObjectsInRange() {
		// stepped on
		List<mapObject> result;
		for (auto x : currentMap.objects.internalList) {
			if (x.isSteppedOn(playerOnMap.position)) {
				result.push_back(x);
			}
		}
		return result;
	}
	List<mapObject> getObjectsThatAreClose() {
		// stepped near
		List<mapObject> result;
		for (auto x : currentMap.objects.internalList) {
			if (x.isSteppedNear(playerOnMap.position)) {
				result.push_back(x);
			}
		}
		return result;
	}
	void disableThisObject(string objectName) {
		for (int x = 0; x < currentMap.objects.size(); x++) {
			if (currentMap.objects.at(x).name == objectName) {
				currentMap.objects.at(x).canInteract = false;
			}
		}
	}
	Map<string, int> getAnimationSpeeds() {
		Map<string, int> result;
		result.internalMap = { pair<string, int>("WALK",200) , pair<string, int>("STAND" ,500), pair<string, int>("MOVE", mapSize.first / 50) };
		return result;
	}
	bool areThesePointsInSamePlace(pair<float, float> LHS, pair<float, float> RHS, bool ignoreFirst, bool ignoreSecond) {
		bool firstSame = abs(LHS.first - RHS.first) < 0.01;
		bool secondSame = abs(LHS.second - RHS.second) < 0.01;
		if (ignoreFirst and ignoreSecond) {
			return true;
		}
		if (ignoreFirst) {
			return secondSame;
		}
		if (ignoreSecond) {
			return firstSame;
		}
		return firstSame and secondSame;
	}
	bool areThesePointsThisClose(pair<float, float> LHS, pair<float, float> RHS, bool ignoreFirst, bool ignoreSecond, float distance) {
		bool firstSame = abs(LHS.first - RHS.first) < distance;
		bool secondSame = abs(LHS.second - RHS.second) < distance;
		if (ignoreFirst and ignoreSecond) {
			return true;
		}
		if (ignoreFirst) {
			return secondSame;
		}
		if (ignoreSecond) {
			return firstSame;
		}
		return firstSame and secondSame;
	}

	pair<float, float> moveLHSCloserToRHSByValue(pair<float, float> LHS, pair<float, float> RHS, bool ignoreFirst, bool ignoreSecond, float xUnit, float yUnit ) {
		bool firstIsLower = LHS.first < RHS.first;
		bool secondIsLower = LHS.second < RHS.second;
		float XUnit = xUnit;
		float YUnit = yUnit;
		bool firstAreSame = abs(LHS.first - RHS.first) < XUnit;
		bool secondAreSame = abs(LHS.second - RHS.second) < YUnit;
		if (ignoreFirst and ignoreSecond) {
			return LHS;
		}
		if (!ignoreFirst) {
			if (!firstAreSame) {
				if (firstIsLower) {
					LHS.first += XUnit;
				}
				else {
					LHS.first -= XUnit;
				}
			}
		}
		if (!ignoreSecond) {
			if (!secondAreSame) {
				if (secondIsLower) {
					LHS.second += YUnit;
				}
				else {
					LHS.second -= YUnit;
				}
			}
		}
		if (secondAreSame and firstAreSame and !ignoreFirst and !ignoreSecond) {
			return RHS;
		}
		return LHS;
	}

	pair<float, float> moveLHSCloserToRHSAsPercentage(pair<float, float> originalLHS, pair<float, float> LHS, pair<float, float> RHS, bool ignoreFirst, bool ignoreSecond, float unitAsPercentage) {
		float xDistance = abs(originalLHS.first - RHS.first);
		float yDistance = abs(originalLHS.second - RHS.second);

		float currentXDistance = abs(LHS.first - RHS.first);
		float currentYDistance = abs(LHS.second - RHS.second);

		float xUnit = xDistance * unitAsPercentage;
		float yUnit = yDistance * unitAsPercentage;

		return moveLHSCloserToRHSByValue(LHS, RHS, ignoreFirst, ignoreSecond, xUnit, yUnit);
	}

	Map<string, pair<float, float>> getPositionsOfAllObjects() {
		Map<string, pair<float, float>> result;
		for (auto object : currentMap.objects.internalList) {
			result[object.name] = object.positionOnMap;
		}
		return result;
	}
	string decideDirectionDependingOnTwoPoints(pair<float, float> LHS, pair<float, float> RHS) {
		bool front = false; bool back = false; bool left = false; bool right = false;
		if (LHS.first < RHS.first and LHS.second == RHS.second) {
			return "RIGHT";
		}
		if (LHS.first > RHS.first and RHS.second == RHS.second) {
			return "LEFT";
		}
		if (LHS.second < RHS.second) {
			front = true;
		}
		else {
			back = true;
		}
		if (front) {
			return "FRONT";
		}
		if (back) {
			return "BACK";
		}
		return ""; // no change
	}

	int resource;
	float unitOfMovement = 0.5;
	camera activeCamera;
	playerObject playerOnMap;
	mapInstance currentMap;
	string perspective = "FOLLOW_PLAYER";
	string mapPopupTextID = "mappopuptextID";
	Map<string, mapInstance> maps;
	pair<int, int> resolution = controller.actualRenderSizeAsFloat;
	pair<float, float> actualRenderSizeAsFloat = controller.actualRenderSizeAsFloat;
	pair<int, int> mapSize = { 2500,2500 };
	pair<float, float> resolutionAsFloat = { mapSize.first * 100.0f / actualRenderSizeAsFloat.first, mapSize.second * 100.0f / actualRenderSizeAsFloat.second };
};
Explorer explorer;