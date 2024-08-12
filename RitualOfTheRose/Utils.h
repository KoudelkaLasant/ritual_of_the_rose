#pragma once
#include "Structures.h"

static string SReplace(string input, string toFind, string toReplace) {
	size_t start_pos = 0;
	while ((start_pos = input.find(toFind, start_pos)) != std::string::npos) {
		input.replace(start_pos, toFind.length(), toReplace);
		start_pos += toReplace.length();
	}
	return input;
}

static List<string> split(string input, string delimiter) {
	size_t pos = 0;
	List<string> results;
	while ((pos = input.find(delimiter)) != string::npos) {
		results.push_back(input.substr(0, pos));
		input.erase(0, pos + delimiter.length());
	}
	if (input.size() > 0) {
		results.push_back(input);
	}
	return results;
}

template <typename T>
T TChange(T target, T amount, T lowerLimit, T upperLimit) {
	target += amount;
	if (target < lowerLimit) { target = lowerLimit; }
	if (target > upperLimit) { target = upperLimit; }
	return target;
}

wstring StringToWString(const string& str)
{
	wstring wstr;
	size_t size;
	wstr.resize(str.length());
	mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
	return wstr;
}

vector<float> convertIntColour(vector<int> colour) {
	float factor = 255.0f;
	vector<float> result = { 
		float(colour.at(0)) / factor,
		float(colour.at(1)) / factor,
		float(colour.at(2)) / factor,
		float(colour.at(3)) / factor,
	};
	return result;
}

class Random {
public:
	Random() {
#pragma warning (push)
#pragma warning(disable: 4244)
		srand(time(NULL));
#pragma warning (pop)
	}
	static int getRandom(int lower, int higher) {
		int result = rand() % (higher + 1) + lower;
		if (result > higher) { result = higher; }
		return result;
	}
	string createUniqueID() {
		string result = to_string(nameGenerator);
		nameGenerator++;
		if (nameGenerator > 9999999) {
			nameGenerator = 0;
		}
		return result;
	}
	template <typename T> T getRandom(List<T> theList) {
		int index = Random::getRandom(0, theList.size() - 1);
		return theList.at(index);
	}

private:
	unsigned int nameGenerator = 0;
};
Random RANDOM;

class RuntimeArgs {
public:
	RuntimeArgs() {}
	List<string> convertCmdToStringList(LPWSTR lpCmdLine) {
		if (wcslen(lpCmdLine) == 0) {
			return {};
		}
		int args;
		LPWSTR* szArglist;
		szArglist = CommandLineToArgvW(lpCmdLine, &args);
		if (szArglist == NULL) {
			return {};
		}
		LocalFree(szArglist);
		List<string> results;
		for (int x = 0; x < args; x++) {
			results.push_back(SReplace(string(CW2A(szArglist[x])), "-", ""));
		}
		return results;
	}
	void parse_args(LPWSTR lpCmdLine) {
		raw_args = convertCmdToStringList(lpCmdLine);
		int x = 0;
		for (auto& raw_arg : raw_args.internalList) {
			int position = raw_arg.find("=");
			if (position != string::npos) {
				parsed_args[raw_arg.substr(0, position)] = raw_arg.substr(position + 1, raw_arg.size());
			}
		}
	}
	string get(string key) {
		return parsed_args[key];
	}
	List<string> raw_args;
	Map<string, string> parsed_args = List<pair<string,string>>({pair<string, string>("mode","game")});
};
RuntimeArgs Args;

class Clock {
public:
	struct timeAndDate {
		string time;
		string date;
		bool operator< (const timeAndDate&) {
			// finish this
			return true;
		}
	};
	bool hasEnoughTimePassed(string ID, int amountOfTimeInMillis) {
		chrono::milliseconds timeInterval(amountOfTimeInMillis);
		chrono::steady_clock::time_point timeNow = chrono::steady_clock::now();
		chrono::milliseconds timeDifference = chrono::duration_cast<chrono::milliseconds>(timeNow - clocks[ID]);
		if (timeDifference > timeInterval) {
			clocks[ID] = timeNow;
			return true;
		}
		return false;
	}
	void startClock(string ID) {
		clocks[ID] = chrono::steady_clock::now();
	}
	void startClockIfItDoesNotExist(string ID) {
		if (!clocks.hasKey(ID)) { startClock(ID); }
	}
	int pingTime(string ID) {
		return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - clocks[ID]).count();
	}
	void waitForXMiliseconds(int duration) {
		string name = "WaitingForNothing";
		startClock(name);
		while (!hasEnoughTimePassed(name, duration)) {}
	}
	timeAndDate getCurrentTimeAndDate() {
		timeAndDate result;
		SYSTEMTIME st;
		GetLocalTime(&st);
		result.date = to_string(st.wYear) + "/" + to_string(st.wMonth) + "/" + to_string(st.wDay);
		result.time = to_string(st.wMinute);
		if (result.time.size() == 1) {
			result.time = "0" + result.time;
		}
		result.time = to_string(st.wHour) + ":" + result.time;
		return result;
	}

	chrono::milliseconds howLongDidThisTake(chrono::milliseconds start, chrono::milliseconds end) {
		return chrono::duration_cast<chrono::milliseconds>(end - start);
	}
	Map<string, chrono::steady_clock::time_point> clocks;
};
Clock CLOCK;

