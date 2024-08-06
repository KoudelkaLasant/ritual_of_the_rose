#pragma once
#include "Structures.h"

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
	void startTimer(string ID) {
		timers[ID] = chrono::steady_clock::now();
	}
	void startClock(string ID) {
		clocks[ID] = chrono::steady_clock::now();
	}
	void startClockIfItDoesNotExist(string ID) {
		if (!clocks.hasKey(ID)) { startClock(ID); }
	}
	int pingTime(string ID) {
		return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - timers[ID]).count();
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
	Map<string, chrono::steady_clock::time_point> timers;
};
Clock CLOCK;

template <typename T>
struct ThreadContainer {
	struct threadManagement {
		bool keepRunning = true;
		Map<string, thread> T;
	};

	Map<string, thread> threads;
	bool run = true;

	Map<string, threadManagement> specialisedThreads;
};
ThreadContainer THREAD;

struct audioManager {
	audioManager() {
		setUpMappings();
		THREAD.threads["AUDIO"] = thread([&]() {
			runThread();
			});
	}
	void setUpMappings() {
		for (double x = 1.0; x > 0.0; x -= 0.1) {
			string result = to_string(x);
			string result2 = to_string(stod(result) / 2.0);
			string key1 = to_string(stod(result));
			string key2 = to_string(stod(result2));
			musicVolumeMappings[key1] = result;
			musicVolumeMappingsReversed[result] = key1;
			soundVolumeMappings[key2] = result2;
			soundVolumeMappingsReversed[result2] = key2;
		}
	}
	struct audioDataNode {
		audioDataNode() {}
		audioDataNode(int source, HWND hwnd, int loop) {
			file = MAKEINTRESOURCE(source);
			sound = cs_load_wav(file.c_str());
			s0 = cs_make_playing_sound(&sound);
			cs_loop_sound(&s0, loop);
			initialised = true;
		}
		~audioDataNode() {
			cs_free_sound(&sound);
		}
		void setVolume(string input) {
			if (!initialised) { return; }
			double x = stod(input);
			if (x < 0) { x = 0.0; }
			s0.volume0 = x;
			s0.volume1 = x;
		}
		void volumeUp() {
			if (!initialised) { return; }
			if (s0.volume0 + 0.1 > 1.0) { return; }
			s0.volume0 += 0.1f;
			s0.volume1 += 0.1f;
		}
		void volumeDown() {
			if (!initialised) { return; }
			if (s0.volume0 <= 0) { return; }
			s0.volume0 -= 0.1f;
			s0.volume1 -= 0.1f;
		}

		int source;
		wstring file;
		cs_loaded_sound_t sound;
		cs_playing_sound_t s0;
		bool initialised;
	};
	void createContext(HWND * hwnd) {
		int frequency = 22000;
		int buffered_samples = 8192; // number of samples internal buffers can hold at once
		ctx = cs_make_context(hwnd, frequency, buffered_samples, 0, NULL);
	}