class audioManager {
public:
	audioManager() {
		SFXCollections["PARCHMENT"] = List<int>({ 
			PARCHMENT_WAV_1,
			PARCHMENT_WAV_2,
			PARCHMENT_WAV_3,
			PARCHMENT_WAV_4,
			PARCHMENT_WAV_5,
			PARCHMENT_WAV_6,
			PARCHMENT_WAV_7,
			PARCHMENT_WAV_8,
			PARCHMENT_WAV_9,
			PARCHMENT_WAV_10,
			PARCHMENT_WAV_11,
			PARCHMENT_WAV_12,
			PARCHMENT_WAV_13,
		
		});
		preloadSFX();
		soloud.init();
	}
	~audioManager() {
		unloadAllAudio();
		soloud.deinit();
	}
	void preloadSFX() {
		for (auto const & x : SFXCollections["PARCHMENT"].internalList) {
			loadAudio(x);
		}
	}

	void loadSound(int resource) {
		if (!loadedResources.internalMap[resource]) {
			loading_mutex.lock();
			thread([&]() {loadAudio(resource); }).detach();
			loading_mutex.unlock();
		}
	}
	bool isThisAudioLoaded(int resource) {
		return loadedBytes.getKeys().contains(resource);
	}
	void playSound(int resource, float volume, bool loop, bool fadein) {
		string clockName = "LOADING_" + to_string(resource);
		CLOCK.startClock("LOADING_" + to_string(resource));
		while (!isThisAudioLoaded(resource)) {
			Sleep(100);
			if (CLOCK.hasEnoughTimePassed(clockName, 50000)) {
				return;
			}
		}
		loadedSFX[resource].setVolume(volume);
		if (!fadein) {
			loadedSFX[resource].setVolume(volume);
		}
		else {
			loadedSFX[resource].setVolume(0);
		}
		loadedSFX[resource].setLooping(loop);
		int handle = soloud.play(loadedSFX[resource]);
		if (fadein) {
			soloud.fadeVolume(handle, volume, 5.0f);
		}
	}
	void loadAudio(int resource) {
		HRSRC hResInfo = FindResource(NULL, MAKEINTRESOURCE(resource), L"WAVE");
		HGLOBAL hResData = LoadResource(NULL, hResInfo);
		LPVOID lpAddress = LockResource(hResData);
		LPBYTE lpData = (LPBYTE)LockResource(hResData);
		DWORD dwSize = SizeofResource(NULL, hResInfo);

		audio_mutex.lock();
		BytesLoadingInProgress[resource] = new unsigned char[dwSize];
		sizeOfLoadedBytes[resource] = dwSize;
		for (int x = 0; x < dwSize; x++) {
			BytesLoadingInProgress[resource][x] = lpData[x];
		}
		int example = sizeof(BytesLoadingInProgress[resource]) * sizeof(unsigned char);

		loadedSFX[resource] = SoLoud::Wav();
		loadedSFX[resource].loadMem(BytesLoadingInProgress[resource], sizeOfLoadedBytes[resource], true, true);
		loadedResources[resource] = true;
		loadedBytes[resource] = BytesLoadingInProgress[resource];
		audio_mutex.unlock();
	}
	void unloadAllAudio() {
		loadedSFX.clear();
		for (auto const& [key, val] : loadedSFX.internalMap) {
			delete loadedBytes[key];
			loadedBytes[key] = 0;
		}
		loadedBytes.clear();
		sizeOfLoadedBytes.clear();
		loadedResources.clear();
	}
	void playRandomSFXFromThisCollection(string colName, float volume) {
		int resource = RANDOM.getRandom(SFXCollections[colName]);
		loadSound(resource);
		playSound(resource, volume, false, false);
	}