	/// returns false if no file was found / there's a problem
	bool setUpAudioDataNode(string proxyName, string type) {
		int repeat = 0;
		if (type == "MUSIC") {
			repeat = 1;
		}
		if (!AUDIOBANK.audioMappings.getKeys().contains(proxyName)) {
			ErrorHelper::warning({ "Tried to play a song, but it does not exist.", proxyName.internalString }, false);
			return false;
		}
		String fileName = AUDIOBANK.getAudioPath(proxyName);

		audioDataNode* result = new audioDataNode(fileName, WIN32RESOURCES.theWindow, repeat);

		// failed to create song
		if (result->sound.sample_rate == 0) {
			ErrorHelper::warning({ "Something is wrong with this audio file.", fileName.internalString }, false);
			return false;
		}

		if (type == "MUSIC") {
			musicLibrary[proxyName] = result;
			musicLibrary[proxyName]->setVolume(musicVolume);
		}
		else {
			soundLibrary[proxyName] = result;
			soundLibrary[proxyName]->setVolume(soundVolume);
		}
		return true;
	}
	void playActiveSounds() {
		if (loaded) {
			cs_mix(ctx);
			executeFadeOuts();
		}
	}
	void stopPlayingAllMusic() {
		for (auto x : musicLibrary.internalMap) {
			stopPlayingThisSong(x.first);
		}
	}
	void startPlayingThisSong(string song) {
		if (!musicLibrary.getKeys().contains(song)) {
			if (!setUpAudioDataNode(song, String{ "","MUSIC" })) { return; }
		}
		musicLibrary[song]->s0.paused = 0;
		musicLibrary[song]->setVolume(musicVolume);
		cs_insert_sound(ctx, &musicLibrary[song]->s0);
	}
	void stopPlayingThisSong(string song) {
		if (!musicLibrary.hasKey(song)) {
			return;
		}
		musicLibrary[song]->s0.paused = 1;
	}
	void makeThisSound(string sound) {
		if (!soundLibrary.getKeys().contains(sound)) {
			if (!setUpAudioDataNode(sound, String{ "","SOUND" })) {
				return;
			}
		}
		soundLibrary[sound]->s0.paused = 0;
		cs_insert_sound(ctx, &soundLibrary[sound]->s0);
	}
	void stopMakingThisSound(string song) {
		if (!soundLibrary.hasKey(song)) {
			return;
		}
		soundLibrary[song]->s0.paused = 1;
	}
	void shutDown() {
		for (auto x : musicLibrary.internalMap) {
			cs_free_sound(&x.second->sound);
		}
		for (auto x : soundLibrary.internalMap) {
			cs_free_sound(&x.second->sound);
		}
		cs_shutdown_context(ctx);
	}
	// used for leaving the audio menu
	void tempSaveCurrentAudioSettings() {
		previousMV = musicVolume;
		previousSV = soundVolume;
	}
	void restoreTempAudioSettings() {
		musicVolume = previousMV;
		soundVolume = previousSV;
	}
	void setMusicVolume(string input) {
		musicVolume = input.internalString;
		for (auto y : musicLibrary.internalMap) {
			y.second->setVolume(musicVolume);
		}
	}
	void setSoundVolume(string input) {
		soundVolume = input.internalString;
		for (auto y : soundLibrary.internalMap) {
			y.second->setVolume(soundVolume);
		}
	}
	void startMakingSongFadeOut(string song) {
		songsThatAreFadingOut.addToBackIfNotAlreadyInList(song);
	}
	void executeFadeOuts() {
		if (!CLOCK.hasEnoughTimePassed("FadeOutSongsClock", 10)) { return; }
		List<String> toRemove;
		for (auto x : songsThatAreFadingOut.internalList) {
			musicLibrary[x]->volumeDown();
			if (musicLibrary[x]->s0.volume0 < 0.1) {
				stopPlayingThisSong(x);
				toRemove.push_back(x);
			}
		}
		songsThatAreFadingOut -= toRemove;
	}
	void runThread() {
		while (true) {
			if (!THREAD.run) {
				return;
			}
			if (CLOCK.hasEnoughTimePassed("AUDIOCLOCK", 50)) {
				playActiveSounds();
			}
		}
	}

	bool loaded = false;
	void temporarilySilence() {
		for (auto y : musicLibrary.internalMap) {
			y.second->s0.paused = 1;
		}
		for (auto y : soundLibrary.internalMap) {
			y.second->s0.paused = 1;
		}
	}
	void restoreMusic() {
		for (auto y : musicLibrary.internalMap) {
			y.second->s0.paused = 0;
		}
		for (auto y : soundLibrary.internalMap) {
			y.second->s0.paused = 0;
		}
	}
	List<string> provideTheCurrentAudioSettings(string menuName) {
		List<String> result;
		result.push_back(String{ menuName.internalString, musicVolumeMappingsReversed[musicVolume].internalString });
		result.push_back(String{ menuName.internalString, soundVolumeMappingsReversed[soundVolume].internalString });
		return result;
	}
	Map<string, string> provideIniSettings() {
		Map<String, String> result;
		result[String{ "","MUSICVOLUME" }] = String{ "",to_string((int)(stod(musicVolume) * 100)) };
		result[String{ "","SOUNDVOLUME" }] = String{ "", to_string((int)(stod(soundVolume) * 200)) };
		return result;
	}

	map<string, string> musicVolumeMappings;
	map<string, string> musicVolumeMappingsReversed;
	map<string, string> soundVolumeMappings;
	map<string, string> soundVolumeMappingsReversed;
	List<string> songsThatAreFadingOut;

private:
	Map<string, audioDataNode*> musicLibrary;
	Map<string, audioDataNode*> soundLibrary;
	string clockName = string{ "","AUDIOMANAGERCLOCK" };
	cs_context_t* ctx;
	string musicVolume = to_string(1.0);
	string soundVolume = to_string(stod(musicVolume) / 2); // sounds seem to be really loud, making them half whatever music is as a maximum
	string previousMV = musicVolume; // used if player cancels changes in audio menu
	string previousSV = soundVolume;
};
audioManager AUDIOMANAGER;