	Map<int, bool> loadedResources;
	List<int> inTheLoadingQueue;
	Map<int, unsigned char*> BytesLoadingInProgress;
	Map<int, unsigned char*> loadedBytes;
	Map<int, DWORD> sizeOfLoadedBytes;
	Map<int, SoLoud::Wav> loadedSFX;
	Map<string, List<int>> SFXCollections;
	SoLoud::Soloud soloud;
	mutex audio_mutex;
	mutex loading_mutex;
	mutex soloud_mutex;
	int currentSongResource;
};
audioManager audio;

class UserInput {
public:
	UserInput() {
		allKeyboardButtons.internalMap = {{8, VK_BACK},{9,VK_TAB},{12,VK_CLEAR},{13,VK_RETURN},{16,VK_SHIFT},{17,VK_CONTROL},{18,VK_MENU},{19,VK_PAUSE},{20,VK_CAPITAL},{21,VK_KANA},{27,VK_ESCAPE},{30,VK_ACCEPT},{32,VK_SPACE},{33,VK_PRIOR},{34,VK_NEXT},{35,VK_END},{36,VK_HOME},{37,VK_LEFT},{38,VK_UP},{39,VK_RIGHT},{40,VK_DOWN},{41,VK_SELECT},{42,VK_PRINT},{43,VK_EXECUTE},{44,VK_SNAPSHOT},{46,VK_DELETE},{48,0},{49,1},{50,2},{51,3},{52,4},{53,5},{54,6},{55,7},{56,8},{57,9},{65,65},{66,66},{67,67},{68,68},{69,69},{70,70},{71,71},{72,72},{73,73},{74,74},{75,75},{76,76},{77,77},{78,78},{79,79},{80,80},{81,81},{82,82},{83,83},{84,84},{85,85},{86,86},{87,87},{88,88},{89,89},{90,90},{96,VK_NUMPAD0},{97,VK_NUMPAD1},{98,VK_NUMPAD2},{99,VK_NUMPAD3},{100,VK_NUMPAD4},{101,VK_NUMPAD5},{102,VK_NUMPAD6},{103,VK_NUMPAD7},{104,VK_NUMPAD8},{105,VK_NUMPAD9},{106,VK_MULTIPLY},{107,VK_ADD},{108,VK_SEPARATOR},{109,VK_SUBTRACT},{110,VK_DECIMAL},{111,VK_DIVIDE},{112,VK_F1},{113,VK_F2},{114,VK_F3},{115,VK_F4},{116,VK_F5},{117,VK_F6},{118,VK_F7},{119,VK_F8},{120,VK_F9},{121,VK_F10},{122,VK_F11},{123,VK_F12},{124,VK_F13},{125,VK_F14},{126,VK_F15},{127,VK_F16},{128,VK_F17},{129,VK_F18},{130,VK_F19},{131,VK_F20},{132,VK_F21},{133,VK_F22},{134,VK_F23},{135,VK_F24},{144,VK_NUMLOCK},{145,VK_SCROLL},{160,VK_LSHIFT},{161,VK_RSHIFT},{162,VK_LCONTROL},{163,VK_RCONTROL}};
		upperToLowerCase.internalMap = {{"A","a"},{"B","b"},{"C","c"},{"D","d"},{"E","e"},{"F","f"},{"G","g"},{"H","h"},{"I","i"},{"J","j"},{"K","k"},{"L","l"},{"M","m"},{"N","n"},{"O","o"},{"P","p"},{"Q","q"},{"R","r"},{"S","s"},{"T","t"},{"U","u"},{"V","v"},{"W","w"},{"X","x"},{"Y","y"},{"Z","z"} };
		allPossibleMouseStatuses.internalList = {WM_LBUTTONDOWN, WM_LBUTTONDBLCLK, WM_LBUTTONUP, WM_MBUTTONDOWN, WM_MBUTTONUP, WM_MOUSEHWHEEL, WM_MOUSEMOVE, WM_MOUSEWHEEL };
		allPossibleMouseInputs.internalList = {MK_CONTROL, MK_LBUTTON, MK_MBUTTON, MK_RBUTTON, MK_SHIFT, MK_XBUTTON1, MK_XBUTTON2};
	}
	void acceptAllInput(UINT msg, WPARAM wParam, LPARAM lParam) {
		mouseInstructionsInOrder.clear();
		keysPressedInOrder.clear();
		acceptMousePosition(msg, lParam);
		for (auto const &  [key, value] : allKeyboardButtons.internalMap) {
			if (hasThisBeenPressed(key)) {
				keysPressedInOrder.addToBackIfNotAlreadyInList(to_string(key));
			}
		}
	}
	void acceptMousePosition(UINT msg, LPARAM lParam) {
		if (msg == WM_MOUSEMOVE) {
			acceptMouseMovePosition(lParam);
			mouseInstructionsInOrder.push_back("MouseMove");
		}
		if (msg ==WM_LBUTTONDOWN) {
			acceptMouseClickPosition(lParam);
			mouseInstructionsInOrder.push_back("LButtonDown");
		}
		if (msg == WM_LBUTTONUP) {
			acceptMouseUnclickPosition(lParam);
			mouseInstructionsInOrder.push_back("LButtonUp");
		}
	}
	void acceptMouseClickPosition(LPARAM lParam) {
		mouseClickPosition = whereIsCursor(lParam);
	}
	void acceptMouseMovePosition(LPARAM lParam) {
		mouseMovePosition = whereIsCursor(lParam);
	}
	void acceptMouseUnclickPosition(LPARAM lParam) {
		mouseUnclickPosition = whereIsCursor(lParam);
	}
	pair<int, int> whereIsCursor(LPARAM lParam) {
		pair<int, int> results;
		results.first = LOWORD(lParam);
		results.second = HIWORD(lParam);
		return results;
	}
	bool userPressedOneOfThese(list<int> codes) {
		bool result = false;
		for (auto const& x : codes) {
			if (keysPressedInOrder.contains(to_string(x))) {
				result = true;
			}
		}
		return result;
	}

	string controllerDebug() {
		string result = "Keys Pressed: ";
		for (auto const& x : keysPressedInOrder.internalList) {
			result += x + " ";
		}
		result += "\nMouse Instructions Interpreted: ";
		for (auto const& x : mouseInstructionsInOrder.internalList) {
			result += x + " ";
		}
		result += "\nMouse Clicked Position: ";
		result += "x = " + to_string(mouseClickPosition.first) + " y = " + to_string(mouseClickPosition.second);
		result += "\nMouse Moved Position: ";
		result += "x = " + to_string(mouseMovePosition.first) + " y = " + to_string(mouseMovePosition.second);
		result += "\nMouse Unclicked Position: ";
		result += "x = " + to_string(mouseUnclickPosition.first) + " y = " + to_string(mouseUnclickPosition.second);
		return result;
	}
	bool hasThisBeenPressed(int code) {
		return (GetKeyState(code) & 0x8000) || (1 << 15) & GetAsyncKeyState(code);
	}
	List<string> keysPressedInOrder;
	List<string> mouseInstructionsInOrder;
	Map<int, WPARAM> allKeyboardButtons;
	Map<string, string> upperToLowerCase;
	List<int> allPossibleMouseInputs;
	List<int> allPossibleMouseStatuses;
	pair<int, int> mouseMovePosition;
	pair<int, int> mouseClickPosition;
	pair<int, int> mouseUnclickPosition;
};
UserInput controller;

class JSONIO {
public:
	json read_file(filesystem::path path){
		ifstream f(path);
		return json::parse(f);
	}
};
JSONIO jsonio;

class SaveContainer {
public:
	class SaveFile {
	public:
		SaveFile() {}
		SaveFile(filesystem::path filepath) {
		json data = jsonio.read_file(filepath);
		party = data["party"];
		flags = data["flags"];
		knownSkills = data["known skills"];
		equippedSkills = data["equipped skills"];
		equippedItems = data["equipped items"];
		inventory = data["inventory"];
		itemsSold = data["items sold"];
		attributeInvestments = data["attribute investments"];
		stats = data["stats"];
		money = data["money"];
		loaded = true;
		}
		void saveToDisk(filesystem::path filepath) {
			json data;
			data["party"] = party;
			data["flags"] = flags;
			data["equipped items"] = equippedItems;
			data["equipped skills"] = equippedSkills;
			data["known skills"] = knownSkills;
			data["inventory"] = inventory;
			data["items sold"] = itemsSold;
			data["attribute investments"] = attributeInvestments;
			data["stats"] = stats;
			data["money"] = money;
			
			ofstream file(filepath);
			file << data;
		}
		SaveFile& operator=(const SaveFile& RHS) {
			party = RHS.party;
			flags = RHS.flags;
			equippedItems = RHS.equippedItems;
			equippedSkills = RHS.equippedSkills;
			knownSkills = RHS.knownSkills;
			inventory = RHS.inventory;
			itemsSold = RHS.itemsSold;
			attributeInvestments = RHS.attributeInvestments;
			stats = RHS.stats;
			money = RHS.money;
			loaded = true;
			return *this;
		}

		list<string> party;
		map<string, bool> flags;
		map<string, map<string, string>> equippedItems;
		map<string, map<string, string>> equippedSkills;
		map<string, list<string>> knownSkills;
		list<string> inventory;
		list<string> itemsSold;
		map<string, map<string, int>> attributeInvestments;
		map<string, int> stats;
		int money;
		const int partyLimit = 4;
		bool loaded = false;
	};

	void load(filesystem::path filepath) {
		current = SaveFile(filepath);
	}
	bool loadedSomething() {
		return (current.loaded);
	}
	string getCurrentMainCharacter() {
		if (!current.loaded) {
			ErrorHelper::warning("Can't play without a party.", false);
		}
		return current.party.front();
	}

	SaveFile current;
	Map<int, SaveFile> slots;
	const int slotLimit = 9;
};
SaveContainer saveContainer;

class ImageLookup {
public:
	ImageLookup() {
		animationFrames["Angela Fleuret"] = {};
		animationFrames["Angela Fleuret"]["STAND_FRONT"].internalList = { ANGELA_STAND_FRONT_1, ANGELA_STAND_FRONT_2 };
		animationFrames["Angela Fleuret"]["STAND_BACK"].internalList = { ANGELA_STAND_BACK_1, ANGELA_STAND_BACK_2 };
		animationFrames["Angela Fleuret"]["STAND_LEFT"].internalList = { ANGELA_STAND_LEFT_1, ANGELA_STAND_LEFT_2 };
		animationFrames["Angela Fleuret"]["STAND_RIGHT"].internalList = { ANGELA_STAND_RIGHT_1, ANGELA_STAND_RIGHT_2 };
		animationFrames["Angela Fleuret"]["WALK_BACK"].internalList = { ANGELA_WALK_BACK_1, ANGELA_WALK_BACK_2, ANGELA_WALK_BACK_3, ANGELA_WALK_BACK_2, };
		animationFrames["Angela Fleuret"]["WALK_FRONT"].internalList = { ANGELA_WALK_FRONT_1, ANGELA_WALK_FRONT_2, ANGELA_WALK_FRONT_3, ANGELA_WALK_FRONT_2, };
		animationFrames["Angela Fleuret"]["WALK_LEFT"].internalList = { ANGELA_WALK_LEFT_1, ANGELA_WALK_LEFT_2, ANGELA_WALK_LEFT_3, ANGELA_WALK_LEFT_2, };
		animationFrames["Angela Fleuret"]["WALK_RIGHT"].internalList = { ANGELA_WALK_RIGHT_1, ANGELA_WALK_RIGHT_2, ANGELA_WALK_RIGHT_3, ANGELA_WALK_RIGHT_2 };
	}
	string getSequenceAsString(string character, string action) {
		// get it as one string so it can be used in event data
		string result;
		for (auto const& x : animationFrames[character][action].internalList) {
			result += to_string(x) + " ";
		}
		return result;
	}

	Map <string, Map<string, List<int>>> animationFrames;
};
ImageLookup imageLookup;

class Explorer {
public:
	// map positions start at 0,0 = top left
	struct mapObject {
		string name = "";
	};
	struct playerObject {
		pair<int, int> position;
	};
	struct camera {
		pair<int, int> position;
	};
	map<string, pair<float, float>> getUpdatedMapImagePositions() {
		map<string, pair<float, float>> result;
		if (perspective == "FOLLOW_PLAYER") {
			activeCamera.position = playerOnMap.position;
		}
		result["player image position"] = { 50,50 };
		result["map position"] = { 50 + (50 - playerOnMap.position.first) * resolutionAsFloat.first / 100.0f , 50 + (50 - playerOnMap.position.second) * resolutionAsFloat.second / 100.0f };

		return result;
	}

	void tryToMovePlayer(string direction) {
		// don't move if destination isn't acceptable
		pair<int, int> toMove;
		int unit = 1;
		if (direction == "BACK") { toMove.second -= unit; }
		if (direction == "FRONT") { toMove.second += unit; }
		if (direction == "LEFT") { toMove.first -= unit; }
		if (direction == "RIGHT") { toMove.first += unit; }
		playerOnMap.position.first = TChange(playerOnMap.position.first, toMove.first, 0, 100);
		playerOnMap.position.second = TChange(playerOnMap.position.second, toMove.second, 0, 100);
	}

	int resource;
	camera activeCamera;
	playerObject playerOnMap;
	mapObject currentMap;
	string perspective = "FOLLOW_PLAYER";
	pair<float, float> resolutionAsFloat = { 195.3125f, 347.2222f};
	pair<int, int> resolution = { 1280, 720 };
	pair<int, int> mapSize = { 2500,2500 };
};
Explorer explorer;