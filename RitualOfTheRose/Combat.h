#pragma once

#include "Menu.h"

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
		influenceLookups["Minor ArmsBOOST"] = 0.06;
		influenceLookups["ArmsBOOST"] = 0.06;

		// skillValueBoosts
		Map<string, wstring> types; types.internalMap = strings["ENG"]["Type Names"];
		for (auto type : types.getKeys().internalList) {
			influenceLookups[type + "COST"] = 0.05;
			influenceLookups[type + "ACTIVATION"] = 0.05;
			influenceLookups[type + "RECHARGE"] = 0.05;
		}

		// armour boosts
		influenceLookups["ARMOURVSPHYSICAL"] = 0.05;
		influenceLookups["ARMOURVSELECTRIC"] = 0.05;

		AttributesInOrder = { "VITALITY","PIETY","STRENGTH", "INTELLIGENCE", "AGILITY","LUCK" };
		statsInOrder = { "LIFE","ENERGY","ENERGYREGEN", "SPEED" };
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
		defaultAttInvestments["Father Michelet"] = {
			{"STRENGTH" , 0},
			{"INTELLIGENCE" ,0},
			{"VITALITY", 2},
			{"PIETY", 17},
			{"AGILITY", 0},
			{"LUCK", 1}
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
			{"1", "Minor Arms"},
			{"2", "Umbromancy"},
		};
		defaultSkillTreeChoices["Hernando Pizarro"] = {
			{"1", "Arms"},
			{"2", "Wayfaring"},
		};
		defaultSkillTreeChoices["Father Michelet"] = {
			{"1", "Cleromancy"},
			{"2", "Hagiomancy"},
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
		defaultSkillChoices["Father Michelet"] = {
			{"1", "Heal Wounds"},
			{"2", "Strength of Reason"},
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
		defaultEquipment["Father Michelet"] = {
			{"Weapon", "Withered Secespita"},
			{"Armour" , "Vatican Vestiments"},
			{"Accessory", "Cross of St Jeanne-Marie"}
		};


		defineAllSkills();
		defineAllEquipment();
		defineAllCombatants();
		defineAllTeams();
		defineAllEffectDefinitions();

		layerScaleLookup["TEAM1"] = "1.0";
		layerScaleLookup["TEAM1ALLIES"] = "0.9";
		layerScaleLookup["TEAM2ALLIES"] = "0.8";
		layerScaleLookup["TEAM2"] = "0.7";
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
		int getResultantValue(Combat& combat, Map<string, int> percentInfluences, Map<string, int> flatInfluences) {
			float result = baseValue;
			float multiplier = 1.0;
			if (!goesUpwards) {
				multiplier *= -1;
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
	class Effect {
		// has an affect on combat stats (for example, holyboost)
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
			if (tag.find("ARMOURVS") != -1) {
				result = strings[language]["Item Effect Strings"]["ARMOUR"];
				string typeName = SReplace(tag, "ARMOURVS", "");
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

		float getPowerForCombat(Combat& combat) {
			// assuming power ups are by percentage
			return combat.influenceLookups[tag] * 100 * influence;
		}

		string tag;
		float influence;
		bool goesUp;
		bool isFlat; // or a %
	};
	class Equipment {
	public:
		Equipment() {}
		Equipment(Combat & combat, string _uniqueID, string _category, int _imageSource, List<Effect> _powers, string _textColour, int _price) {
			uniqueID = _uniqueID;
			category = _category;
			imageSource = _imageSource;
			powers = _powers;
			textColour = _textColour;
			price = _price;
			imageLookup.animationFrames["Codex"][_uniqueID].internalList = {_imageSource};
			for (auto [language, val] : strings) {
				wstring baseDescription = strings[language]["Item Descriptions"][_uniqueID];
				wstring stats = printout(language, *&combat);
				wstring result = baseDescription + L"\n\n" + stats;
				strings[language]["Item Descriptions For Codex"][_uniqueID] = result;
				strings[language]["Codex " + _category][_uniqueID] = strings[language]["Item Names"][_uniqueID];
			}
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
		List<Combat::Effect> powers;
		int imageSource;
		int price;
	};
	class EffectObject {
		// is an object which is applied to a combatant (for example, poison)
	public:
		EffectObject() {}
		EffectObject(string _uniqueID, string _type, int _imageRes, string _logicName, bool _infinite, List<string> _effectNames, List<string> _triggers) {
			uniqueID = _uniqueID;
			imageRes = _imageRes;
			logicName = _logicName;
			infinite = _infinite;
			triggers = _triggers;
			type = _type;
			effectNames = _effectNames;
		}
		void tick() {
			if (!infinite) {
				roundsLeft = TChange(roundsLeft, -1, -1, 999);
			}
		}
		bool expired() {
			return !infinite and roundsLeft < 0;
		}
		int getBorder() {
			int result = NORMALSKILLBORDER;
			if (type == "BANE") {
				result = BANEBORDER;
			}
			if (type == "BOON") {
				result = BOONBORDER;
			}
			return result;
		}

		wstring getDescription(Combat& combat, string language) {
			if (combat.skillDefinitions.getKeys().contains(uniqueID)) {
				wstring result = strings[language]["Skill Descriptions"][uniqueID];
				for (auto value : values.getKeys().internalList) {
					wstring name = StringToWString(toUpper(value));
					wstring replacement = L"$" + name + L"_" + StringToWString(uniqueID) + L"$";
					result = WSReplace(result, replacement, to_wstring(values[value]));
				}
				return result;
			}
			wstring result = strings[language]["Effect Descriptions"][uniqueID];
			for (auto value : values.getKeys().internalList) {
				wstring name = StringToWString(toUpper(value));
				result = WSReplace(result, L"$" + name + L"$", to_wstring(values[value]));
			}
			return result;
		}

		wstring getName(Combat& combat, string language) {
			if (combat.skillDefinitions.getKeys().contains(uniqueID)) {
				return strings[language]["Skill Names"][uniqueID];
			}
			wstring result = strings[language]["Effect Names"][uniqueID];
			return result;
		}

		string owner; // who made the effect
		string uniqueID;
		string type; // BOON, BANE or PERM (positive, negative, permanent can't be remove effect)
		int imageRes;
		int charges = -1; // some effects have charges and will end instantly if charges reaches zero
		string target;
		string logicName;
		int duration = -1;
		int roundsLeft = -1;
		bool infinite = false;
		Map<string, int> values;
		List<string> effectNames;
		List<string> triggers; // trigger every turn, upon creation, when it expires, etc
		int timeCreated;
	};
	class EffectObjectInstance {
	public:
		EffectObjectInstance() {}
		EffectObjectInstance(Combat& combat, string effectName, string who, string target, Map<string, int> values) {
			e = combat.allEffectDefinitions[effectName];
			e.owner = who;
			e.target = target;
			e.values = values;
			if (values.hasKey("duration")) {
				e.duration = values["duration"];
				e.roundsLeft = e.duration;
			}
			if (values.hasKey("charges")) {
				e.charges = values["charges"];
			}
			if (e.duration == -1) {
				e.infinite = true;
			}
			uniqueID = effectName + "_" + who;
		}

		bool operator==(const EffectObjectInstance* RHS) {
			return RHS->e.timeCreated == e.timeCreated;
		}
		bool operator<(const EffectObjectInstance* RHS) {
			return RHS->e.timeCreated < e.timeCreated;
		}
		bool operator>(const EffectObjectInstance* RHS) {
			return RHS->e.timeCreated > e.timeCreated;
		}

		Map<string, int> executeDirectly(Combat& combat) {
			// possible that not all effects will be triggered like this
			Map<string, int> result;
			for (auto subeffect : e.effectNames.internalList) {
				if (subeffect == "Life Drain") {
					result["LIFESTEAL_SINGLE_UNHOLY"] = e.values["power"];
				}
				if (subeffect == "DAMAGE_SINGLE_HOLY") {
					result["DAMAGE_SINGLE_HOLY"] = e.values["power"];
				}
				if (subeffect == "LIFEHEAL_SINGLE_HOLY") {
					result["LIFEHEAL_SINGLE_HOLY"] = e.values["power"];
				}
				if (subeffect == "Rainstorm") {
					result["APPLY_WET_ALL"] = e.values["duration"];
				}
				if (subeffect == "Sandstorm") {
					result["APPLY_DUSTY_ALL"] = e.values["duration"];
				}
				if (subeffect == "Heatwave") {
					result["APPLY_PARCHED_ALL"] = e.values["duration"];
				}
				if (subeffect == "Pressure Front") {
					result["APPLY_STATIC_ALL"] = e.values["duration"];
				}
				if (subeffect == "Healing Rain") {
					result["LIFEHEAL_AOE_WATER"] = e.values["power"];
				}
				if (subeffect == "Ice Storm") {
					result["DAMAGE_SINGLE_COLD"] = e.values["power"];
				}
				if (subeffect == "Cataclysm") {
					result["DAMAGE_SINGLE_FIRE"] = e.values["power"];
				}
				if (subeffect == "Chaos Storm") {
					result["MANABURN_SINGLE_SHADOW"] = e.values["power"];
				}
				if (subeffect == "Great Gospel") {
					result["LIFEHEAL_SINGLE_HOLY"] = e.values["power"];
				}
				if (subeffect == "Apostle of Patience") {
					result["LIFEHEAL_SINGLE_HOLY"] = e.values["power"];
				}
				if (subeffect == "Ice AgeW") {
					result["APPLY_Ice Age_ALL"] = e.values["duration"];
				}
				if (subeffect == "Global WarmingW") {
					result["APPLY_Global Warming_ALL"] = e.values["duration"];
				}
				if (subeffect == "TempestW") {
					result["APPLY_Tempest_ALL"] = e.values["duration"];
				}
				if (subeffect == "DroughtW") {
					result["APPLY_Drought_ALL"] = e.values["duration"];
				}
			}
			return result;
		}

		EffectObject e;
		string uniqueID; // effect name + who
	};
	class Skill {
	public:
		Skill() {}
		Skill(string _uniqueID, string _displayName, string _skillTree, int _imageSource, int _baseCost, int _baseCastingTime, int _baseRecharge, string _targetLogic, List<string> _skillLogicNames, List<string> _skillTypeTags, Map<string, PowerValue> _powerValues, List<string> _purposes, int _audioSource) {
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
			targetLogic = _targetLogic;
			audioSource = _audioSource;
			timeToRecharge = 0;
		}



		Skill makeSkillInstance() {
			Skill result(uniqueID, displayName, skillTree, imageSource, baseCost, baseCastingTime, baseRecharge, targetLogic, skillLogicNames, skillTypeTags, powerValues, purposes, audioSource);
			return result;
		}
		bool canThisSkillBeUsed(Combat& combat) {
			bool validTargets = areThereAnyValidTargets(*&combat);
			bool notDisabled = !disabled;
			bool recharging = false;
			if (timeToRecharge > 0) {
				recharging = true;
			}
			bool enoughEnergy = canPayEnergyCost(*&combat);
			if (skillLogicNames.contains("RESURRECT_SINGLE_HOLY") and combat.currentBattle->doesTargetXHaveStatusY(combat.currentBattle->currentRound.whoseTurnIsIt(), "Entomb Spirit")) {
				return false;
			}
			return validTargets and notDisabled and !recharging and enoughEnergy;
		}
		bool areThereAnyValidTargets(Combat& combat) {
			Battle* battle = combat.currentBattle;
			if (battle == NULL) { return false; /* Should never happen hopefully*/ }
			string currentActor = battle->currentRound.whoseTurnIsIt();
			Combat::CombatantInstance* actor = battle->getThisCombatant(currentActor);
			List<Combat::CombatantInstance*> validTargets = battle->getAllValidTargetsForThisSkill(*&combat, actor, *this);
			return validTargets.size() > 0;
		}
		bool canPayEnergyCost(Combat& combat) {
			Battle* battle = combat.currentBattle;
			if (battle == NULL) { return false; }
			string currentActor = battle->currentRound.whoseTurnIsIt();
			Combat::CombatantInstance* actor = battle->getThisCombatant(currentActor);
			int cost = actor->c.getSkillCost(*this, true, *&combat);
			return cost <= actor->c.combatStats["CURRENTENERGY"];
		}
		bool isThisABuffICanUseOnSomeone(Combat& combat) {
			if (!canThisSkillBeUsed(*&combat)) { return false; }
			if (targetLogic.find("ALLY") == -1) { return false; }
			for (auto s : skillLogicNames.internalList) {
				if (s.find("APPLY") != -1) {
					return true;
				}
			}
			return false;
		}
		bool isThisABuffICanUseOnMyselfOnly(Combat& combat) {
			if (!canThisSkillBeUsed(*&combat)) { return false; }
			if (purposes.contains("ENCHANTSELF") and !purposes.contains("ENCHANTALLY") or !purposes.contains("ENCHANTOTHERALLY")) {
				return true;
			}
		}
		bool isThisACurseICanUseOnSomeone(Combat& combat) {
			if (!canThisSkillBeUsed(*&combat)) { return false; }
			if (targetLogic.find("FOE") == -1) { return false; }
			for (auto s : skillLogicNames.internalList) {
				if (s.find("APPLY") != -1) {
					return true;
				}
			}
			return false;
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
		int timeToRecharge;
		int timeToCast;
		bool disabled = false;
		int audioSource;
		string targetLogic;
		List<string> skillLogicNames;
		List<string> skillTypeTags;
		Map<string, PowerValue> powerValues;
		List<string> purposes; // used by the AI to decide which skill to use
	};
	class CombatEvent {
		// taking damage, gaining life, applying xyz
	public:
		CombatEvent() {};
		CombatEvent(string _logic, string _type, string _sourceName, string _originalUser, List<string> _combatantsAffected, Map<string, string> _sData, Map<string, int> _vData) {
			logic = _logic;
			type = _type;
			sourceName = _sourceName;
			originalUser = _originalUser;
			combatantsAffected = _combatantsAffected;
			sData = _sData;
			vData = _vData;
		}
		CombatEvent(Combat& combat, EffectObjectInstance* e) {
			logic = e->e.logicName;
			type = e->e.type;
			sourceName = e->e.uniqueID;
			originalUser = e->e.owner;
			combatantsAffected = e->e.target;
			vData = e->executeDirectly(*&combat);
			sData["success"] = "1";
		}

		string getTemporaryID() {
			// use this to stop the game from playing the same animation multiple times in a row when one skill has multiple effects that are played sequentially
			string result = sourceName + "_" + originalUser;
			return result;
		}

		string logic;
		string type; // skill being used or an effect that already exists (Skill object or EffectObject instance)
		string sourceName;
		string originalUser;
		List<string> combatantsAffected;
		Map<string, string> sData;
		Map<string, int> vData;
	};
	class Combatant {
	public:
		Combatant() {}
		Combatant(string _uniqueID, string _displayName, Map<string, int> _intData, Map<string, Map<string, string>> _data) {
			defaultStats["LIFE"] = PowerValue("LIFE", 100, -999, 9999, true, list<string>({ "VITALITY" }));
			defaultStats["ENERGY"] = PowerValue("ENERGY", 30, -999, 9999, true, list<string>({ "PIETY" }));
			defaultStats["LIFEREGEN"] = PowerValue("LIFEREGEN", 0, 0, 9999, true, list<string>({ "LIFEREGENPLUS" }));
			defaultStats["ENERGYREGEN"] = PowerValue("ENERGYREGEN", 3, 0, 9999, true, list<string>({ "PIETY", "ENERGYREGENPLUS" }));
			defaultStats["SPEED"] = PowerValue("SPEED", 10, 0, 9999, true, list<string>({ "AGILITY" }));
			uniqueID = _uniqueID;
			displayName = _displayName;
			data = _data;
			intData = _intData;
		}

		void takeDamage(int value) {
			// armour, damage modifiers already calculated so apply directly
			combatStats["CURRENTLIFE"] = TChange(combatStats["CURRENTLIFE"], value * -1, 0, combatStats["LIFE"]);
		}
		void takeManaDamage(int value) {
			// armour, damage modifiers already calculated so apply directly
			combatStats["CURRENTENERGY"] = TChange(combatStats["CURRENTENERGY"], value * -1, 0, combatStats["ENERGY"]);
		}
		void payEnergyCost(int value) {
			combatStats["CURRENTENERGY"] = TChange(combatStats["CURRENTENERGY"], value * -1, 0, combatStats["ENERGY"]);
		}
		void beHealed(int value) {
			// armour, damage modifiers already calculated so apply directly
			combatStats["CURRENTLIFE"] = TChange(combatStats["CURRENTLIFE"], value, 0, combatStats["LIFE"]);
		}
		void gainEnergy(int value) {
			// armour, damage modifiers already calculated so apply directly
			combatStats["CURRENTENERGY"] = TChange(combatStats["CURRENTENERGY"], value, 0, combatStats["ENERGY"]);
		}
		float getLifeAsPercentage() {
			return float(combatStats["CURRENTLIFE"]) / float(combatStats["LIFE"]);
		}
		float getEnergyAsPercentage() {
			return float(combatStats["CURRENTENERGY"]) / float(combatStats["ENERGY"]);
		}

		bool isDead() {
			return combatStats["LIFE"] > 0 and combatStats["CURRENTLIFE"] == 0;
		}
		void setCombatStats(Combat& combat, bool reset) {
			// set at start of combat or whenever stats change during combat
			for (auto attribute : combat.AttributesInOrder.internalList) {
				combatStats[attribute] = getAttValue(attribute, *&combat, true);
			}
			for (auto stat : combat.statsInOrder.internalList) {
				combatStats[stat] = getPowerOfThis(defaultStats[stat], true, *&combat);
			}
			for (auto sName : data["equippedSkillNames"].getKeys().internalList) {
				combatSkills[stoi(sName)] = combat.skillDefinitions[data["equippedSkillNames"][sName]].makeSkillInstance();
			}
			if (!combatSkills.hasKey(0)) {
				combatSkills[0] = combat.skillDefinitions["DEFAULT_ATTACK"].makeSkillInstance();
			}
			combatSkills[6] = combat.skillDefinitions["DEFAULT_WAIT"].makeSkillInstance();

			if (reset) {
				combatStats["CURRENTLIFE"] = combatStats["LIFE"];
				combatStats["CURRENTENERGY"] = combatStats["ENERGY"];
			}

		}
		bool criticalRoll(int criticalBoost) {
			int diceRoll = RANDOM.getRandom(0, 100);
			int luck = combatStats["LUCK"] + criticalBoost;
			return diceRoll <= luck;
		}
		wstring getPrintout(string language, Combat& combat, bool includeOtherInfluences) {
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
		wstring getPointsRemainingPrintout(string language, Combat& combat) {
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
		wstring getSkillPrintOut(string language, string skillID, Combat& combat, bool includeOtherInfluences) {
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
		wstring getEquipmentPrintout(string language, Combat& combat) {
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

		int countSkillsByCategory(string category) {
			int result = 0;
			if (category == "ATTACK") { // physical attack skills
				for (Skill& skill : combatSkills.getValues().internalList) {
					if (skill.skillTypeTags.contains("ATTACK")) { // there may be other ways a skill is an "attack"
						result++;
					}
				}
			}
			if (category == "OFFENSIVE") { // any skill that targets a foe
				for (Skill& skill : combatSkills.getValues().internalList) {
					if (skill.skillTree == "Default") { continue; }
					if (skill.targetLogic == "SINGLEFOE" or skill.targetLogic == "FOECASTINGASPELL") { // there may be other ways a skill is an "attack"
						result++;
					}
				}
			}
			if (category == "SPELL") { // any spell
				for (Skill& skill : combatSkills.getValues().internalList) {
					if (skill.skillTypeTags.contains("MAGICAL")) {
						result++;
					}
				}
			}
			return result;
		}

		int getPowerOfThis(PowerValue P, bool includeOtherInfluences, Combat& combat) {
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
			// if this combatant is in a battle and suffering from WEAKNESS, reduce all atts before calculating power
			if (combat.currentBattle != NULL) {
				for (EffectObjectInstance* effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(uniqueCombatID).internalList) {
					if (effect->e.logicName == "Weaponsmithing") {
						percentInfluences["ArmsBOOST"] = TChange(percentInfluences["ArmsBOOST"], effect->e.values["power"], 0, 9999);
						percentInfluences["Minor ArmsBOOST"] = TChange(percentInfluences["Minor ArmsBOOST"], effect->e.values["power"], 0, 9999);
					}
					if (effect->e.logicName == "Charge Bolt2") {
						percentInfluences["ElectricBOOST"] = TChange(percentInfluences["ElectricBOOST"], effect->e.values["power"], 0, 9999);
					}
					if (effect->e.logicName == "WEAKENED") {
						for (auto v : percentInfluences.getKeys().internalList) {
							if (v.find("_ATT") != -1) {
								percentInfluences[v] = TChange(percentInfluences[v], -2, 0, 9999);
							}
						}
					}
				}
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
						}
					}
				}
			}
			return result;
		}
		int getSkillCost(Skill which, bool includeOtherInfluences, Combat& combat) {
			int result = which.baseCost;
			if (which.powerValues.hasKey("COST")) {
				result = getPowerOfThis(which.powerValues["COST"], includeOtherInfluences, *&combat);
			}
			if (combat.currentBattle != NULL) {
				for (EffectObjectInstance* effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(uniqueCombatID).internalList) {
					if (effect->e.logicName == "OVERWHELMED" and which.skillTypeTags.contains("MAGICAL")) {
						result += 20;
					}
					if (effect->e.logicName == "Gift of Knowledge") { 
						List<string> valid; valid.internalList = { "Cleromancy", "Hagiomancy" };
						if (valid.contains(which.skillTree)) {
							float power = (100.0 - effect->e.values["power"]) / 100;
							result *= power;
						}
					}
				}
			}
			return result;
		}
		int getSkillActivationTime(Skill which, bool includeOtherInfluences, Combat& combat) {
			int result = which.baseCastingTime;
			if (which.powerValues.hasKey("ACTIVATION")) {
				result = getPowerOfThis(which.powerValues["ACTIVATION"], includeOtherInfluences, *&combat);
			}
			if (combat.currentBattle != NULL) {
				for (EffectObjectInstance* effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(uniqueCombatID).internalList) { // effect on user
					if (which.skillTypeTags.contains("PHYSICAL") and which.uniqueID != "DEFAULT_WAIT" and effect->e.logicName == "CRIPPLED") {
						result++;
					}
					if (which.skillTypeTags.contains("MAGICAL") and which.uniqueID != "DEFAULT_WAIT" and effect->e.logicName == "FROZEN") {
						result++;
					}
					if (which.skillTypeTags.contains("MAGICAL") and which.uniqueID != "DEFAULT_WAIT" and effect->e.logicName == "Time Walk") {
						result = 0;
					}
				}
				for (EffectObjectInstance* effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(currentTarget).internalList) { // effect on victim
					if (which.skillTypeTags.contains("ATTACK") and effect->e.logicName == "Proscribe") {
						result = TChange(result, -1, 0, 99);
					}
				}
			}
			return result;
		}
		int getSkillRecharge(Skill which, bool includeOtherInfluences, Combat& combat) {
			int result = which.baseRecharge;
			if (which.powerValues.hasKey("RECHARGE")) {
				result = getPowerOfThis(which.powerValues["RECHARGE"], includeOtherInfluences, *&combat);
			}
			if (combat.currentBattle != NULL) {
				for (EffectObjectInstance* effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(uniqueCombatID).internalList) { // effect on user
					if (which.skillTypeTags.contains("MAGICAL") and effect->e.logicName == "Mind Maze") {
						result += effect->e.values["power"];
					}
					if (which.skillTypeTags.contains("ATTACK") and effect->e.logicName == "Exemplar's Posture") {
						if (result > 0) {
							result--;
						}
					}
				}
				for (EffectObjectInstance * effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(currentTarget).internalList) { // effect on victim
					if (which.skillTypeTags.contains("ATTACK") and effect->e.logicName == "Proscribe") {
						result = TChange(result, -1, 0, 99);
					}
				}
			}
			return result;
		}
		List<string> getSkillsThatCanBeUsedbyName(Combat& combat) {
			List<string> result;
			for (auto x : combatSkills.getKeys().internalList) {
				if (combatSkills[x].canThisSkillBeUsed(*&combat)) {
					result.push_back(combatSkills[x].uniqueID);
				}
			}
			return result;
		}
		bool currentlyCastingASpell() {
			return currentlyCasting and getSkillBeingCast().skillTypeTags.contains("MAGICAL");
		}

		void startCasting(Combat& combat, string target, int skillIndex) {
			indexOfSkillCurrentlyBeingCast = skillIndex;
			currentTarget = target;
			int castingTime = getSkillActivationTime(combatSkills[skillIndex], true, *&combat);
			int castingCost = getSkillCost(combatSkills[skillIndex], true, *&combat);
			payEnergyCost(castingCost);
			combatSkills[skillIndex].timeToCast = castingTime;
			currentlyCasting = true;
		}
		void finishCasting(Combat& combat) {
			int rechargeTime = getSkillRecharge(combatSkills[indexOfSkillCurrentlyBeingCast], true, *&combat);
			if (combatSkills[indexOfSkillCurrentlyBeingCast].timeToRecharge > rechargeTime) {
				// if, upon being used successfully, it becomes disabled for longer than it would normally recharge for
				rechargeTime = combatSkills[indexOfSkillCurrentlyBeingCast].timeToRecharge;
			}
			for (auto report : combat.currentBattle->currentEventStackObject.ongoingReport.internalList) {
				if (report.vData.hasKey("DELAYTHISSKILLRECHARGE")) {
					rechargeTime += report.vData["DELAYTHISSKILLRECHARGE"];
				}
			}
			currentlyCasting = false;
			combatSkills[indexOfSkillCurrentlyBeingCast].timeToRecharge = rechargeTime + 1;
			indexOfSkillCurrentlyBeingCast = -1;
		}
		void tickDownIfCasting() {
			if (currentlyCasting) {
				combatSkills[indexOfSkillCurrentlyBeingCast].timeToCast = TChange(combatSkills[indexOfSkillCurrentlyBeingCast].timeToCast, -1, 0, 999);
			}
		}
		void tickDownRechargingSkills() {
			for (auto skill : combatSkills.getKeys().internalList) {
				if (combatSkills[skill].timeToRecharge > 0) {
					combatSkills[skill].timeToRecharge--; // I doubt it, but anything which perma disables would happen here
				}
			}
		}
		void passiveManaRegen() {
			combatStats["CURRENTENERGY"] = TChange(combatStats["CURRENTENERGY"], combatStats["ENERGYREGEN"], 0, combatStats["ENERGY"]);
		}
		bool finishedCasting() {
			if (indexOfSkillCurrentlyBeingCast == -1) {
				return false;
			}
			return combatSkills[indexOfSkillCurrentlyBeingCast].timeToCast == 0;
		}
		Skill& getSkillBeingCast() {
			if (indexOfSkillCurrentlyBeingCast == -1) {
				throw exception("This actor is not using a skill, but getSkillBeingCast was called.");
			}
			return *&combatSkills[indexOfSkillCurrentlyBeingCast];
		}
		Skill& getSkillByName(string skillName) {
			return *&combatSkills[getIndexOfThisSkill(skillName)];
		}
		int getIndexOfThisSkill(string skillName) {
			for (auto s : combatSkills.getKeys().internalList) {
				if (combatSkills[s].uniqueID == skillName) {
					return s;
				}
			}
			throw exception("This actor has no skill with that name equipped.");
		}
		void cancelCast() {
			// if, upon activation, there are no valid targets (i.e. target died and there are no other targets left)
			currentlyCasting = false;
			indexOfSkillCurrentlyBeingCast = -1;
			currentTarget = "";
		}

		bool currentlyCasting = false;
		int indexOfSkillCurrentlyBeingCast = -1;
		string currentTarget = "";

		string uniqueID;
		string uniqueCombatID; // if multiple copies of the same combatant are in a battle
		string slot; // 1,2,3,4
		string displayName;
		Map<string, Map<string, string>> data;
		Map<string, int> intData; // things which influence stats go here
		Map<string, PowerValue> defaultStats;
		Map<string, int> combatStats; // stats that change during combat
		Map<int, Skill> combatSkills; // skills in Combat
	};
	class CombatantInstance {
	public:
		class CombatantAI {
		public:
			CombatantAI() {}
			List<string> getCurrentStatuses(Combat& combat, CombatantInstance* me) {
				List<string> result;
				result.addToBackIfNotAlreadyInList("DEALDAMAGE");
				result.addToBackIfNotAlreadyInList("CURSEFOE");
				result.addToBackIfNotAlreadyInList("STEALLIFEFORMASTER");
				if (me->getAllPossiblePositionsICouldPutASummon(*&combat).size() > 0) {
					result.addToBackIfNotAlreadyInList("SUMMON");
				}
				if (me->c.getLifeAsPercentage() < 0.85 and !me->c.isDead()) {
					result.addToBackIfNotAlreadyInList("HEALSELF");
				}

				for (CombatantInstance* actor : me->getMyFoes(*&combat).internalList) {
					if (actor->c.currentlyCasting) {
						result.addToBackIfNotAlreadyInList("INTERRUPTFOE");
					}
					if (!actor->c.isDead() and actor->c.getEnergyAsPercentage() < 10) {
						result.addToBackIfNotAlreadyInList("MANABURN");
					}
					for (EffectObjectInstance* effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(actor->c.uniqueCombatID).internalList) {
						if (effect->e.uniqueID == "UNDEAD") {
							result.addToBackIfNotAlreadyInList("DEALDAMAGE_HOLY");
							result.addToBackIfNotAlreadyInList("KILLUNDEAD");
						}
						if (effect->e.uniqueID == "DEMONIC") {
							result.addToBackIfNotAlreadyInList("KILLDEMONS");
						}
						if (effect->e.type == "BOON") {
							result.addToBackIfNotAlreadyInList("REMOVEFOE'SBOON");
						}
					}
				}
				List<string> buffsICouldUseOnAllies;
				for (Skill S : me->c.combatSkills.getValues().internalList) {
					if (S.isThisABuffICanUseOnSomeone(*&combat)) {
						buffsICouldUseOnAllies.push_back(S.uniqueID);
					}
				}
				List<string> buffsICouldUseOnMyselfOnly;
				for (Skill S : me->c.combatSkills.getValues().internalList) {
					if (S.isThisABuffICanUseOnMyselfOnly(*&combat)) {
						buffsICouldUseOnMyselfOnly.push_back(S.uniqueID);
					}
				}
				bool ICouldBuff = false;
				for (CombatantInstance* actor : me->getAllMyOtherAlliesNotMe(*&combat).internalList) {
					if (actor->c.getLifeAsPercentage() < 0.9 and !actor->c.isDead()) {
						result.addToBackIfNotAlreadyInList("HEALALLY");
					}
					if (actor->c.isDead()) {
						result.addToBackIfNotAlreadyInList("RESURRECT");
					}
					List<string> effectsOnThisActor;
					for (EffectObjectInstance* effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(actor->c.uniqueCombatID).internalList) {
						effectsOnThisActor.push_back(effect->e.uniqueID);
					}
					for (auto s : buffsICouldUseOnAllies.internalList) {
						if (!effectsOnThisActor.contains(s)) {
							ICouldBuff = true;
						}
					}
				}
				if (ICouldBuff) {
					result.push_back("ENCHANTALLY");
				}
				List<string> effectsOnMeByName = combat.currentBattle->getNamesOfEffectsOnX(me->c.uniqueCombatID);
				for (auto skill : buffsICouldUseOnMyselfOnly.internalList) {
					if (!effectsOnMeByName.contains(skill)) {
						result.addToBackIfNotAlreadyInList("ENCHANTSELF");
					}
				}
				for (CombatantInstance* actor : me->getMyAlliesThatAreAlive(*&combat).internalList) {
					if (!combat.currentBattle->getAllEffectsOnXThatAreThisType(actor->c.uniqueCombatID, "BANE").empty()) {
						result.addToBackIfNotAlreadyInList("CUREALLY");
					}
				}
				return result;
			}
			string startCasting(Combat& combat, CombatantInstance* me) {
				// returns the instruction the game engine uses to use the same casting code as a human player
				string skill = makeSkillDecision(*&combat, me);
				string target = makeTargetDecision(*&combat, me, skill);
				int skillSlot = me->c.getIndexOfThisSkill(skill);
				me->c.indexOfSkillCurrentlyBeingCast = skillSlot;
				me->c.currentTarget = target;
				return "STARTUSINGSKILLON_$" + target + "$" + to_string(skillSlot);
			}
			string makeTargetDecision(Combat& combat, CombatantInstance* me, string skillDecision) {
				int skillIndex = me->c.getIndexOfThisSkill(skillDecision);
				Skill& theSkill = me->c.combatSkills[skillIndex];
				List<CombatantInstance*> possibleTargets = combat.currentBattle->getAllValidTargetsForThisSkill(*&combat, me, theSkill);
				if (possibleTargets.empty()) {
					throw exception("The AI chose to use a skill which has no valid targets.");
				}
				if (possibleTargets.size() == 1) {
					return possibleTargets.front()->c.uniqueCombatID;
				}
				Map<string, int> targetProbability;
				for (CombatantInstance* actor : possibleTargets.internalList) {
					List<EffectObjectInstance*> buffsOnThisActor = combat.currentBattle->getAllEffectsOnXThatAreThisType(actor->c.uniqueCombatID, "BOON");
					List<EffectObjectInstance*> cursesOnThisActor = combat.currentBattle->getAllEffectsOnXThatAreThisType(actor->c.uniqueCombatID, "BANE");
					targetProbability[actor->c.uniqueCombatID] = 10;
					for (string strategy : theSkill.purposes.internalList) {
						if (strategy.find("HEAL") != -1) {
							float lifeAsPercentage = actor->c.getLifeAsPercentage();
							if (lifeAsPercentage > 0.99 or actor->c.isDead()) {
								targetProbability[actor->c.uniqueCombatID] = 1;
							}
							targetProbability[actor->c.uniqueCombatID] += (1000 - (lifeAsPercentage * 1000));
						}
						if (strategy.find("CURE") != -1 and cursesOnThisActor.empty()) {
							targetProbability[actor->c.uniqueCombatID] = 0;
						}
						if (strategy.find("ENCHANT") != -1) {
							for (EffectObjectInstance* buff : buffsOnThisActor.internalList) {
								if (buff->e.uniqueID == skillDecision and buff->e.duration > 1 and not buff->e.infinite) {
									targetProbability[actor->c.uniqueCombatID] = 0;
								}
							}
						}
						if (strategy.find("CURSE") != -1) {
							for (EffectObjectInstance* buff : cursesOnThisActor.internalList) {
								if (buff->e.uniqueID == skillDecision and buff->e.roundsLeft > 1 and not buff->e.infinite) {
									targetProbability[actor->c.uniqueCombatID] = 0;
								}
							}
						}
						if (strategy.find("INTERRUPT") != -1) {
							if (!actor->c.currentlyCasting) {
								targetProbability[actor->c.uniqueCombatID] = 0;
							}
						}
					}
				}
				if (highestValueInMap(targetProbability) == 0) {
					// failed to make an informed decision (for example, actor can only curse and every target has that curse
					// so just target anyone
					return RANDOM.getRandom(possibleTargets)->c.uniqueCombatID;

				}
				string decision = RANDOM.diceRollChoice(targetProbability);
				return decision;
			}
			string makeSkillDecision(Combat& combat, CombatantInstance* me) {
				List<string> statuses = getCurrentStatuses(*&combat, me);
				List<string> usableSkills = me->c.getSkillsThatCanBeUsedbyName(*&combat);
				Map<string, int> chanceOfUsingSkill;
				for (auto s : usableSkills.internalList) {
					chanceOfUsingSkill[s] = 2;
				}

				if (usableSkills.size() == 0) {
					throw exception("This actor has no skills.");
				}
				List<string> allStrategiesICouldFollow;
				for (auto s : usableSkills.internalList) {
					for (auto p : combat.skillDefinitions[s].purposes.internalList) {
						if (statuses.contains(p)) {
							allStrategiesICouldFollow.addToBackIfNotAlreadyInList(p);
						}
					}
				}
				Map<string, int> strategyToPriorityMap = List<pair<string, int>>({
					pair<string, int>("RESURRECT",100),
					pair<string, int>("HEALSELF",100),
					pair<string, int>("HEALALLY",90),
					pair<string, int>("CUREALLY",90),
					pair<string, int>("CURESELF",90),
					pair<string, int>("CURSEFOE",90),
					pair<string, int>("ENCHANTSELF", 90),
					pair<string, int>("ENCHANTALLY", 90),
					pair<string, int>("SUMMON",250),
					pair<string, int>("DEALDAMAGE",20),
					pair<string, int>("STEALLIFEFORMASTER",100),
					pair<string, int>("WORLDEFFECT",250),
					pair<string, int>("INTERRUPTFOE",100),
					pair<string, int>("MANADAMAGE",100),
					});
				for (auto strat : allStrategiesICouldFollow.internalList) {
					if (strategyToPriorityMap.getKeys().contains(strat)) {
						for (auto skillName : usableSkills.internalList) {
							List<string> skillPurposes = combat.skillDefinitions[skillName].purposes;
							if (!skillPurposes.doTheseListsHaveAnythingInCommon(statuses)) {
								chanceOfUsingSkill[skillName] = 0;
							}
							for (auto p : skillPurposes.internalList) {
								if (statuses.contains(p)) {
									chanceOfUsingSkill[skillName] += strategyToPriorityMap[p];
								}
								// don't use this curse if every foe already has it
								if (p == "CURSEFOE") {
									bool remainingTargets = false;
									for (CombatantInstance* potentialTarget : me->getMyFoesThatAreAlive(*&combat).internalList) {
										if (!combat.currentBattle->doesTargetXHaveStatusY(potentialTarget->c.uniqueCombatID, skillName)) {
											remainingTargets = true;
										}
										else if (combat.currentBattle->getThisEffect(potentialTarget->c.uniqueCombatID, skillName)->e.roundsLeft < 2) {
											remainingTargets = true;
										}
									}
									if (!remainingTargets) {
										chanceOfUsingSkill[skillName] = 0;
									}
								}
								// don't use buff on self if already exists on self
								if (p == "ENCHANTSELF") {
									if (combat.currentBattle->getNamesOfEffectsOnX(me->c.uniqueCombatID).contains(skillName)) {
										chanceOfUsingSkill[skillName] = 0;
									}
								}
								// if a foe is casting increase chance of using a rupt
								if (p == "INTERRUPTFOE") {
									for (CombatantInstance* potentialTarget : me->getMyFoesThatAreAlive(*&combat).internalList) {
									if (potentialTarget->c.currentlyCasting) {
										chanceOfUsingSkill[skillName] += 100;
										}
									}
								}
							}
						}
					}
				}
				// if AI can't make any valid decisions force them to wait
				bool noOptions = true;
				for (auto skill : chanceOfUsingSkill.getValues().internalList) {
					if (skill > 0) {
						noOptions = false;
					}
				}
				if (chanceOfUsingSkill.hasKey("DEFAULT_ATTACK") and chanceOfUsingSkill.getKeys().size() > 1) {
					chanceOfUsingSkill["DEFAULT_ATTACK"] = 1;
				}
				string decision = "";
				if (noOptions) {
					decision = "DEFAULT_WAIT";
				}
				else {
					decision = RANDOM.diceRollChoice(chanceOfUsingSkill);
				}
				return decision;
			}
		};
		// make these as pointers
		CombatantInstance() {}
		CombatantInstance(Combatant _c, string _team) {
			c = _c;
			team = _team;
		}
		bool operator==(CombatantInstance* rhs) {
			return c.uniqueCombatID == rhs->c.uniqueCombatID;
		}

		List<CombatantInstance*> getMyTeam(Combat& combat) {
			if (team == "TEAM1" or team == "TEAM1_ALLIES") {
				return combat.currentBattle->party1;
			}
			return combat.currentBattle->party2;
		}
		List<CombatantInstance*> getAllMyAllies(Combat& combat) {
			if (team == "TEAM1" or team == "TEAM1_ALLIES") {
				return combat.currentBattle->party1 + combat.currentBattle->party1allies;
			}
			return combat.currentBattle->party2 + combat.currentBattle->party2allies;
		}
		List<CombatantInstance*> getMyAlliesThatAreAlive(Combat& combat) {
			List<CombatantInstance*> allAllies = getAllMyAllies(*&combat);
			List<CombatantInstance*> results;
			for (CombatantInstance* ally : allAllies.internalList) {
				if (!ally->c.isDead()) {
					results.push_back(ally);
				}
			}
			return results;
		}
		List<CombatantInstance*> getMyNonTeamAlliesOnly(Combat& combat) {
			if (team == "TEAM1" or team == "TEAM1_ALLIES") {
				return combat.currentBattle->party1allies;
			}
			return combat.currentBattle->party2allies;
		}
		List<CombatantInstance*> getMyFoes(Combat& combat) {
			if (team == "TEAM1" or team == "TEAM1_ALLIES") {
				return combat.currentBattle->party2 + combat.currentBattle->party2allies;
			}
			return combat.currentBattle->party1 + combat.currentBattle->party1allies;
		}
		List<CombatantInstance*> getMyFoesThatAreAlive(Combat& combat) {
			List<CombatantInstance*> allFoes = getMyFoes(*&combat);
			List<CombatantInstance*> result;
			for (CombatantInstance* actor : allFoes.internalList) {
				if (!actor->c.isDead()) {
					result.push_back(actor);
				}
			}
			return result;
		}
		List<CombatantInstance*> getAllMyOtherAlliesNotMe(Combat& combat) {
			List<CombatantInstance*> results;
			for (CombatantInstance* combatant : getAllMyAllies(*&combat).internalList) {
				if (combatant->c.uniqueCombatID != c.uniqueCombatID) {
					results.push_back(combatant);
				}
			}
			return results;
		}
		List<CombatantInstance*> getMyDeadPartyMembers(Combat& combat) {
			List<CombatantInstance*> result;
			for (CombatantInstance* actor : getMyTeam(*&combat).internalList) {
				if (actor->c.isDead()) {
					result.push_back(actor);
				}
			}
			return result;
		}
		List<CombatantInstance*> getMyDeadFoes(Combat& combat) {
			List<CombatantInstance*> allFoes = getMyFoes(*&combat);
			List<CombatantInstance*> result;
			for (CombatantInstance* actor : allFoes.internalList) {
				if (actor->c.isDead()) {
					result.push_back(actor);
				}
			}
			return result;
		}
		List<string> getAllPossiblePositionsICouldPutASummon(Combat& combat) {
			Map<string, Map<string, pair<float, float>>> lookup = combat.getCombatantPositionLookup();
			List<string> alreadyOccupied;
			List<string> empty;
			string validSlotTeam = "TEAM1_ALLIES";
			if (team == "TEAM2" or team == "TEAM2_ALLIES") {
				validSlotTeam = "TEAM2_ALLIES";
			}
			for (CombatantInstance* actor : combat.currentBattle->getAllCombatants().internalList) {
				if (actor == NULL) {
					return {};
				}
				if (actor->team == validSlotTeam) {
					alreadyOccupied.push_back(actor->c.slot);
				}
			}

			List<string> result;
			for (auto possible : lookup[validSlotTeam].getKeys().internalList) {
				if (!alreadyOccupied.contains(possible)) {
					result.push_back(possible);
				}
			}


			return result;
		}
		List<string> getTheirNames(List<CombatantInstance*> input) {
			List<string> results;
			for (CombatantInstance* actor : input.internalList) {
				results.push_back(actor->c.uniqueCombatID);
			}
			return results;
		}
		wstring getCanIUseThisSkillOrNotMessage(Combat& combat, string skillName) {
			wstring result = L"";
			Skill theSkill = c.getSkillByName(skillName);

			// not enough energy to use it
			if (!theSkill.canPayEnergyCost(*&combat)) {
				result += strings[combat.language]["Combat Messages"]["NOTENOUGHENERGY"];
			}

			// no one valid to use it on
			if (!theSkill.areThereAnyValidTargets(*&combat)) {
				result += L"\n\n" + strings[combat.language]["Combat Messages"]["NOVALIDTARGETS"];
			}

			// recharging
			if (theSkill.timeToRecharge > 0) {
				wstring toAdd = strings[combat.language]["Combat Messages"]["RECHARGING"];
				toAdd = WSReplace(toAdd, L"$", to_wstring(theSkill.timeToRecharge));
				result += L"\n\n" + toAdd;
			}

			// see if an effect prevents it from being used

			return result;
		}
		List<CombatantInstance*> getMyFoesThatWereSummoned(Combat& combat) {
			List<CombatantInstance*> all = getMyFoesThatAreAlive(*&combat);
			List<CombatantInstance*> result;
			for (CombatantInstance* actor : all.internalList) {
				if (actor->wasISummoned()) {
					result.push_back(actor);
				}
			}
			return result;
		}
		List<CombatantInstance*> getMyFoesThatAreCastingASpell(Combat& combat) {
			List<CombatantInstance*> results;
			for (CombatantInstance* actor : getMyFoesThatAreAlive(*&combat).internalList) {
				if (actor->c.currentlyCastingASpell()) {
					results.push_back(actor);
				}
			}
			return results;
		}
		bool wasISummoned() {
			return c.data["EXTRAS"]["SUMMONED"] == "1";
		}
		List<CombatantInstance*> getActorsISummoned(Combat& combat) {
			List<CombatantInstance*> results;
			for (CombatantInstance* actor : combat.currentBattle->getAllCombatants().internalList) {
				if (actor->c.data.hasKey("EXTRAS") and actor->c.data["EXTRAS"].hasKey("WHOSUMMONEDME") and actor->c.data["EXTRAS"]["WHOSUMMONEDME"] == c.uniqueCombatID) {
					results.push_back(actor);
				}
			}
			return results;
		}

		int getARandomCastableSkillByIndex(Combat& combat, string target, string category) {
			Map<int, Skill> possibleSkills = combat.currentBattle->getThisCombatant(target)->c.combatSkills;
			Map<int, Skill> viableSkills;
			for (auto [key, value] : possibleSkills.internalMap) {
				if (value.areThereAnyValidTargets(*&combat) and value.skillTypeTags.contains(category) and value.uniqueID != "DEFAULT_WAIT" and value.uniqueID != "DEFAULT_ATTACK") {
					viableSkills[key] = value;
				}
			}
			if (viableSkills.getKeys().empty()) {
				return -999; // none
			}
			int whichSkill = RANDOM.getRandom(viableSkills.getKeys());
			return whichSkill;
		}

		void changeTargetOrCancelCastingIfCurrentTargetIsNowInapplicable(Combat& combat, string language) {
			CombatantInstance* target = combat.currentBattle->getThisCombatant(c.currentTarget);
			List<CombatantInstance*> validTargets = combat.currentBattle->getAllValidTargetsForThisSkill(*&combat, this, c.getSkillBeingCast());
			if (validTargets.contains(target)) {
				return;
			}
			if (validTargets.empty()) {
				// stop casting
				combat.currentBattle->addCombatMessage("FAILED_NO_TARGETS", List<pair<string, string>>({
					pair<string, string>("language", language),
					pair<string, string>("name", c.uniqueID),
					pair<string, string>("skill", c.getSkillBeingCast().uniqueID),
					pair<string, string>("target", c.currentTarget),
					}), 0);
				c.cancelCast();
				return;
			}
			target = validTargets.at(0);
			c.currentTarget = target->c.uniqueCombatID;
		}

		List<CombatEvent> startSkillExecution(Combat& combat, string language) {
			List<CombatEvent> results;
			Map<string, string> skillSData;
			Map<string, int> skillVData;
			skillSData["success"] = "1";
			Skill toCast = c.combatSkills[c.indexOfSkillCurrentlyBeingCast];
			string target = combat.currentBattle->getThisCombatant(c.currentTarget)->c.uniqueCombatID;
			if (toCast.skillLogicNames.contains("COPYSPELL") or toCast.skillLogicNames.contains("COPYPHYSICAL")) {
				int castableIndex = -999;
				if (toCast.skillLogicNames.contains("COPYSPELL")) {
					castableIndex = getARandomCastableSkillByIndex(*&combat, target, "MAGICAL");
				}
				if (toCast.skillLogicNames.contains("COPYPHYSICAL")) {
					castableIndex = getARandomCastableSkillByIndex(*&combat, target, "PHYSICAL");
				}

				if (castableIndex == -999) {
					skillSData["success"] = "-1";
					skillSData["failReason"] = "NOTHINGTOCOPY";
				}
				else {
					toCast = combat.currentBattle->getThisCombatant(target)->c.combatSkills[castableIndex];
					c.combatSkills[-999] = toCast;
					string newTarget = CombatantAI().makeTargetDecision(*&combat, this, toCast.uniqueID);
					c.combatSkills.internalMap.erase(-999);
					target = newTarget;
				}
			}


			for (auto [key, value] : toCast.powerValues.internalMap) {
				skillVData[key] = c.getPowerOfThis(value, true, *&combat);
			}
			bool magical = toCast.skillTypeTags.contains("MAGICAL");
			string skillNameS = toCast.uniqueID;
			wstring skillName = strings[language]["Skill Names"][toCast.uniqueID];
			wstring userName = strings[language]["NPCNames"][c.uniqueID];
			wstring targetName = strings[language]["NPCNames"][combat.currentBattle->getThisCombatant(c.currentTarget)->c.uniqueID];
			skillSData["message"] = WStringToString(strings[language]["Skill Actions"]["PHYSICAL_DONE"]);
			int criticalBoost = 0;
			if (skillVData.hasKey("CRITICALBOOST")) {
				criticalBoost += skillVData["CRITICALBOOST"];
			}
			bool critical = c.criticalRoll(criticalBoost);
			float criticalModifier = 1.0;
			if (critical) {
				criticalModifier = 2.0;
				skillSData["message2"] = WStringToString(strings[language]["Skill Actions"]["CRITICAL_PHYSICAL"]);
				if (magical) {
					skillSData["message2"] = WStringToString(strings[language]["Skill Actions"]["CRITICAL_MAGICAL"]);
				}
			}
			for (auto v : skillVData.getKeys().internalList) {
				if (v != "CRITICALBOOST") {
					skillVData[v] *= criticalModifier;
				}
			}

			if (magical) {
				skillSData["message"] = WStringToString(strings[language]["Skill Actions"]["MAGICAL_DONE"]);
			}
			if (c.currentTarget == c.uniqueCombatID) { // using skill on self
				skillSData["message"] = SReplace(skillSData["message"], "$PLAYER$", WStringToString(userName));
				skillSData["message"] = SReplace(skillSData["message"], "$SKILL$", WStringToString(skillName));
				skillSData["message"] = SReplace(skillSData["message"], "$ON$", "");
				skillSData["message"] = SReplace(skillSData["message"], "$TARGET$", "");
			}
			else {
				skillSData["message"] = SReplace(skillSData["message"], "$PLAYER$", WStringToString(userName));
				skillSData["message"] = SReplace(skillSData["message"], "$SKILL$", WStringToString(skillName));
				skillSData["message"] = SReplace(skillSData["message"], "$TARGET$", WStringToString(targetName));
				skillSData["message"] = SReplace(skillSData["message"], "$ON$", WStringToString(strings[language]["Skill Actions"]["ON"]));
			}

			
			// decide if skill attempt should succeed at all
			List<EffectObjectInstance*> allEffects = combat.currentBattle->getAllEffectsInTimeOrderOldestFirst();
			List<CombatantInstance*> allOpponents = getMyFoesThatAreAlive(*&combat);
			List<CombatantInstance*> allAllies = getAllMyOtherAlliesNotMe(*&combat);

			// go through all effects
			List<pair<string,string>> effectsThatNeedToBeRemoved;
			for (EffectObjectInstance* effect : allEffects.internalList) {
				// effect is on any opponent
				for (CombatantInstance* actor : allOpponents.internalList) {
					if (effect->e.target == actor->c.uniqueCombatID) {
						if (effect->e.triggers.contains("ANYALLYATTACKEDPHYSICAL")
							and !magical 
							and !getKeysWhichContainX(c.combatSkills[c.indexOfSkillCurrentlyBeingCast].skillLogicNames, "DAMAGE").empty()) {
							if (effect->e.logicName == "Gentleman's Riposte") {
								skillSData["success"] = "0";
								skillSData["failReason"] = "ATTACKBLOCKED";
								string foeUsingRiposte = actor->c.uniqueCombatID;
								Map<string, int> riposteVData;
								Map<string, string> riposteSData;
								for (auto [key, value] : actor->c.combatSkills[0].powerValues.internalMap) {
									riposteVData[key] = actor->c.getPowerOfThis(value, true, *&combat);
								}
								riposteSData["success"] = "1";
								results.push_back(CombatEvent("DAMAGE_SINGLE_PHYSICAL", "SKILL", "DEFAULT_ATTACK", foeUsingRiposte, {c.uniqueCombatID}, riposteSData, riposteVData));
								effectsThatNeedToBeRemoved.push_back(pair<string, string>(foeUsingRiposte, effect->e.uniqueID));
							}
						}
					}
				}

				// effect is on the user
				if (effect->e.target == c.uniqueCombatID) {
					if (effect->e.logicName == "Shield of the Messenger" and c.getSkillBeingCast().skillTypeTags.contains("MAGICAL")) {
						effectsThatNeedToBeRemoved.push_back(pair<string, string>({ c.uniqueCombatID, "Shield of the Messenger" }));
					}
					if (effect->e.logicName == "Cestodarian Siphon" and c.getSkillBeingCast().skillTypeTags.contains("MAGICAL")) {
						string user = effect->e.owner;
						string victim = c.uniqueCombatID;
						Map<string, string> subSData; subSData["success"] = "1";
						subSData["audioSource"] = "8737";
						Map<string, int> subVData; subVData["LIFESTEAL_SINGLE_UNHOLY"] = effect->e.values["power"];
						results.push_back(CombatEvent("LIFESTEAL_SINGLE_UNHOLY", "SKILL", "Cestodarian Siphon", user, { victim }, subSData, subVData));
					}
					if (effect->e.logicName == "BLEEDING" and c.getSkillBeingCast().skillTypeTags.contains("MAGICAL")) {
						Map<string, string> bleedingSData;
						Map<string, int> bleedingVData;
						bleedingSData["success"] = "1";
						bleedingVData["DAMAGE_SINGLE_NEUTRAL"] = 40;
						string user = effect->e.owner;
						string victim = c.uniqueCombatID;
						results.push_back(CombatEvent("DAMAGE_SINGLE_NEUTRAL", "SKILL", "BLEEDING", user, { victim }, bleedingSData, bleedingVData));
					}
					if (effect->e.logicName == "POISONED" and c.getSkillBeingCast().skillTypeTags.contains("PHYSICAL") and c.getSkillBeingCast().uniqueID != "DEFAULT_WAIT") {
						Map<string, string> poisonedSData;
						Map<string, int> poisonedVData;
						poisonedSData["success"] = "1";
						poisonedVData["DAMAGE_SINGLE_NEUTRAL"] = 40;
						string user = effect->e.owner;
						string victim = c.uniqueCombatID;
						results.push_back(CombatEvent("DAMAGE_SINGLE_NEUTRAL", "SKILL", "POISONED", user, { victim }, poisonedSData, poisonedVData));
					}
					if (effect->e.logicName == "DISEASED" and c.getSkillBeingCast().uniqueID != "DEFAULT_WAIT") {
						Map<string, string> diseasedSData;
						Map<string, int> diseasedVData;
						diseasedSData["success"] = "1";
						diseasedVData["DAMAGE_SINGLE_NEUTRAL"] = 10;
						string user = effect->e.owner;
						string victim = c.uniqueCombatID;
						results.push_back(CombatEvent("DAMAGE_SINGLE_NEUTRAL", "SKILL", "DISEASED", user, { victim }, diseasedSData, diseasedVData));
						CombatantInstance* currentActor = combat.currentBattle->getThisCombatant(user);
						if (currentActor->c.currentTarget != "WORLD") {
							CombatantInstance* theTarget = combat.currentBattle->getThisCombatant(currentActor->c.currentTarget);
							if (allAllies.contains(theTarget)) {
								Map<string, string> spreadDiseaseSData;
								Map<string, int> spreadDiseaseVData;
								spreadDiseaseSData["success"] = "1";
								spreadDiseaseSData["sourceOfTheDisease"] = user;
								spreadDiseaseVData["DURATION_DISEASED"] = effect->e.roundsLeft;
								results.push_back(CombatEvent("APPLY_DISEASED_SINGLE", "SKILL", "DISEASED2", user, { theTarget->c.uniqueCombatID }, diseasedSData, diseasedVData));
								// this is DISEASED2 because in this case we want the game to animate the same skill animation twice in a row!
							}
						}
						
					}
					if (effect->e.logicName == "CONCUSSED" and c.getSkillBeingCast().skillTypeTags.contains("MAGICAL")) {
						int diceRoll = RANDOM.getRandom(1, 100);
						if (diceRoll < 75) {
							skillSData["success"] = "0";
							skillSData["failReason"] = "CONCUSSED";
						}
					}
					if (effect->e.logicName == "BLIND" and c.getSkillBeingCast().skillTypeTags.contains("PHYSICAL")) {
						int diceRoll = RANDOM.getRandom(1, 100);
						if (diceRoll < 50) {
							skillSData["success"] = "0";
							skillSData["failReason"] = "BLIND";
						}
					}
					if (effect->e.logicName == "Stalked by Shadows" and c.getSkillBeingCast().skillTypeTags.contains("MAGICAL")) {
						skillSData["success"] = "0";
						skillSData["failReason"] = "STALKEDBYSHADOWS";
					}
					if (effect->e.logicName == "Wildfire") {
						Map<string, int> damageWildfireVData;
						Map<string, string> damageWildfireSData;
						damageWildfireSData["success"] = "1";
						int damage = effect->e.values["power"];
						damageWildfireVData["DAMAGE_SINGLE_POWER"] = damage;
						string user = effect->e.owner;
						string victim = c.uniqueCombatID;
						results.push_back(CombatEvent("DAMAGE_SINGLE_POWER", "SKILL", "Wildfire", user, { victim }, damageWildfireSData, damageWildfireVData));
						List<CombatantInstance*> toTry = scrambleList(getAllMyOtherAlliesNotMe(*&combat));
						for (CombatantInstance* actor : toTry.internalList) {
							if (actor->c.isDead()) { continue; }
							if (combat.currentBattle->doesTargetXHaveStatusY(actor->c.uniqueCombatID, "Wildfire")) { continue; }
							Map<string, string> applyWildfireSData;
							Map<string, int> applyWildfireVData;
							applyWildfireSData["success"] = "1";
							applyWildfireVData["DURATION_Wildfire"] = effect->e.roundsLeft;
							applyWildfireVData["DAMAGE_FIRE_SINGLE"] = effect->e.values["power"];
							applyWildfireVData["POWER_Wildfire"] = effect->e.values["power"];
							results.push_back(CombatEvent("APPLY_Wildfire_SINGLE", "SKILL", "Wildfire", user, { actor->c.uniqueCombatID }, applyWildfireSData, applyWildfireVData));
						}
					}
					
				}
			}
			for (auto eff : effectsThatNeedToBeRemoved.internalList) {
				combat.currentBattle->removeAnEffect(eff.first, eff.second);
			}

			
			if (toCast.audioSource != -1) {
				skillSData["audioSource"] = to_string(toCast.audioSource);
			}

			for (auto skillLogicName : toCast.skillLogicNames.internalList) {
				List<string> combatantsAffected;
				Map<string, string> sData = skillSData;
				Map<string, int> vData = skillVData;
				if (skillLogicName.find("APPLY_") != -1 and skillLogicName.find("IF?") != -1) {
					List<string> conditionData = split(skillLogicName, "IF?");
					sData["ifCondition"] = split(conditionData.at(1), "_").at(0);
					skillLogicName = SReplace(skillLogicName, "IF?" + sData["ifCondition"], "");
				}
				if (skillLogicName.find("REMOVEBANE") != -1 or skillLogicName.find("REMOVEXBANES_") != -1) {
					if (combat.currentBattle->getAllEffectsOnXThatAreThisType(c.currentTarget, "BANE").empty()) {
						continue;
					}
				}
				if (skillLogicName.find("_TARGETPLUSSUMMONS_") != -1) {
					combatantsAffected.addToBackIfNotAlreadyInList(combat.currentBattle->getThisCombatant(c.currentTarget)->c.uniqueCombatID);
					for (CombatantInstance* actor : combat.currentBattle->getThisCombatant(c.currentTarget)->getActorsISummoned(*&combat).internalList) {
						combatantsAffected.addToBackIfNotAlreadyInList(actor->c.uniqueCombatID);
					}
					sData["message"] = SReplace(skillSData["message"], "$TARGET$",
						" " + WStringToString(strings[language]["Skill Actions"]["ON"]) + " " + WStringToString(targetName)
					);
					skillSData["message"] = sData["message"];
					for (auto victim : combatantsAffected.internalList) {
						results.push_back(CombatEvent(skillLogicName, "SKILL", toCast.uniqueID, c.uniqueCombatID, {victim}, sData, vData));
					}

				}
				if (skillLogicName.find("_SINGLE") != -1 or skillLogicName.find("_SELF") != -1) {
					if (skillLogicName.find("DAMAGE_SELF_") != -1) {
						// treat this separately so that a skill with damage packets for target and self don't hit twice on both
						string toReplace = SReplace(skillLogicName, "_SELF_", "_SINGLE_");
						combatantsAffected.push_back(c.uniqueCombatID);
						sData["success"] = "1";
						vData[toReplace] = vData[skillLogicName];
						vData.internalMap.erase(skillLogicName);
						results.push_back(CombatEvent(toReplace, "SKILL", toCast.uniqueID, c.uniqueCombatID, combatantsAffected, sData, vData));
						continue;
					}
					if (skillLogicName.find("DAMAGE_SINGLE_") != -1) {
						// don't process damage-self on the target
						string toRemove = SReplace(skillLogicName, "_SINGLE_", "_SELF_");
						if (vData.hasKey(toRemove)) {
							vData.internalMap.erase(SReplace(skillLogicName, "_SINGLE_", "_SELF_"));
						}
					}
					if (skillLogicName.find("_SELF") != -1 and c.currentTarget != c.uniqueCombatID) {
						combatantsAffected.push_back(c.uniqueCombatID);
					}
					combatantsAffected.addToBackIfNotAlreadyInList(combat.currentBattle->getThisCombatant(c.currentTarget)->c.uniqueCombatID);
					sData["message"] = SReplace(skillSData["message"], "$TARGET$",
						" " + WStringToString(strings[language]["Skill Actions"]["ON"]) + " " + WStringToString(targetName)
					);
					skillSData["message"] = sData["message"];
					results.push_back(CombatEvent(skillLogicName, "SKILL", toCast.uniqueID, c.uniqueCombatID, combatantsAffected, sData, vData));
					
					if (skillNameS == "Chain Lightning") {
						if (allOpponents.size() == 1) { continue; }
						List<CombatantInstance*> allValidTargets = combat.currentBattle->getAllValidTargetsForThisSkill(*&combat, this, toCast);
						allValidTargets.forcibleRemove(combat.currentBattle->getThisCombatant(c.currentTarget));
						CombatantInstance* newTarget = scrambleList(allValidTargets).front();
						Map<string, int> chainLightningVData = vData;
						chainLightningVData["DAMAGE_SINGLE_ELECTRIC"] /= 2;
						results.push_back(CombatEvent(skillLogicName, "SKILL", toCast.uniqueID, c.uniqueCombatID, { newTarget->c.uniqueCombatID }, sData, chainLightningVData));
					}

					if (skillLogicName.find("LIFEHEAL_") != -1) {
						for (auto person : combatantsAffected.internalList) {
							if (combat.currentBattle->doesTargetXHaveStatusY(person, "Beggar's Blessing")) {
								EffectObjectInstance * effect = combat.currentBattle->getThisEffect(person, "Beggar's Blessing");
								CombatantInstance* caster = combat.currentBattle->getThisCombatant(effect->e.owner);
								if (!caster->c.isDead()) {
									Map<string, string> subSData;
									subSData["success"] = "1";
									subSData["audioSource"] = to_string(BEGGARBLESSING_WAV);
									Map<string, int> subVData;
									for (auto key : vData.getKeys().internalList) {
										if (key.find("LIFEHEAL_") != -1) {
											subVData[SReplace(key, "LIFEHEAL_", "LIFESTEAL_")] = vData[key] * 0.5;
										}
									}
									results.push_back(CombatEvent("LIFESTEAL_SINGLE_UNHOLY", "SKILL", "Beggar's Blessing", caster->c.uniqueCombatID, combatantsAffected, subSData, subVData));
								}
							}
						}
					}
				}
				if (skillLogicName.find("_WORLD") != -1) {
					combatantsAffected.push_back("WORLD");
					sData["message"] = SReplace(WStringToString(strings[language]["Skill Actions"]["WORLD_DONE"]), "$PLAYER$", WStringToString(userName));
					sData["message"] = SReplace(sData["message"], "$SKILL$", WStringToString(skillName));
					skillSData["message"] = sData["message"];
					results.push_back(CombatEvent(skillLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, combatantsAffected, sData, vData));
				}
				if (skillLogicName.find("_ALLFOES") != -1) {
					for (CombatantInstance* actor : getMyFoesThatAreAlive(*&combat).internalList) {
						combatantsAffected.push_back(actor->c.uniqueCombatID);
					}
					if (skillLogicName.find("INTERRUPT_ALLFOES_") != -1) {
						skillLogicName = SReplace(skillLogicName, "INTERRUPT_ALLFOES_", "INTERRUPT_SINGLE_");
					}
					if (skillLogicName.find("DAMAGE_ALLFOES_") != -1 or skillLogicName.find("MANABURN_ALLFOES_") != -1) {
						string oldSkillLogicName = skillLogicName;
						skillLogicName = SReplace(skillLogicName, "_ALLFOES_", "_SINGLE_");
						Map<string, int> newVData;
						for (auto v : vData.getKeys().internalList) {
							if (v == oldSkillLogicName) {
								newVData[skillLogicName] = vData[v];
							}
							else {
								newVData[v] = vData[v];
							}
						}
						for (auto victim : combatantsAffected.internalList) {
							sData["message"] = SReplace(WStringToString(strings[language]["Skill Actions"]["WORLD_DONE"]), "$PLAYER$", WStringToString(userName));
							sData["message"] = SReplace(sData["message"], "$SKILL$", WStringToString(skillName));
							skillSData["message"] = sData["message"];
							results.push_back(CombatEvent(skillLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, {victim}, sData, newVData));
						}
						continue;
					}
					results.push_back(CombatEvent(skillLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, combatantsAffected, sData, vData));
					sData["message"] = SReplace(WStringToString(strings[language]["Skill Actions"]["WORLD_DONE"]), "$PLAYER$", WStringToString(userName));
					sData["message"] = SReplace(sData["message"], "$SKILL$", WStringToString(skillName));
					skillSData["message"] = sData["message"];
				}
				if (skillLogicName.find("_ALLOTHERALLIES") != -1) {
					for (CombatantInstance* actor : getAllMyOtherAlliesNotMe(*&combat).internalList) {
						if (actor->c.isDead()) { continue; }
						combatantsAffected.push_back(actor->c.uniqueCombatID);
					}
					results.push_back(CombatEvent(skillLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, combatantsAffected, sData, vData));
					sData["message"] = SReplace(WStringToString(strings[language]["Skill Actions"]["WORLD_DONE"]), "$PLAYER$", WStringToString(userName));
					sData["message"] = SReplace(sData["message"], "$SKILL$", WStringToString(skillName));
					skillSData["message"] = sData["message"];
				}
				if (skillLogicName.find("_ALLALLIES") != -1) {
					for (CombatantInstance* actor : getAllMyAllies(*&combat).internalList) {
						if (actor->c.isDead()) { continue; }
						combatantsAffected.push_back(actor->c.uniqueCombatID);
					}
					results.push_back(CombatEvent(skillLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, combatantsAffected, sData, vData));
					sData["message"] = SReplace(WStringToString(strings[language]["Skill Actions"]["WORLD_DONE"]), "$PLAYER$", WStringToString(userName));
					sData["message"] = SReplace(sData["message"], "$SKILL$", WStringToString(skillName));
					skillSData["message"] = sData["message"];
				}
				if (skillLogicName.find("_ALL") != -1 and skillLogicName.find("FOES") == -1 and skillLogicName.find("_ALLALLIES") == -1 and skillLogicName.find("_ALLOTHERALLIES") == -1) {
					for (CombatantInstance* actor : combat.currentBattle->getAllCombatants().internalList) {
						combatantsAffected.push_back(actor->c.uniqueCombatID);
					}
					results.push_back(CombatEvent(skillLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, combatantsAffected, sData, vData));
					sData["message"] = SReplace(WStringToString(strings[language]["Skill Actions"]["WORLD_DONE"]), "$PLAYER$", WStringToString(userName));
					sData["message"] = SReplace(sData["message"], "$SKILL$", WStringToString(skillName));
					skillSData["message"] = sData["message"];
				}
				if (skillLogicName.find("_AOE") != -1) {
					List<CombatantInstance*> targets = combat.currentBattle->getThisCombatant(c.currentTarget)->getMyTeam(*&combat);
					string newLogicName = SReplace(skillLogicName, "_AOE", "_SINGLE");
					for (CombatantInstance* target : targets.internalList) {
						results.push_back(CombatEvent(newLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, { target->c.uniqueCombatID }, sData, vData));
					}
				}
				if (skillLogicName.find("ALLUNDEADORDEMONICFOES") != -1) {
					List<CombatantInstance*> all = getMyFoesThatAreAlive(*&combat);
					List<CombatantInstance*> targets;
					for (CombatantInstance* actor : all.internalList) {
						if (combat.currentBattle->doesTargetXHaveStatusY(actor->c.uniqueCombatID, "UNDEAD") or combat.currentBattle->doesTargetXHaveStatusY(actor->c.uniqueCombatID, "DEMONIC")) {
							targets.push_back(actor);
						}
					}
					string newLogicName = SReplace(skillLogicName, "_ALLUNDEADORDEMONICFOES", "_SINGLE");
					for (CombatantInstance* target : targets.internalList) {
						results.push_back(CombatEvent(newLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, { target->c.uniqueCombatID }, sData, vData));
					}
				}
				// add other targeting systems
				if (skillLogicName == "STEALLIFEFORMASTER") {
					Map<string, int> unholyDmg; unholyDmg["DAMAGE_SINGLE_UNHOLY"] = vData["STEALLIFEFORMASTER"];
					Map<string, int> unholyHealing; unholyHealing["LIFEHEAL_SINGLE_UNHOLY"] = vData["STEALLIFEFORMASTER"];
					results.push_back(CombatEvent("DAMAGE_SINGLE_UNHOLY", "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, List<string>(combat.currentBattle->getThisCombatant(c.currentTarget)->c.uniqueCombatID), sData, unholyDmg));
					string nameOfSummoner = c.data["EXTRAS"]["WHOSUMMONEDME"];
					if (combat.currentBattle->doesThisCombatantExist(nameOfSummoner) and
						!combat.currentBattle->getThisCombatant(nameOfSummoner)->c.isDead()) {
						results.push_back(CombatEvent("LIFEHEAL_SINGLE_UNHOLY", "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, List<string>(nameOfSummoner), sData, unholyHealing));
					}
				}
			}
			if (toCast.uniqueID == "DEFAULT_WAIT") {
				combat.currentBattle->addCombatMessage("WAIT", List<pair<string, string>>({
					pair<string, string>("language", language),
					pair<string, string>("name", c.uniqueID),
					}), 0);
				results.push_back(CombatEvent("DEFAULT_WAIT", "SKILL", toCast.uniqueID, c.uniqueCombatID, { c.uniqueCombatID }, skillSData, {}));
			}
			else {
				combat.currentBattle->addCombatMessage("DIRECT", pair<string, string>("message", skillSData["message"]), 0);
			}
			if (combat.currentBattle->doesTargetXHaveStatusY(c.uniqueCombatID, "Papalcy")) {
				int papalcyPower = combat.currentBattle->getThisEffect(c.uniqueCombatID, "Papalcy")->e.values["power"];
				List<string> peopleHealedByPapalcy;
				List<CombatantInstance*> allies = getAllMyAllies(*&combat);
				List<string> namesOfAllies = getTheirNames(allies);
				for (auto report : results.internalList) {
					for (auto person : report.combatantsAffected.internalList) {
						if (namesOfAllies.contains(person)) {
							peopleHealedByPapalcy.addToBackIfNotAlreadyInList(person);
						}
					}
				}
				for (auto person : peopleHealedByPapalcy.internalList) {
					Map<string, string> subSData; subSData["success"] = "1";
					Map<string, int> subVData; subVData["LIFEHEAL_SINGLE_HOLY"] = papalcyPower;
					results.push_back(CombatEvent("LIFEHEAL_SINGLE_HOLY", "SKILL", "Papalcy", c.uniqueCombatID, {person}, subSData, subVData));
				}
			}

			// blocking an attack also blocks any other reports associated with the attack
			List<CombatantInstance*> peopleWhoBlockedTheAttack;
			for (auto& report : results.internalList) {
				if (report.logic == "DAMAGE_SINGLE_PHYSICAL" and !magical) {
					for (auto victim : report.combatantsAffected.internalList) {
						if (combat.currentBattle->doesTargetXHaveStatusY(victim, "Shield of the Messenger")) {
							int dieRoll = RANDOM.getRandom(1, 100);
							bool blocked = dieRoll < 50;
							if (blocked) {
								peopleWhoBlockedTheAttack.addToBackIfNotAlreadyInList(combat.currentBattle->getThisCombatant(victim));
							}
						}
						if (combat.currentBattle->doesTargetXHaveStatusY(victim, "Fencer's Flash")) {
							int dieRoll = RANDOM.getRandom(1, 100);
							bool blocked = dieRoll <= 33;
							int numberOfAttackSkills = 0;
							for (auto s : c.combatSkills.getValues().internalList) {
								if (s.uniqueID == "DEFAULT_ATTACK") { continue; }
								if (s.skillTypeTags.contains("ATTACK")) {
									numberOfAttackSkills++;
								}
							}
							if (numberOfAttackSkills < 1) {
								blocked = false;
							}
							if (blocked) {
								peopleWhoBlockedTheAttack.addToBackIfNotAlreadyInList(combat.currentBattle->getThisCombatant(victim));
							}
						}
						if (combat.currentBattle->doesTargetXHaveStatusY(victim, "No One Said You Could Touch!")) {
							int dieRoll = RANDOM.getRandom(1, 100);
							bool blocked = dieRoll < 15;
							if (blocked) {
								peopleWhoBlockedTheAttack.addToBackIfNotAlreadyInList(combat.currentBattle->getThisCombatant(victim));
							}
						}
					}
				}
			}

			for (CombatantInstance* blocker : peopleWhoBlockedTheAttack.internalList) {
				for (auto& report : results.internalList) {
					if (report.combatantsAffected.front() == blocker->c.uniqueCombatID and report.sourceName == skillNameS) {
						report.sData["success"] = "0";
						skillSData["failReason"] = "ATTACKBLOCKED";
					}
				}
			}


			// check for effects that only trigger on the unit attacked but may be successful against others
			for (auto & report : results.internalList) {
				if (report.logic == "DAMAGE_SINGLE_PHYSICAL" and !magical) {
					for (auto victim : report.combatantsAffected.internalList) {
						if (combat.currentBattle->doesTargetXHaveStatusY(victim, "Trickblade")) {
							Map<string, int> riposteVData;
							Map<string, string> riposteSData;
							report.sData["success"] = "0";
							skillSData["failReason"] = "ATTACKBLOCKED";
							string foeUsingRiposte = victim;
							CombatantInstance* actor = combat.currentBattle->getThisCombatant(victim);
							for (auto [key, value] : actor->c.combatSkills[0].powerValues.internalMap) {
								riposteVData[key] = actor->c.getPowerOfThis(value, true, *&combat);
							}
							riposteSData["success"] = "1";
							results.push_back(CombatEvent("DAMAGE_SINGLE_PHYSICAL", "SKILL", "DEFAULT_ATTACK", foeUsingRiposte, { c.uniqueCombatID }, riposteSData, riposteVData));
							combat.currentBattle->removeAnEffect(victim, "Trickblade");
						}
						if (combat.currentBattle->doesTargetXHaveStatusY(c.uniqueCombatID, "Bailiff's Blade")) {
							EffectObjectInstance* e = combat.currentBattle->getThisEffect(c.uniqueCombatID, "Bailiff's Blade");
							int power = e->e.values["power"];
							Map<string, int> subVData; subVData["DURATION_CRIPPLED"] = power;
							Map<string, string> subSData = skillSData;
							subSData["audioSource"] = "12747";
							results.push_back(CombatEvent("APPLY_CRIPPLED_SINGLE", "SKILL", "Bailiff's Blade", c.uniqueCombatID, { victim }, subSData, subVData));
						}
					}
				}
			}
			if (combat.currentBattle->doesTargetXHaveStatusY(c.uniqueCombatID, "Knight Vision")) {
				if (c.getSkillBeingCast().skillTypeTags.contains("PHYSICAL")) {
					for (auto& r : results.internalList) {
						r.sData["success"] = "1";
					}
					
				}
			}


			return results;
		}

		string AIMakeDecision(Combat& combat) {
			CombatantAI logic;
			return logic.startCasting(*&combat, this);
		}

		Combatant c;
		string team;
	};
	class Battle {
	public:
		class Round {
		public:
			Round() {}
			Round(int _roundNumber, List<string> _actorOrder) {
				roundNumber = _roundNumber;
				actorOrder.internalList = { "WORLD" };
				actorOrder += _actorOrder;
			}
			int tick() {
				currentAct++;
				return currentAct;
			}
			string whoseTurnIsIt() {
				return actorOrder.at(currentAct);
			}
			string whoseTurnIsNext() {
				if (currentAct + 1 == actorOrder.size()) {
					return "WORLD";
				}
				return actorOrder.at(currentAct + 1);
			}
			bool isRoundOver() {
				return currentAct == actorOrder.size();
			}

			int roundNumber;
			int currentAct = 0;
			List<string> actorOrder;
		};
		class EventStackObject {
		public:
			class Result {
			public:
				Result() {}
				Result(string _uniqueID, string _message, string _colour, pair<float, float> _startingPosition) {
					uniqueID = _uniqueID;
					message = _message;
					colour = _colour;
					startingPosition = _startingPosition;
				}

				string message;
				string uniqueID;
				string colour;
				pair<float, float> startingPosition;
			};
			EventStackObject() {};
			EventStackObject(List<CombatEvent> _ongoingReport) {
				ongoingReport = _ongoingReport;
			}
			void executeTheStack(Combat& combat) {
				// iterate through all effects in order, decide what would be triggered and change the end result

				// passive effects which damage all foes should be converted into individual damage reports so they can be resisted individually
				List<CombatEvent> reportsBasedOnPassiveEffect;
				for (auto& report : ongoingReport.internalList) {
					if (report.logic.find("DAMAGE_ALLFOES_") != -1 or report.logic.find("MANABURN_ALLFOES_") != -1) {
						string logicName = SReplace(report.logic, "_ALLFOES_", "_SINGLE_");
						for (CombatantInstance* C : combat.currentBattle->getThisCombatant(report.originalUser)->getMyFoesThatAreAlive(*&combat).internalList) {
							Map<string, int> newVData;
							for (auto v : report.vData.getKeys().internalList) {
								if (v == report.logic) {
									newVData[logicName] = report.vData[v];
								}
								else {
									newVData[v] = report.vData[v];
								}
							}
							reportsBasedOnPassiveEffect.push_back(CombatEvent(logicName, "SKILL", report.sourceName, report.originalUser, List<string>({C->c.uniqueCombatID}), report.sData, newVData));
						}
					}
				}
				for (auto report : reportsBasedOnPassiveEffect.internalList) {
					ongoingReport.push_back(report);
				}
				
				// check for effects which would add new things to the stack before processing them
				List<CombatEvent> anyNewReportsToAddToStackAtStart;

				for (auto& report : ongoingReport.internalList) {
					CombatantInstance* user = combat.currentBattle->getThisCombatant(report.originalUser);
					for (auto key : report.vData.getKeys().internalList) {
						if (combat.currentBattle->doesTargetXHaveStatusY(report.originalUser, "Stalked by Vengeance")) {
							EffectObjectInstance* effect = combat.currentBattle->getThisEffect(report.originalUser, "Stalked by Vengeance");
							if (combat.skillDefinitions.hasKey(report.sourceName) and combat.skillDefinitions[report.sourceName].skillTypeTags.contains("MAGICAL")) {
								if (report.logic.find("DAMAGE_") != -1 and key.find("DAMAGE") != -1 and key.find("_HOLY_") == -1) {
										Map<string, string> subSData;
										Map<string, int> subVData;
										int powerAsPercentage = effect->e.values["power"];
										int damageBeingDealt = report.vData[key];
										float resultPower = (user->c.combatStats["LIFE"] * powerAsPercentage) / 100;
										subVData["DAMAGE_SINGLE_HOLY"] = resultPower;
										subSData["success"] = "1";
										subSData["audioSource"] = to_string(STALKEDBYVENGEANCE_WAV);
										anyNewReportsToAddToStackAtStart.push_back(CombatEvent("DAMAGE_SINGLE_HOLY", "SKILL", "Stalked by Vengeance", effect->e.owner, { user->c.uniqueCombatID }, subSData, subVData));
								}
							}
						}
					}
					if (combat.currentBattle->doesTargetXHaveStatusY(report.originalUser, "Castigate Cruor")) {
						EffectObjectInstance* effect = combat.currentBattle->getThisEffect(report.originalUser, "Castigate Cruor");
						if (report.logic.find("APPLY_Life Drain_SINGLE") != -1 or report.logic.find("LIFESTEAL_SINGLE") != -1 or report.logic.find("LIFESACRIFICE_SELF") != -1) {
							Map<string, string> subSData;
							Map<string, int> subVData;
							if (report.sourceName == "Life Drain" and report.vData.getKeys().empty()) { // the data is stored in the effect object and not the report (yet)
								EffectObjectInstance* lifeDrainEffect = combat.currentBattle->getThisEffect(report.combatantsAffected.front(), "Life Drain");
								subVData["DAMAGE_SINGLE_HOLY"] = lifeDrainEffect->e.values["power"];
								subSData["success"] = "1";
								subSData["audioSource"] = to_string(CASTIGATECRUOR_WAV);
								anyNewReportsToAddToStackAtStart.push_back(CombatEvent("DAMAGE_SINGLE_HOLY", "SKILL", "Castigate Cruor", effect->e.owner, { user->c.uniqueCombatID }, subSData, subVData));
							}
							for (auto v : report.vData.getKeys().internalList) {
								if (v == "LIFESTEAL_SINGLE_UNHOLY") {
									subVData["DAMAGE_SINGLE_HOLY"] = report.vData[v];
									subSData["success"] = "1";
									subSData["audioSource"] = to_string(CASTIGATECRUOR_WAV);
									anyNewReportsToAddToStackAtStart.push_back(CombatEvent("DAMAGE_SINGLE_HOLY", "SKILL", "Castigate Cruor", effect->e.owner, {user->c.uniqueCombatID}, subSData, subVData));
								}
								if (v == "LIFESACRIFICE_SELF_UNHOLY") {
									int powerAsPercentage = report.vData["LIFESACRIFICE_SELF_UNHOLY"];
									float resultPower = (user->c.combatStats["LIFE"] * powerAsPercentage) / 100;
									subVData["DAMAGE_SINGLE_HOLY"] = resultPower;
									subSData["success"] = "1";
									subSData["audioSource"] = to_string(CASTIGATECRUOR_WAV);
									anyNewReportsToAddToStackAtStart.push_back(CombatEvent("DAMAGE_SINGLE_HOLY", "SKILL", "Castigate Cruor", effect->e.owner, { user->c.uniqueCombatID }, subSData, subVData));
								}
							}
						}
					}
					if (report.sourceName == "Absolution") {
						List<EffectObjectInstance*> effects = combat.currentBattle->getAllEffectsOnXThatAreThisType(report.combatantsAffected.front(), "BANE");
						int howManyBanes = effects.size();
						if (howManyBanes > report.vData["power"]) { howManyBanes = report.vData["power"]; }
						List<CombatantInstance*> whoItCouldHit = combat.currentBattle->getThisCombatant(report.originalUser)->getMyFoesThatAreAlive(*&combat);
						List<string> whoItCouldHitNames;
						for (CombatantInstance* actor : whoItCouldHit.internalList) {
							whoItCouldHitNames.push_back(actor->c.uniqueCombatID);
						}
						for (int x = 0; x < howManyBanes; x++) {
							Map<string, string> subSData;
							Map<string, int> subVData;
							subVData["DAMAGE_SINGLE_HOLY"] = report.vData["POWER_Absolution"];
							subSData["success"] = "1";
							subSData["audioSource"] = to_string(ABSOLUTION_WAV);
							for (auto who : whoItCouldHitNames.internalList) {
								anyNewReportsToAddToStackAtStart.push_back(CombatEvent("DAMAGE_SINGLE_HOLY", "SKILL", "Absolution", report.originalUser, {who}, subSData, subVData));
							}
						}
					}
				}
				for (auto report : anyNewReportsToAddToStackAtStart.internalList) {
					ongoingReport.push_back(report);
				}

				// main stack
				for (auto& report : ongoingReport.internalList) {
					List<EffectObjectInstance*> allEffects = combat.currentBattle->getAllEffectsInTimeOrderOldestFirst();
					allEffects.internalList.reverse();
					CombatantInstance* user = combat.currentBattle->getThisCombatant(report.originalUser);
					List<CombatantInstance* > targets;

					if (report.logic.find("DAMAGE_ALLFOES_") != -1 or report.logic.find("MANABURN_ALLFOES_") != -1) {
						continue; // already converted to damage_single
					}

					if (report.sData.hasKey("ifCondition")) {
						List<string> targetHasAStatus = List<string>::quickMake({"TARGETHASDUSTY", "TARGETHASBLEEDING"});
						if (targetHasAStatus.contains(report.sData["ifCondition"])) {
							string effectName = SReplace(report.sData["ifCondition"], "TARGETHAS", "");
							if (!combat.currentBattle->doesTargetXHaveStatusY(report.combatantsAffected.at(0), effectName)) {
								report.sData["success"] = "0";
								report.sData["failSilently"] = "1";
							}
						}
					}

					if (report.logic == "BURNING") {
						if (combat.currentBattle->getThisCombatant(report.combatantsAffected.front())->c.isDead()) {
							report.sData["success"] = "0";
							report.sData["failSilently"] = "1";
						}
						else {
							report.logic = "DAMAGE_SINGLE_FIRE";
							report.vData["DAMAGE_SINGLE_FIRE"] = 20;
						}
					}

					if (report.combatantsAffected.contains("WORLD")) {
						// an effect stack executed by a world effect may need to be retargeted if it does not affect all targets by default. i.e. Healing Rain
						if (report.logic.find("_AOE_") != -1) {
							List<CombatantInstance* > newTargets = combat.currentBattle->getThisCombatant(report.originalUser)->getAllMyAllies(*&combat);
							List<string> newTargetNames;
							for (CombatantInstance* c : newTargets.internalList) {
								newTargetNames.push_back(c->c.uniqueCombatID);
							}
							report.combatantsAffected = newTargetNames;
						}
						else {
							continue;
						}
					}
					for (auto t : report.combatantsAffected.internalList) {
						if (combat.currentBattle->doesThisCombatantExist(t)) {
							targets.push_back(combat.currentBattle->getThisCombatant(t));
						}
					}
					List<string> reportKeys = report.vData.getKeys();

					if (report.logic.find("INTERRUPTIF?TARGETHAS") != -1) {
						string data = SReplace(report.logic, "INTERRUPTIF?TARGETHAS", "");
						string req = split(data, "_").at(0);
						string howMany = split(data, "_").at(1);
						for (CombatantInstance* actor : targets.internalList) {
							if (combat.currentBattle->doesTargetXHaveStatusY(actor->c.uniqueCombatID, req)) {
								report.logic = "INTERRUPT_SINGLE";
							}
						}

					}

					// must come after the interrupt_single report
					if (report.logic.find("IF?INTERRUPTED") != -1) {
						bool failed = true;
						for (auto & subreport : ongoingReport.internalList) {
							if (subreport.logic.find("INTERRUPT_SINGLE_") != -1 and subreport.sData["success"] == "1") {
								report.logic = SReplace(report.logic, "IF?INTERRUPTED", "");
								failed = false;
							}
						}
						if (failed) {
							report.sData["success"] = "-1";
							report.sData["failSilently"] = "1";
						}
					}

					if (report.logic.find("INTERRUPT_SINGLE") != -1) {
						for (EffectObjectInstance* effect : combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(report.originalUser).internalList) {
							if (effect->e.triggers.contains("ONTRYINGTOINTERRUPT")) {
								if (effect->e.uniqueID == "Stone Curse") {
									report.sData["success"] = "0";
									report.sData["failReason"] = "STONECURSE";
									continue;
								}
								if (effect->e.uniqueID == "Suppress") {
									report.sData["success"] = "0";
									report.sData["failReason"] = "SUPPRESS";
									continue;
								}
							}
						}
						// put here anything that can prevent interruption
						for (CombatantInstance* actor : targets.internalList) {
							if (actor->c.currentlyCasting) {
								List<EffectObjectInstance*> effectsOnTarget = combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(actor->c.uniqueCombatID);
								for (EffectObjectInstance* effect : effectsOnTarget.internalList) {
									if (effect->e.effectNames.contains("PROTECTVSINTERRUPTION")) {
										report.sData["success"] = "0";
										report.sData["failReason"] = effect->e.uniqueID;
									}
								}
								if (report.sData["success"] != "0") {
								Map<string, string> interruptedData;
								interruptedData["language"] = combat.language;
								interruptedData["name"] = actor->c.uniqueID;
								interruptedData["interrupter"] = user->c.uniqueID;
								interruptedData["interruption"] = report.sourceName;
								interruptedData["skill"] = actor->c.combatSkills[actor->c.indexOfSkillCurrentlyBeingCast].uniqueID;
								int indexBeingCast = actor->c.indexOfSkillCurrentlyBeingCast;
								actor->c.finishCasting(*&combat);
								if (report.sourceName == "Rude Awakening") {
									int power = report.vData["DISABLEPOWER"];
									actor->c.combatSkills[indexBeingCast].timeToRecharge += power;
								}
								combat.currentBattle->addCombatMessage("INTERRUPTED", interruptedData, 0);
								}
							}
						}
					}

					if (report.logic.find("LIFEHEAL_SINGLE") != -1) {
						if (report.sourceName == "Fight the Pain!") {
							int basePower = combat.currentBattle->getThisEffect(report.combatantsAffected.front(), "Fight the Pain!")->e.values["power"];
							report.vData["LIFEHEAL_SINGLE_HOLY"] = basePower * combat.currentBattle->getAllEffectsOnXThatAreThisType(report.combatantsAffected.front(), "BANE").size();
						}
						if (report.sourceName == "Conciliatory Prayer") {
							CombatantInstance* user = combat.currentBattle->getThisCombatant(report.originalUser);
							CombatantInstance* target = combat.currentBattle->getThisCombatant(report.combatantsAffected.front());
							if (user->c.combatStats["CURRENTLIFE"] > target->c.combatStats["CURRENTLIFE"]) {
								report.vData["LIFEHEAL_SINGLE_HOLY"] *= 1.5;
							}
						}
						for (auto v : reportKeys.internalList) {
							for (CombatantInstance* target : targets.internalList) {
								if (target->c.isDead()) {
									if (v.find("LIFEHEAL") != -1) {
										report.vData[v] = 0;
									}
								}
							}
							for (EffectObjectInstance* effect : allEffects.internalList) {
								if (effect->e.target == "WORLD") { continue; }
								CombatantInstance* target = combat.currentBattle->getThisCombatant(effect->e.target);
								if (user->c.uniqueCombatID == effect->e.target) { // the effect is on the one doing the healing
									if (effect->e.uniqueID == "Incessant Devotion") {
										if (v == "LIFEHEAL_SINGLE_HOLY") {
											float power = (100 + effect->e.values["power"]) / 100.0;
											report.vData[v] *= power;
										}
									}
								
								}
								if (targets.contains(target)) { // the effect is on the one being healed
									if (v == "LIFEHEAL_SINGLE_HOLY" and effect->e.triggers.contains("ONBEINGHEALED")) {
										if (effect->e.logicName == "UNDEAD") {
											report.vData["LIFEHEAL_SINGLE_HOLY"] /= 2;
										}
									}
								}
							}
						}
					}

					if (report.logic.find("LIFESACRIFICE_SELF") != -1) {
						// convert percentage to actual
						int powerAsPercentage = report.vData["LIFESACRIFICE_SELF_UNHOLY"];
						float resultPower = (user->c.combatStats["LIFE"] * powerAsPercentage) / 100;
						report.vData["LIFESACRIFICE_SELF_UNHOLY"] = resultPower;
						// effects which make user sacrifice more
					}

					if (report.logic.find("LIFESTEAL_SINGLE") != -1 or report.vData.hasKey("LIFESTEAL_SINGLE_UNHOLY")) {
						if (report.sourceName == "Botched Procedure") {
							List<EffectObjectInstance*> effectsOnVictim = combat.currentBattle->getAllEffectsOnXThatAreThisType(report.combatantsAffected.front(), "BANE");
							report.vData["power"] = effectsOnVictim.size();
							report.vData["LIFESTEAL_SINGLE_UNHOLY"] *= report.vData["power"];
						}
						for (EffectObjectInstance* effect : allEffects.internalList) {
							if (effect->e.target == "WORLD") { 
								// world effects which would take place here
								continue; 
							}
							CombatantInstance* target = combat.currentBattle->getThisCombatant(effect->e.target);
							if (targets.contains(target)) {
								if (effect->e.logicName == "LIFESTEAL_SINGLE_UNHOLY") {
									report.vData["LIFESTEAL_SINGLE_UNHOLY"] = effect->e.values["power"];
								}
							}
							if (effect->e.target == report.originalUser) { // this effect is on the user
								if (effect->e.triggers.contains("ONSTEALINGLIFE")) {
									if (effect->e.logicName == "Viper Eyes") {
										if (report.vData.hasKey("LIFESTEAL_SINGLE_UNHOLY")) {
											report.vData["LIFESTEAL_SINGLE_UNHOLY"] *= 1.8;
										}
									}
								}
							}
						}
					}

					if (report.sourceName == "Exalted Smash" and user->c.countSkillsByCategory("OFFENSIVE") == 1) {
						report.vData["DAMAGE_SINGLE_HOLY"] *= 2;
					}

					if (report.logic.find("DAMAGE_SINGLE") != -1) {
						if (report.sourceName == "Vapour Blade") {
							if (combat.currentBattle->getAllEffectsOnXThatAreThisType(report.combatantsAffected.front(), "BANE").empty()) {
								report.vData["DAMAGE_SINGLE_COLD"] *= 2;
							}
						}
						if (report.sourceName == "Bewrayment") {
							int power = report.vData["bonus"];
							int modifier = targets.front()->getActorsISummoned(*&combat).size();
							report.vData["DAMAGE_SINGLE_HOLY"] += (power * modifier);
						}
						if (report.sourceName == "Magebane Strike") {
							int bonusPower = report.vData["BONUS"];
							int multiplier = targets.front()->c.countSkillsByCategory("SPELL");
							report.vData["DAMAGE_SINGLE_PHYSICAL"] += (bonusPower * multiplier);
						}
						if (report.sourceName == "Mourning Edge") {
							if (user->getMyFoesThatAreAlive(*&combat).size() < user->getMyFoes(*&combat).size()) {
								report.vData["DAMAGE_SINGLE_PHYSICAL"] *= 1.66;
							}
						}
						if (report.sourceName == "Excommunicative Assault") {
							int bonus = report.vData["bonus"];
							int divider = user->getMyFoesThatAreAlive(*&combat).size();
							if (divider > 0) {
								// it should never be smaller than 1 but just in case
								report.vData["DAMAGE_SINGLE_PHYSICAL"] += (bonus / divider);
							}
						}
						if (report.sourceName == "Crazed Chop") {
							int lower = report.vData["LOWER"];
							int upper = report.vData["UPPER"];
							int power = RANDOM.getRandom(lower, upper);
							report.vData["DAMAGE_SINGLE_PHYSICAL"] = power;
						}
						if (report.sourceName == "Deathdancer's Strike") {
							float victimLife = combat.currentBattle->getThisCombatant(report.combatantsAffected.front())->c.getLifeAsPercentage();
							int basePower = report.vData["DAMAGE_SINGLE_PHYSICAL"];
							if (victimLife < 0.66) { basePower *= 1.33; }
							if (victimLife < 0.33) { basePower *= 1.33; }
							report.vData["DAMAGE_SINGLE_PHYSICAL"] = basePower;
						}
						if (report.sourceName == "Natural Stab") {
							string victim = report.combatantsAffected.front();
							if (combat.currentBattle->getAllEffectsOnXThatAreThisType(victim, "BOON").empty() and combat.currentBattle->getAllEffectsOnXThatAreThisType(victim, "BANE").empty()) {
								report.vData["DAMAGE_SINGLE_PHYSICAL"] *= 2;
							}
						}
						for (auto v : reportKeys.internalList) {
							for (EffectObjectInstance* effect : allEffects.internalList) {
								if (effect->e.target == report.originalUser) { // this effect is on the user		
									if (effect->e.triggers.contains("ONDOINGPHYSICALATTACK")) {
										if (effect->e.logicName == "Bailiff's Blade") {
											combat.currentBattle->removeAnEffect(user->c.uniqueCombatID, "CRIPPLED");
										}
										if (effect->e.logicName == "I Shall Take Care of This!" and v == "DAMAGE_SINGLE_PHYSICAL") {
											report.vData["DAMAGE_SINGLE_PHYSICAL"] += effect->e.values["power"];
										}
										if (effect->e.logicName == "Blade of Blood") {
											if (report.vData.hasKey("DAMAGE_SINGLE_PHYSICAL")) {
												report.vData["LIFESTEAL_SINGLE_UNHOLY"] += effect->e.values["power"];
											}
										}
										if (effect->e.logicName == "Strength of Reason") {
											if (report.vData.hasKey("DAMAGE_SINGLE_PHYSICAL")) {
												report.vData["DAMAGE_SINGLE_HOLY"] += effect->e.values["power"];
											}
										}
										if (effect->e.logicName == "Fading Justice") {
											if (report.vData.hasKey("DAMAGE_SINGLE_PHYSICAL")) {
												int modifier = effect->e.values["power"];
												int multiplier = targets.front()->c.countSkillsByCategory("ATTACK");
												int extraDamage = modifier * multiplier;
												report.vData["DAMAGE_SINGLE_HOLY"] += extraDamage;
											}
										}
										if (effect->e.logicName == "Order of the Wasp") {
											if (report.vData.hasKey("DAMAGE_SINGLE_PHYSICAL")) {
												report.vData["MANASTEAL_SINGLE_UNHOLY"] += effect->e.values["power"];
											}
										}
									}
									if (effect->e.logicName == "Conversion" and v.find("HOLY") != -1) {
										float modifier = effect->e.values["power"];
										float originalDamage = report.vData[v];
										float newDamage = (originalDamage / 100.0) * modifier;
										int newDmg = round(newDamage);
										report.vData["DAMAGE_FIRE_SINGLE"] += newDmg;
									}
								}
								if (effect->e.target == "WORLD") {
									// any world effects that would happen at this stage (probably none)
									continue;
								}
								CombatantInstance* target = combat.currentBattle->getThisCombatant(effect->e.target);
								if (targets.contains(target)) { // this effect is on the victim
									if (effect->e.uniqueID == "Reckless Swing") {
										report.vData[v] *= 1.2;
									}
									if (effect->e.uniqueID == "On My Target!" and v == "DAMAGE_SINGLE_PHYSICAL") {
										report.vData["DAMAGE_SINGLE_PHYSICAL"] += effect->e.values["power"];
									}
									if (effect->e.uniqueID == "Bramble Cloak" and combat.currentBattle->isThisDamageElemental(v)) {
										float power = (100.0 - effect->e.values["power"]) / 100;
										report.vData[v] *= power;
									}
									if (effect->e.uniqueID == "Cleave Armour" and combat.currentBattle->isThisDamageElemental(v) or combat.currentBattle->isThisDamagePhysical(v)) {
										float power = (effect->e.values["power"] + 100.0) / 100.0;
										report.vData[v] *= power;
									}
									if (effect->e.uniqueID == "Incessant Devotion") {
										if (v.find("DAMAGE_SINGLE_") != -1) {
											report.vData[v] *= 1.33;
										}
									}
									if (effect->e.uniqueID == "Ivory Sanctuary") {
										if (combat.skillDefinitions.hasKey(report.sourceName) and not combat.skillDefinitions[report.sourceName].skillTypeTags.contains("MAGICAL")) {
											if (v.find("DAMAGE_SINGLE_") != -1) {
												float power = (100.0 - effect->e.values["power"]) / 100;
												report.vData[v] *= power;
											}
										}
									}
									if (effect->e.uniqueID == "Shield of a Goddess") {
										if (combat.skillDefinitions.hasKey(report.sourceName) and combat.skillDefinitions[report.sourceName].skillTypeTags.contains("MAGICAL")) {
											if (v.find("DAMAGE_SINGLE_") != -1) {
												float power = (100.0 - effect->e.values["power"]) / 100;
												report.vData[v] *= power;
											}
										}
									}
									if (effect->e.uniqueID == "Curse from Beyond the Grave") {
										if (combat.currentBattle->getThisCombatant(report.originalUser)->wasISummoned()) {
											for (auto key : report.vData.getKeys().internalList) {
												if (key.find("DAMAGE_") != -1) {
													report.vData[key] *= 2;
												}
											}
										}
									}
									if (effect->e.uniqueID == "Viper Eyes") {
										report.vData[v] *= 1.33;
									}
									if (effect->e.uniqueID == "Fragile") {
										report.vData[v] *= 100;
									}
									if (effect->e.uniqueID == "Sanctum Shroud") {
										report.vData[v] = TChange(report.vData[v], effect->e.values["power"] * -1, 0, 9999);
										if (target->c.combatStats["CURRENTLIFE"] < target->c.combatStats["LIFE"] / 2) {
											combat.currentBattle->removeAnEffect(target->c.uniqueCombatID, "Sanctum Shroud");
										}
									}
									if (effect->e.uniqueID == "Go On Without Me!") {
										report.vData[v] *= 0.75;
									}
									if (v == "DAMAGE_SINGLE_ELECTRIC" and effect->e.triggers.contains("ONTAKINGELECTRICDAMAGE")) {
										if (effect->e.logicName == "ARMOURVSELECTRIC") {
											float power = (100.0 - effect->e.values["power"]) / 100;
											report.vData["DAMAGE_SINGLE_ELECTRIC"] *= power;
										}
										if (effect->e.logicName == "WET") {
											report.vData["DAMAGE_SINGLE_ELECTRIC"] *= 1.25;
										}
										if (effect->e.logicName == "STATIC") {
											report.vData["DAMAGE_SINGLE_ELECTRIC"] *= 1.25;
										}
										if (effect->e.logicName == "DUSTY") {
											report.vData["DAMAGE_SINGLE_ELECTRIC"] *= 0.9;
										}
										if (effect->e.logicName == "Global Warming") {
											report.vData["DAMAGE_SINGLE_ELECTRIC"] *= 1.2;
										}
									}
									if (v == "DAMAGE_SINGLE_FIRE" and effect->e.triggers.contains("ONTAKINGFIREDAMAGE")) {
										if (effect->e.logicName == "WET") {
											report.vData["DAMAGE_SINGLE_FIRE"] *= 0.9;
										}
										if (effect->e.logicName == "PARCHED") {
											report.vData["DAMAGE_SINGLE_FIRE"] *= 1.25;
										}
										if (effect->e.logicName == "STATIC") {
											report.vData["DAMAGE_SINGLE_FIRE"] *= 1.25;
										}
										if (effect->e.logicName == "Global Warming") {
											report.vData["DAMAGE_SINGLE_FIRE"] *= 1.2;
										}
									}
									if (v == "DAMAGE_SINGLE_COLD" and effect->e.triggers.contains("ONTAKINGCOLDDAMAGE")) {
										if (effect->e.logicName == "WET") {
											report.vData["DAMAGE_SINGLE_COLD"] *= 1.25;
										}
										if (effect->e.logicName == "DUSTY") {
											report.vData["DAMAGE_SINGLE_ELECTRIC"] *= 1.25;
										}
										if (effect->e.logicName == "PARCHED") {
											report.vData["DAMAGE_SINGLE_FIRE"] *= 0.9;
										}
										if (effect->e.logicName == "Ice Age") {
											report.vData["DAMAGE_SINGLE_COLD"] *= 1.2;
										}
									}
									if (v == "DAMAGE_SINGLE_EARTH" and effect->e.triggers.contains("ONTAKINGEARTHDAMAGE")) {
										if (effect->e.logicName == "DUSTY") {
											report.vData["DAMAGE_SINGLE_EARTH"] *= 1.25;
										}
										if (effect->e.logicName == "PARCHED") {
											report.vData["DAMAGE_SINGLE_FIRE"] *= 1.25;
										}
										if (effect->e.logicName == "STATIC") {
											report.vData["DAMAGE_SINGLE_FIRE"] *= 0.9;
										}
										if (effect->e.logicName == "Drought") {
											report.vData["DAMAGE_SINGLE_EARTH"] *= 1.2;
										}
									}
									if (v == "DAMAGE_SINGLE_HOLY" and effect->e.triggers.contains("ONTAKINGHOLYDAMAGE")) {
										if (effect->e.logicName == "UNDEAD") {
											report.vData["DAMAGE_SINGLE_HOLY"] *= 2;
										}
									}
									// these effects must be processed after the rest
									if (effect->e.uniqueID == "Angelic Observatory") {
										if (v.find("DAMAGE_SINGLE") != -1) {
											int limit = target->c.combatStats["LIFE"] * 0.1;
											if (report.vData[v] > limit) {
												report.vData[v] = limit;
											}
										}
									}
									if (effect->e.uniqueID == "Paraclete's Invitation") {
										int power = effect->e.values["power"];
										int limit = target->c.combatStats["LIFE"] / power;
										if (report.vData[v] > limit) {
											report.vData[v] = limit;
										}
									}
								}
							}
						}
					}
					if (report.logic.find("MANABURN_SINGLE") != -1 or report.logic.find("MANASTEAL_SINGLE") != -1) {
						// effects which enhance or reduce mana damage
						if (report.combatantsAffected.front() == "WORLD") { continue; }
						
					}
				
					if (report.logic.find("RESURRECT_SINGLE") != -1) {
						// any effects which impact resurrection (such as res prevention)
					}

					if (report.logic.find("REMOVEBOONANDMANASTEAL_") != -1) {
						// any effects which impact removing a boon
						CombatantInstance* target = combat.currentBattle->getThisCombatant(report.combatantsAffected.front());
						List<EffectObjectInstance*> effects = combat.currentBattle->getAllEffectsOnXThatAreThisType(target->c.uniqueCombatID, "BOON");
						string element = split(report.logic, "_").back();
						int power = report.vData["POWER_" + report.sourceName];
						if (effects.empty()) {
							report.sData["success"] = "0";
							report.sData["failReason"] = "noEnchantmentToDrain";
						}
						else {
							report.sData["success"] = "1";
							report.sData["removeThisBoon"] = effects.back()->e.uniqueID;
							report.vData["MANASTEAL_SINGLE_" + element] = power;
						}
					}
					
					if (report.logic.find("REMOVEBANE_") != -1) {
						report.sData["success"] = "1";
						report.vData["howManyBanes"] = 1;
					}
					if (report.logic.find("REMOVEXBANES_") != -1) {
						report.sData["success"] = "1";
						report.vData["howManyBanes"] = report.vData["power"];
					}
					if (report.logic.find("APPLY_") != -1) {
						// change duration
						if (user == targets.front()) {// using on self
							if (combat.skillDefinitions.hasKey(report.sourceName)) { // the effect is a skill
								if (combat.currentBattle->doesTargetXHaveStatusY(user->c.uniqueCombatID, "Lord's Authority")) {
									if (combat.skillDefinitions[report.sourceName].skillTypeTags.contains("PHYSICAL")) {
										for (auto v : report.vData.getKeys().internalList) {
											if (v.find("DURATION_") != -1) {
												report.vData[v] *= 2;
											}
										}
									}
								}
							}
						}

						// prevent application of effects
						List<string> effectData = split(report.logic, "_");
						string effectName = effectData.at(1);
						EffectObject theEffect = combat.allEffectDefinitions[effectName];
						bool isABane = theEffect.type == "BANE";
						for (CombatantInstance* target : targets.internalList) {
							string whoWillGetCondition = target->c.uniqueCombatID;
							if (report.logic.find("_SELF") != -1) {
								whoWillGetCondition = report.originalUser;
							}
							if (isABane and combat.currentBattle->doesTargetXHaveStatusY(whoWillGetCondition, "Remedy Ward")) {
								report.sData["success"] = "0";
								report.sData["failSilently"] = "1";
								report.sData["failReason"] = "Remedy Ward";
								combat.currentBattle->removeAnEffect(target->c.uniqueCombatID, "Remedy Ward");
							}
						}
						// these events are triggered / cancelled by the failure to apply a bane
						if (isABane and report.sData["success"] == "0") {
							for (auto & reportAgain : ongoingReport.internalList) {
								if (reportAgain.sourceName == "Shock Value") {
									reportAgain.sData["success"] = "0";
								}
							}
							
						}
					}
}

				// predict if targets will die during resolution
				Map<string, int> projectedLifeLoss;
				for (auto& report : ongoingReport.internalList) {
					if (report.vData.hasKey("LIFESACRIFICE_SELF")) {
						projectedLifeLoss[report.originalUser] += report.vData["LIFESACRIFICE_SELF"];
					}
					List<CombatantInstance* > targets;
					for (auto t : report.combatantsAffected.internalList) {
						if (combat.currentBattle->doesThisCombatantExist(t)) {
							targets.push_back(combat.currentBattle->getThisCombatant(t));
						}
					}
					for (CombatantInstance* t : targets.internalList) {
						for (auto v : report.vData.getKeys().internalList) {
							if (v.find("DAMAGE_") != -1 or v.find("LIFESTEAL_") != -1) {
								projectedLifeLoss[t->c.uniqueCombatID] += report.vData[v];
							}
							else if (v.find("LIFEHEAL_") != -1) {
								projectedLifeLoss[t->c.uniqueCombatID] -= report.vData[v];
							}
						}
					}
				}
				for (auto& report : ongoingReport.internalList) {
					for (auto name : projectedLifeLoss.getKeys().internalList) {
						if (report.sData["success"] != "1") { continue; }
						if (projectedLifeLoss[name] >= combat.currentBattle->getThisCombatant(name)->c.combatStats["CURRENTLIFE"]) {
							report.sData["THISPERSONDIED"] += name + "$";
						}
					}
				}

				// consider any effects that respond to actually taking damage
				for (auto& report : ongoingReport.internalList) {
					for (auto victim : report.combatantsAffected.internalList) {
						if (victim == "WORLD") { continue; }
						if (victim == report.originalUser) { continue; }
						CombatantInstance* who = combat.currentBattle->getThisCombatant(victim);
						if (combat.currentBattle->doesTargetXHaveStatusY(victim, "Avenger's Prayer")) {
							EffectObjectInstance* e = combat.currentBattle->getThisEffect(victim, "Avenger's Prayer");
							int damageTaken = 0;
							int modifier = e->e.values["power"];
							bool actuallyTookDamage = false;
							for (auto v : report.vData.getKeys().internalList) {
								if (v.find("DAMAGE_") != -1 and v != "DAMAGE_SINGLE_HOLY") {
									damageTaken += report.vData[v];
									actuallyTookDamage = true;
								}
							}
							if (actuallyTookDamage) {
								float step1 = damageTaken / 100.0;
								float step2 = modifier * 1.0;
								float step3 = step1 * step2;
								int newDamage = round(step3);

								ongoingReport.push_back(CombatEvent("DAMAGE_SINGLE_HOLY", "Avenger's Prayer", "Avenger's Prayer",victim, List<string>(report.originalUser), List<pair<string, string>>(
									pair<string, string>("success", "1")),
									List<pair<string, int>>(
										pair<string, int>("DAMAGE_SINGLE_HOLY", newDamage))));
							}
						}
					}
				}

				List<string> everyoneWhoDied;
				// consider any effects that happen if a target dies
				for (auto& report : ongoingReport.internalList) {
					if (report.sData.hasKey("THISPERSONDIED")) {
						List<string> whoDied = split(report.sData["THISPERSONDIED"], "$");
						for (auto dead : whoDied.internalList) {
							everyoneWhoDied.addToBackIfNotAlreadyInList(dead);
						}
						if (report.vData.hasKey("HEAVENSTRIKE")) {
							ongoingReport.push_back(CombatEvent("DELAYTHISSKILLRECHARGE", "SKILL", "Heal Wounds", report.originalUser, {}, {}, List<pair<string, int>>(
								pair<string, int>("DELAYTHISSKILLRECHARGE", report.vData["SKILLRECHARGEDELAY"]))));
							List<CombatantInstance*> targets = combat.currentBattle->getThisCombatant(report.originalUser)->getMyTeam(*&combat);
							for (CombatantInstance* target : targets.internalList) {
								ongoingReport.push_back(CombatEvent("LIFEHEAL_SINGLE_HOLY", "Heal Wounds All", "Heal Wounds All", report.originalUser, List<string>(target->c.uniqueCombatID), List<pair<string,string>>(
									pair<string, string>("success", "1")), List<pair<string, int>>(
									pair<string, int>("LIFEHEAL_SINGLE_HOLY", report.vData["HEAVENSTRIKE"]))));
							}
						}
					}
				}

				

				if (!everyoneWhoDied.empty()) {
						for (CombatantInstance* actor : combat.currentBattle->getAllCombatants().internalList) {
						if (actor->c.isDead()) {continue;}
						if (combat.currentBattle->doesTargetXHaveStatusY(actor->c.uniqueCombatID, "Lacrymactory")) {
							EffectObjectInstance* e = combat.currentBattle->getThisEffect(actor->c.uniqueCombatID, "Lacrymactory");
							ongoingReport.push_back(CombatEvent("MANAHEAL_SINGLE_UNHOLY", "Lacrymactory", "Lacrymactory", actor->c.uniqueCombatID, List<string>(actor->c.uniqueCombatID), List<pair<string, string>>(
								pair<string, string>("success", "1")), 
							List<pair<string, int>>(
								pair<string, int>("MANAHEAL_SINGLE_UNHOLY", e->e.values["power"]))));
						}
					}
				}

				// consider any effects that happen if a bane is applied to someone
				for (auto& report : ongoingReport.internalList) {
					if (combat.currentBattle->doesTargetXHaveStatusY(report.originalUser, "Shock Value")) {
						if (report.logic.find("APPLY_") != -1) {
							string applyWhat = split(report.logic, "_").at(1);
							if (!combat.allEffectDefinitions.hasKey(applyWhat)) {
								throw exception("There is no effect with this name.");
							}
							if (combat.allEffectDefinitions[applyWhat].type == "BANE") {
								EffectObjectInstance* shockValue = combat.currentBattle->getThisEffect(report.originalUser, "Shock Value");
								ongoingReport.push_back(CombatEvent("LIFEHEAL_SINGLE_ELECTRIC", "SKILL", "Shock Value", report.originalUser, List<string>(report.originalUser), List<pair<string, string>>(
									pair<string, string>("success", "1")), List<pair<string, int>>(
										pair<string, int>("LIFEHEAL_SINGLE_ELECTRIC", shockValue->e.values["POWER2"]))));
								ongoingReport.push_back(CombatEvent("MANAHEAL_SINGLE_ELECTRIC", "SKILL", "Shock Value", report.originalUser, List<string>(report.originalUser), List<pair<string, string>>(
									pair<string, string>("success", "1")), List<pair<string, int>>(
										pair<string, int>("MANAHEAL_SINGLE_ELECTRIC", shockValue->e.values["POWER1"]))));
							}
						}
					}
				}
			};

			void executeTheResults(Combat& combat, string language) {
				// don't repeat fail messages for every failed effect belonging to one skill
				List<string> failedSkillExecutions;

				// execute the final results of what happened in the stack
				for (auto& report : ongoingReport.internalList) {
					string user = report.originalUser;
					if (report.sData["success"] == "1") {
						if (report.logic == "WISHINGWELL_SELF") {
							CombatantInstance* actor = combat.currentBattle->getThisCombatant(user);
							List<CombatantInstance*> allAllies = actor->getAllMyOtherAlliesNotMe(*&combat);
							for (CombatantInstance* person : allAllies.internalList) {
								for (auto skill : person->c.combatSkills.getKeys().internalList) {
									if (person->c.combatSkills[skill].uniqueID == "Wishing Well") { continue; }
									person->c.combatSkills[skill].timeToRecharge = 0;
								}
							}
						}
						if (report.logic == "SUICIDE_SELF") {
							combat.currentBattle->getThisCombatant(user)->c.combatStats["CURRENTLIFE"] = 0;
						}
						if (report.logic.find("EXILE") != -1) {
							for (auto currentTarget : report.combatantsAffected.internalList) {
								combat.currentBattle->getThisCombatant(currentTarget)->c.combatStats["CURRENTLIFE"] = 0;
							}
						}
						if (report.logic.find("APPLY_") != -1) {
							List<string> effectData = split(report.logic, "_");
							if (report.logic.find("_ALLFOES") != -1) {
								CombatantInstance* actor = combat.currentBattle->getThisCombatant(report.originalUser);
								List<CombatantInstance*> toApplyTo = actor->getMyFoesThatAreAlive(*&combat);
								for (CombatantInstance* person : toApplyTo.internalList) {
									report.combatantsAffected.addToBackIfNotAlreadyInList(person->c.uniqueCombatID);
								}
							}
							if (report.combatantsAffected.contains("ALLALLIES") or report.logic.find("_ALLIES") != -1) {
								CombatantInstance* actor = combat.currentBattle->getThisCombatant(report.originalUser);
								List<CombatantInstance*> toApplyTo = actor->getAllMyAllies(*&combat);
								for (CombatantInstance* person : toApplyTo.internalList) {
									if (person->c.isDead()) { continue; }
									report.combatantsAffected.addToBackIfNotAlreadyInList(person->c.uniqueCombatID);
								}
							}
							if (report.combatantsAffected.contains("ALL") or (report.logic.find("ALLALLIES") == -1 and report.logic.find("_ALL") != -1 and report.logic.find("_ALLOTHERALLIES") == -1
								and report.logic.find("_ALLFOES") == -1
								)) {
								report.combatantsAffected = combat.currentBattle->all.getKeys();
							}
							if (report.logic.find("_SELF") != -1) {
								report.combatantsAffected = List<string>({report.originalUser});
							}
							string effectName = effectData.at(1);
							if (effectName == "On My Target!") { // remove all other copies on that team
								for (CombatantInstance* actor : combat.currentBattle->getThisCombatant(report.originalUser)->getMyFoesThatAreAlive(*&combat).internalList) {
									if (combat.currentBattle->doesTargetXHaveStatusY(actor->c.uniqueCombatID, "On My Target!")) {
										combat.currentBattle->removeAnEffect(actor->c.uniqueCombatID, "On My Target!");
									}
								}
							}



							for (auto currentTarget : report.combatantsAffected.internalList) {
								int duration = report.vData["DURATION_" + effectName];
								int power = report.vData["POWER_" + effectName];

								// these effects convert banes from one to another so change here first
								if (combat.currentBattle->doesTargetXHaveStatusY(currentTarget, "Ring of Ash")) {
									if (combat.allEffectDefinitions[effectName].type == "BANE") {
										if (effectName != "BURNING") {
											effectName = "BURNING";
											report.logic = "APPLY_BURNING_SINGLE";
											EffectObjectInstance* ringOfAshEffect = combat.currentBattle->allEffectsInPlay[currentTarget]["Ring of Ash"];
											ringOfAshEffect->e.charges--;
											if (ringOfAshEffect->e.charges == 0) {
												combat.currentBattle->removeAnEffect(currentTarget, "Ring of Ash");
											}
										}
									}
								}

								if (report.logic == "APPLY_FROZEN_SINGLE" and combat.currentBattle->doesTargetXHaveStatusY(currentTarget, "Ice Age")) {
									duration  *= 2;
								}
								if (report.logic == "APPLY_BURNING_SINGLE" and combat.currentBattle->doesTargetXHaveStatusY(currentTarget, "Global Warming")) {
									duration *= 2;
								}
								if (report.logic == "APPLY_CONCUSSED_SINGLE" and combat.currentBattle->doesTargetXHaveStatusY(currentTarget, "Tempest")) {
									duration *= 2;
								}
								if (report.logic == "APPLY_CRIPPLED_SINGLE" and combat.currentBattle->doesTargetXHaveStatusY(currentTarget, "Drought")) {
									duration *= 2;
								}
								if (report.vData.hasKey(report.logic)) {
									// effects that are already in play and apply their own effects may store power and duration differently
									duration = report.vData[report.logic];
								}
								if (combat.currentBattle->doesTargetXHaveStatusY(currentTarget, "Underworld Dreams")) {
									if (combat.allEffectDefinitions[effectName].type == "BOON") {
										bool durationWasMoreThanZero = duration > 0; // if the duration was zero for some reason don't make it 1 round longer than it should be
										duration /= 2;
										if (durationWasMoreThanZero and duration < 1) {
											duration = 1;
										}
									}
								}
								Map<string, int> values = { List<pair<string, int>>({
									pair<string, int>("duration", duration),
									pair<string, int>("power", power),
									}) };
								// extra values that come from the skill data
								if (combat.skillDefinitions.hasKey(effectName)) {
									CombatantInstance* actor = combat.currentBattle->getThisCombatant(user);
									Skill skillBeingUsed = actor->c.getSkillByName(effectName);
									skillBeingUsed.powerValues;
									for (auto value : skillBeingUsed.powerValues.getKeys().internalList) {
										if (!values.hasKey(value)) {
											values[value] = actor->c.getPowerOfThis(skillBeingUsed.powerValues[value],true, *&combat);
										}
									}
								}


								combat.currentBattle->applyAnEffect(*&combat, effectName, combat.allEffectDefinitions[effectName], report.originalUser, currentTarget, values);
								EffectObjectInstance* theEffect = combat.currentBattle->allEffectsInPlay[currentTarget][effectName];
								if (theEffect->e.triggers.contains("ONAPPLY")) {
									Map<string, int> subeffects = theEffect->executeDirectly(*&combat);
									for (auto subeffect : subeffects.getKeys().internalList) {
										report.vData[subeffect] = subeffects[subeffect];
									}
								}
								if (report.sourceName == "DISEASED2") {
									combat.currentBattle->addCombatMessage("DISEASE", List<pair<string, string>>({
												pair<string, string>("name",combat.currentBattle->getThisCombatant(user)->c.uniqueID),
												pair<string, string>("language",language),
												pair<string, string>("target",combat.currentBattle->all[currentTarget]->c.uniqueID),
										}), 0);
								}

								// if applied effect prevents user from casting X type of skill, stop that skill
								if (currentTarget != "WORLD") {
									CombatantInstance* actor = combat.currentBattle->getThisCombatant(currentTarget);
									List<string> skillsThatStopRes = List<string>({ "Entomb Spirit" });
									if (skillsThatStopRes.contains(theEffect->e.uniqueID) and actor->c.currentlyCasting) {
										Skill& skill = actor->c.getSkillBeingCast();
										if (skill.skillLogicNames.contains("RESURRECT_SINGLE_HOLY")) {
											actor->c.currentlyCasting = false;
										}
									}
								}
								

							}
						}
						if (report.logic.find("LIFESACRIFICE_SELF_UNHOLY") != -1) {
							combat.currentBattle->getThisCombatant(user)->c.takeDamage(report.vData["LIFESACRIFICE_SELF_UNHOLY"]);
						}
						if (report.logic.find("LIFEHEAL_") != -1) {
							for (auto effect : report.vData.getKeys().internalList) {
								if (effect.find("LIFEHEAL_") != -1) {
									string type = split(effect, "_").at(2);
									for (auto currentTarget : report.combatantsAffected.internalList) {
										CombatantInstance* actor = combat.currentBattle->getThisCombatant(currentTarget);
										List<EffectObjectInstance*> boons = combat.currentBattle->getAllEffectsOnXThatAreThisType(currentTarget, "BOON");
										if (report.sourceName == "Healing Winds" and boons.empty()) { continue; }
										combat.currentBattle->getThisCombatant(currentTarget)->c.beHealed(report.vData[effect]);
										combat.currentBattle->addCombatMessage("LIFEHEAL", List<pair<string, string>>({
												pair<string, string>("name",combat.currentBattle->getThisCombatant(user)->c.uniqueID),
												pair<string, string>("language",language),
												pair<string, string>("type",type),
												pair<string, string>("damage",to_string(report.vData[effect])),
												pair<string, string>("target",combat.currentBattle->all[currentTarget]->c.uniqueID),
											}), 0);
										toPrint.push_back(Result(currentTarget, to_string(report.vData[effect]), "HEALINGGREEN", graphics.accessImageViaUniqueID(currentTarget)->positionAsPercentage));
									}
								}
							}
						}
						if (report.logic.find("MANAHEAL_") != -1) {
							for (auto effect : report.vData.getKeys().internalList) {
								if (effect.find("MANAHEAL_") != -1) {
									string type = split(effect, "_").at(2);
									if (report.logic.find("MANAHEAL_SELF_") != -1) {
										report.combatantsAffected.clear();
										report.combatantsAffected.push_back(user);
									}
									for (auto currentTarget : report.combatantsAffected.internalList) {
										combat.currentBattle->getThisCombatant(currentTarget)->c.gainEnergy(report.vData[effect]);
										combat.currentBattle->addCombatMessage("MANAHEAL", List<pair<string, string>>({
												pair<string, string>("name",combat.currentBattle->getThisCombatant(user)->c.uniqueID),
												pair<string, string>("language",language),
												pair<string, string>("type",type),
												pair<string, string>("damage",to_string(report.vData[effect])),
												pair<string, string>("target",combat.currentBattle->all[currentTarget]->c.uniqueID),
											}), 0);
										toPrint.push_back(Result(currentTarget, to_string(report.vData[effect]), "HEALINGBLUE", graphics.accessImageViaUniqueID(currentTarget)->positionAsPercentage));
									}
								}
							}
						}
						if (report.logic.find("SUMMON_") != -1) {
							List<string> validSlots = combat.currentBattle->getThisCombatant(user)->getAllPossiblePositionsICouldPutASummon(*&combat);
							List<string> summonData = split(report.logic, "_");
							Map<string, int> vData = report.vData; // extra stats
							Combatant toSummon = combat.definedCombatants[summonData.at(1)];

							if (validSlots.empty()) {
								Map<string, string> failData;
								failData["name"] = combat.currentBattle->getThisCombatant(user)->c.uniqueID;
								failData["language"] = language;
								failData["summon"] = toSummon.uniqueID;
								combat.currentBattle->addCombatMessage("FAILEDSUMMONNOSPACE", failData, 0);
								combat.currentBattle->getThisCombatant(user)->c.currentTarget = user;
								report.sourceName = "FAILEDSKILL"; // don't animate anything
							}
							else {
								toSummon.intData = vData;
								toSummon.slot = validSlots.at(0);
								string targetTeam = combat.currentBattle->getThisCombatant(user)->team + "_ALLIES";
								string targetUniqueCombatID = targetTeam + "_" + toSummon.slot + "_" + toSummon.uniqueID;
								string direction = "BACK";
								if (combat.currentBattle->getThisCombatant(user)->team == "TEAM2" or combat.currentBattle->getThisCombatant(user)->team == "TEAM2_ALLIES") {
									direction = "FRONT";
								}
								toSummon.uniqueCombatID = targetUniqueCombatID;
								CombatantInstance* summoned = new CombatantInstance(toSummon, combat.currentBattle->getThisCombatant(user)->team + "_ALLIES");
								summoned->c.setCombatStats(*&combat, true);
								summoned->c.combatStats["CURRENTLIFE"] = vData["LIFE"]; // give them life described in the skill
								summoned->c.combatStats["LIFE"] = vData["LIFE"]; // give them life described in the skill
								summoned->c.data["EXTRAS"]["SUMMONED"] = "1";
								summoned->c.data["EXTRAS"]["WHOSUMMONEDME"] = user;
								combat.currentBattle->addNewCombatantDuringBattle(*&combat, summoned);
								combat.currentBattle->getThisCombatant(user)->c.currentTarget = targetUniqueCombatID;
								report.sData["SUMMONTHIS"] = toSummon.uniqueID;
								report.sData["SUMMONTHIS_DIRECTION"] = direction;
								report.sData["SUMMONTHIS_TEAMNAME"] = targetTeam;
								report.sData["SUMMONTHIS_SLOT"] = toSummon.slot;
								report.combatantsAffected.internalList = list<string>({ targetUniqueCombatID });
							}
						}
						if (report.logic.find("DAMAGE_") != -1) {
							if (report.logic.find("DAMAGE_ALLFOES_") != -1) {
								continue; // already converted to damage_single
							}
							for (auto effect : report.vData.getKeys().internalList) {
								if (effect.find("DAMAGE_") != -1) {
									string type = split(effect, "_").at(2);
									for (auto currentTarget : report.combatantsAffected.internalList) {
										combat.currentBattle->getThisCombatant(currentTarget)->c.takeDamage(report.vData[effect]);
										combat.currentBattle->addCombatMessage("DAMAGE", List<pair<string, string>>({
												pair<string, string>("name",combat.currentBattle->getThisCombatant(user)->c.uniqueID),
												pair<string, string>("language",language),
												pair<string, string>("type",type),
												pair<string, string>("damage",to_string(report.vData[effect])),
												pair<string, string>("target",combat.currentBattle->all[currentTarget]->c.uniqueID),
											}), 0);
										toPrint.push_back(Result(currentTarget, to_string(report.vData[effect]), "RED", graphics.accessImageViaUniqueID(currentTarget)->positionAsPercentage));
									}
								}
							}
						}
						if (report.logic.find("LIFESTEAL_") != -1 or !getKeysWhichContainX(report.vData.getKeys(), "LIFESTEAL_").empty()) {
							for (auto effect : report.vData.getKeys().internalList) {
								if (effect.find("LIFESTEAL") != -1) {
									for (auto currentTarget : report.combatantsAffected.internalList) {
										combat.currentBattle->getThisCombatant(currentTarget)->c.takeDamage(report.vData[effect]);
										combat.currentBattle->getThisCombatant(user)->c.beHealed(report.vData[effect]);
										combat.currentBattle->addCombatMessage("LIFESTEAL", List<pair<string, string>>({
												pair<string, string>("name",combat.currentBattle->getThisCombatant(user)->c.uniqueID),
												pair<string, string>("language",language),
												pair<string, string>("damage",to_string(report.vData[effect])),
												pair<string, string>("target",combat.currentBattle->getThisCombatant(currentTarget)->c.uniqueID),
											}), 0);
										toPrint.push_back(Result(currentTarget + "damage", to_string(report.vData[effect]), "DAMAGERED", graphics.accessImageViaUniqueID(currentTarget)->positionAsPercentage));
										toPrint.push_back(Result(user + "heal", to_string(report.vData[effect]), "HEALINGGREEN", graphics.accessImageViaUniqueID(user)->positionAsPercentage));
									}
								}
							}
						}
						if (report.logic.find("MANASTEAL_") != -1 or !getKeysWhichContainX(report.vData.getKeys(), "MANASTEAL_").empty()) {
							for (auto effect : report.vData.getKeys().internalList) {
								if (effect.find("MANASTEAL") != -1) {
									for (auto currentTarget : report.combatantsAffected.internalList) {
										combat.currentBattle->getThisCombatant(currentTarget)->c.takeManaDamage(report.vData[effect]);
										combat.currentBattle->getThisCombatant(user)->c.gainEnergy(report.vData[effect]);
										combat.currentBattle->addCombatMessage("ENERGYSTEAL", List<pair<string, string>>({
												pair<string, string>("name",combat.currentBattle->getThisCombatant(user)->c.uniqueID),
												pair<string, string>("language",language),
												pair<string, string>("damage",to_string(report.vData[effect])),
												pair<string, string>("target",combat.currentBattle->getThisCombatant(currentTarget)->c.uniqueID),
											}), 0);
										toPrint.push_back(Result(currentTarget + "damage", to_string(report.vData[effect]), "DAMAGEPURPLE", graphics.accessImageViaUniqueID(currentTarget)->positionAsPercentage));
										toPrint.push_back(Result(user + "heal", to_string(report.vData[effect]), "HEALINGBLUE", graphics.accessImageViaUniqueID(user)->positionAsPercentage));
									}
								}
							}
						}
						if (report.logic.find("MANABURN_") != -1) {
							for (auto effect : report.vData.getKeys().internalList) {
								if (effect.find("MANABURN_") != -1) {
									for (auto currentTarget : report.combatantsAffected.internalList) {
										if (currentTarget == "WORLD") { continue; }
										combat.currentBattle->getThisCombatant(currentTarget)->c.takeManaDamage(report.vData[effect]);
										combat.currentBattle->addCombatMessage("MANABURN", List<pair<string, string>>({
												pair<string, string>("name",combat.currentBattle->getThisCombatant(user)->c.uniqueID),
												pair<string, string>("language",language),
												pair<string, string>("damage",to_string(report.vData[effect])),
												pair<string, string>("target",combat.currentBattle->getThisCombatant(currentTarget)->c.uniqueID),
											}), 0);
										toPrint.push_back(Result(currentTarget + "damage", to_string(report.vData[effect]), "DAMAGEPURPLE", graphics.accessImageViaUniqueID(currentTarget)->positionAsPercentage));
									}
								}
							}
						}
						if (report.logic.find("MANABURN_") != -1 or report.logic.find("MANASTEAL") != -1 and report.combatantsAffected.front() != "WORLD") {
							string victim = report.combatantsAffected.front();
							List<EffectObjectInstance*> effectsOnVictim = combat.currentBattle->getAllEffectsOnXThatAreThisType(victim, "BOON");
							if (combat.currentBattle->doesTargetXHaveStatusY(victim, "Charm Collapse")) {
								if (effectsOnVictim.empty()) { continue; }
								string toRemove = effectsOnVictim.front()->e.uniqueID;
								combat.currentBattle->removeAnEffect(victim, toRemove);
							}
						}
						if (report.logic.find("RESURRECT_") != -1) {
							int resPower = 1;
							string type = split(report.logic, "_").at(2);
							for (auto v : report.vData.getKeys().internalList) {
								if (v.find("RESURRECT_SINGLE_") != -1) {
									resPower = report.vData[v];
									break;
								}
							}
							float asPercentage = 100 / resPower;
							for (auto currentTarget : report.combatantsAffected.internalList) {
								int lifeGain = (combat.currentBattle->getThisCombatant(currentTarget)->c.combatStats["LIFE"]/100) * asPercentage;
								int manaGain = (combat.currentBattle->getThisCombatant(currentTarget)->c.combatStats["ENERGY"]/100) * asPercentage;
								combat.currentBattle->getThisCombatant(currentTarget)->c.beHealed(lifeGain);
								combat.currentBattle->getThisCombatant(currentTarget)->c.combatStats["CURRENTENERGY"] = 0;
								combat.currentBattle->getThisCombatant(currentTarget)->c.gainEnergy(manaGain);
								combat.currentBattle->addCombatMessage("RESURRECT", List<pair<string, string>>({
										pair<string, string>("name",combat.currentBattle->getThisCombatant(user)->c.uniqueID),
										pair<string, string>("language",language),
										pair<string, string>("type",type),
										pair<string, string>("damage",to_string(resPower)),
										pair<string, string>("target",combat.currentBattle->all[currentTarget]->c.uniqueID),
									}), 0);
								toPrint.push_back(Result(currentTarget, to_string(resPower), "HEALINGGREEN", graphics.accessImageViaUniqueID(currentTarget)->positionAsPercentage));
								toPrint.push_back(Result(currentTarget, to_string(resPower), "HEALINGBLUE", graphics.accessImageViaUniqueID(currentTarget)->positionAsPercentage));
							}
						}
						if (report.logic.find("REMOVEBOON") != -1) {
							string whichBoon = report.sData["removeThisBoon"];
							string who = report.combatantsAffected.front();
							combat.currentBattle->removeAnEffect(who, whichBoon);
						}
						if (report.logic.find("REMOVEBANE") != -1) {
							for (auto who : report.combatantsAffected.internalList) {
								int howMany = report.vData["howManyBanes"];
								for (int x = 0; x < 1; x++) {
									if (report.sourceName == "Iridescent Breath" and combat.currentBattle->getAllEffectsOnXThatAreThisType(who, "BOON").empty()) { continue; }
									List<EffectObjectInstance*> banes = combat.currentBattle->getAllEffectsOnXThatAreThisType(who, "BANE");
									if (banes.empty()) { break; }
									combat.currentBattle->removeAnEffect(who, banes.back()->e.uniqueID);
								}
							}
						}
						if (report.logic.find("REMOVEXBANES") != -1) {
							for (auto who : report.combatantsAffected.internalList) {
								int howMany = report.vData["howManyBanes"];
								for (int x = 0; x < howMany; x++) {
									List<EffectObjectInstance*> banes = combat.currentBattle->getAllEffectsOnXThatAreThisType(who, "BANE");
									if (banes.empty()) { break; }
									combat.currentBattle->removeAnEffect(who, banes.back()->e.uniqueID);
								}
							}
						}
						if (report.logic.find("STEALBOON_SINGLE") != -1) {
							if (combat.currentBattle->getAllEffectsOnXThatAreThisType(report.combatantsAffected.front(), "BOON").empty()) {
								return;
							}
							string nameOfEffectToSteal = combat.currentBattle->getAllEffectsOnXThatAreThisType(report.combatantsAffected.front(), "BOON").front()->e.uniqueID;
							combat.currentBattle->transferAnEffect(report.combatantsAffected.front(), report.originalUser, nameOfEffectToSteal);
						}
}
					if (report.sData["success"] != "1") {
						Map<string, string> failData;
						failData["name"] = combat.currentBattle->getThisCombatant(user)->c.uniqueID;
						failData["language"] = language;
						failData["skill"] = report.sourceName;
						failData["failReason"] = report.sData["failReason"];
						bool failSilently = report.sData["failSilently"] == "1";
						if (!failedSkillExecutions.contains(report.sourceName) and !failSilently) {
							combat.currentBattle->addCombatMessage("FAILEDSKILL", failData, 0);
						}
						if (report.sourceName == "Smuggler's Gambit") {
							combat.currentBattle->getThisCombatant(report.originalUser)->c.getSkillByName("Smuggler's Gambit").timeToRecharge = 3;
						}
						failedSkillExecutions.push_back(report.sourceName);
						report.sourceName = "FAILEDSKILL";
					}
				}
			}

			CombatEvent getCurrentForAnimation() {
				if (counter >= ongoingReport.size()) {
					return ongoingReport.back();
				}
				return ongoingReport.at(counter);
			}

			void animationTick() {
				if (isAnimationFinished()) { return; }
				counter++;
				if (counter == 0 or counter >= ongoingReport.size()) {
					return;
				}
				string previousAnimation = ongoingReport.at(counter - 1).getTemporaryID();
				string currentAnimation = getCurrentForAnimation().getTemporaryID();
				while (previousAnimation == currentAnimation and counter < ongoingReport.size()) {
					counter++;
					currentAnimation = getCurrentForAnimation().getTemporaryID();
					previousAnimation = ongoingReport.at(counter - 1).getTemporaryID();
				}

			}
			bool isAnimationFinished() {
				return counter >= ongoingReport.size();
			}


			List<Result> toPrint;
			List<Combat::EffectObjectInstance*> allEffectsInOrder; // oldest to newest, so newest effect has the final say (LIFO)
			List<Combat::EffectObjectInstance*> effectsThatWereTriggered;
			List<CombatEvent> ongoingReport; // what each effect did and why in a row
			int counter = 0;
		};
		~Battle() {
		}
		Battle() {}
		Battle(Combat& combat, List<Combatant> _party1, List<Combatant> _party1allies, List<Combatant> _party2, List<Combatant> _party2allies, Map<string, string> _data) {
			for (auto c : _party1.internalList) {
				party1.push_back(new CombatantInstance(c, "TEAM1"));
				all[party1.back()->c.uniqueCombatID] = party1.back();
			}
			for (auto c : _party1allies.internalList) {
				party1allies.push_back(new CombatantInstance(c, "TEAM1_ALLIES"));
				all[party1allies.back()->c.uniqueCombatID] = party1allies.back();
			}
			for (auto c : _party2.internalList) {
				party2.push_back(new CombatantInstance(c, "TEAM2"));
				all[party2.back()->c.uniqueCombatID] = party2.back();
			}
			for (auto c : _party2allies.internalList) {
				party2allies.push_back(new CombatantInstance(c, "TEAM2_ALLIES"));
				all[party2allies.back()->c.uniqueCombatID] = party2allies.back();
			}
			data = _data;
			if (data.hasKey("direction")) {
				playerDirectionBeforeBattle = data["direction"];
			}
			if (data.hasKey("x")) {
				playerPositionBeforeBattle = { stof(data["x"]), stof(data["y"]) };
			}
			List<string> order = decideTurnOrder();
			currentRound = Round(1, order); // add stuff here if any equipment starts an effect immediately
			for (CombatantInstance* actor : all.getValues().internalList) {
				loadAnyPreExistingEffectsForThisCombatant(*&combat, actor);
			}
			addCombatMessage("COMBATSTART", data, 0);
			for (auto key : _data.getKeys().internalList) {
				if (key.find("LOOT") != -1) {
					string item = SReplace(key, "LOOT$", "");
					string quantity = _data[key];
					rewardForWinning[item] = stoi(quantity);
				}
			}
		}
		void endBattle() {
			for (CombatantInstance* c : all.getValues().internalList) {
				delete c;
				c = NULL;
			}
			for (auto who : allEffectsInPlay.getKeys().internalList) {
				for (auto name : allEffectsInPlay[who].getKeys().internalList) {
					delete allEffectsInPlay[who][name];
					allEffectsInPlay[who][name] = NULL;
				}
			}
		}
		List<Graphics::Image*> getAllImagesAssociatedWithThisBattle(Combat& combat) {
			List<Graphics::Image*> results;
			results.push_back(graphics.accessImageViaUniqueID("BattleBackground"));
			for (CombatantInstance* actor : getAllCombatants().internalList) {
				string uniqueIDBase = actor->c.uniqueCombatID;
				string barback_life = uniqueIDBase + "_BARBACKLIFE";
				string barback_mana = uniqueIDBase + "_BARBACKMANA";
				string life = uniqueIDBase + "_LIFE";
				string mana = uniqueIDBase + "_MANA";
				for (auto imageName : { uniqueIDBase , barback_life, barback_mana, life, mana }) {
					results.push_back(graphics.accessImageViaUniqueID(imageName));
				}
				List <Combat::EffectObjectInstance*> effectObjectInstances = combat.currentBattle->getAllEffectsOnXInTimeOrderOldestFirst(uniqueIDBase);
				for (Combat::EffectObjectInstance* effect : effectObjectInstances.internalList) {
					string imageName = uniqueIDBase + "$" + effect->e.uniqueID + "$COMBATEFFECT";
					string borderName = uniqueIDBase + "$" + effect->e.uniqueID + "$COMBATBORDER";
					if (graphics.accessImageViaUniqueID(imageName) != NULL) {
						results.push_back(graphics.accessImageViaUniqueID(imageName));
						results.push_back(graphics.accessImageViaUniqueID(borderName));
					}
				}
			}
			return results;
		}
		void loadAnyPreExistingEffectsForThisCombatant(Combat& combat, CombatantInstance* actor) {
			// combatant is sourced from internal combatant list
			if (actor->c.data.hasKey("Effects")) {
				for (auto effect : actor->c.data["Effects"].getKeys().internalList) {
					applyAnEffect(*&combat, effect, combat.allEffectDefinitions[effect], actor->c.uniqueCombatID, actor->c.uniqueCombatID, List<pair<string, int>>({
							pair<string, int>("duration",stoi(actor->c.data["Effects"][effect])),
						}));
				}
			}
			// combatant is sourced from party
			Map<string, Map<string, string>> equipment = saveContainer.getAllEquipment();
			if (equipment.hasKey(actor->c.uniqueID)) {
				for (auto category : equipment[actor->c.uniqueID].getKeys().internalList) {
					string itemName = equipment[actor->c.uniqueID][category];
					if (combat.equipmentDefinitions.hasKey(itemName)) {
						for (Effect effect : combat.equipmentDefinitions[itemName].powers.internalList) {
							if (combat.allEffectDefinitions.hasKey(effect.tag)) {
								applyAnEffect(*&combat, effect.tag, combat.allEffectDefinitions[effect.tag], actor->c.uniqueCombatID, actor->c.uniqueCombatID, List<pair<string, int>>({
							pair<string, int>("duration",999),
							pair<string, int>("power",effect.getPowerForCombat(*&combat)),
									}));
							}
						}
					}
				}
			}

		}
		void addNewCombatantDuringBattle(Combat& combat, CombatantInstance* actor) {
			all[actor->c.uniqueCombatID] = actor;
			if (actor->team == "TEAM1_ALLIES") {
				party1allies.push_back(actor);
			}
			else {
				party2allies.push_back(actor);
			}
			loadAnyPreExistingEffectsForThisCombatant(*&combat, actor);
		}
		List<string> decideTurnOrder() {
			/*
			Turn order Logic:
			Ranked by highest Speed, if tied, highest strength, if tied, random
			*/
			List<string> result;
			Map<int, Map<int, List<string>>> sorted;
			for (CombatantInstance* c : all.getValues().internalList) {
				int speed = c->c.combatStats["SPEED"];
				int strength = c->c.combatStats["STRENGTH"];
				sorted[speed][strength].push_back(c->c.uniqueCombatID);
			};
			List<int> sortedSpeedKeys = sorted.getKeys();
			sortedSpeedKeys.internalList.sort();
			for (auto speed : sortedSpeedKeys.internalList) {
				List<int> sortedStrengthKeys = sorted[speed].getKeys();
				sortedStrengthKeys.internalList.sort();
				for (auto strength : sortedStrengthKeys.internalList) {
					if (sorted[speed][strength].size() == 1) {
						result.push_back(sorted[speed][strength].front());
					}
					else {
						List<string> randomized = scrambleList(sorted[speed][strength]);
						for (auto r : randomized.internalList) {
							result.push_back(r);
						}
					}
				}
			}
			result.internalList.reverse();
			return result;
		}
		string tick(Combat & combat) {
			currentEventStackObject = EventStackObject();
			int whichRound = currentRound.roundNumber;
			int currentAct = currentRound.tick();
			if (currentAct == currentRound.actorOrder.size()) {
				currentRound = Round(whichRound + 1, decideTurnOrder());
			}
			string who = currentRound.whoseTurnIsIt();
			tickDownTheirEffects(who);
			if (all.getKeys().contains(who)) {
				all[who]->c.tickDownIfCasting();
				all[who]->c.tickDownRechargingSkills();
				all[who]->c.passiveManaRegen();
			}
			return battleStatusCheck();
		}
		void tickDownTheirEffects(string who) {
			List<EffectObjectInstance*> toRemove;
			for (EffectObjectInstance* effect : getAllEffectsOnXInTimeOrderOldestFirst(who).internalList) {
				effect->e.tick();
				if (effect->e.expired()) {
					toRemove.push_back(effect);
				}
			}
			for (EffectObjectInstance * effect : toRemove.internalList) {
				removeAnEffect(who, effect->e.uniqueID);
			}
		}
		string battleStatusCheck() {
			bool battleWon = isBattleWon();
			bool battleLost = isBattleLost();

			if (battleLost) { return "PLAYERLOSE"; }
			if (battleWon) { return "PLAYERWIN"; }
			return "INPROGRESS";
		}
		bool isBattleWon() {
			if (winCondition == "STANDARD") {
				bool allDead = true;
				for (auto c : party2.internalList) {
					if (!c->c.isDead()) { allDead = false; }
				}
				for (auto c : party2allies.internalList) {
					if (!c->c.isDead()) { allDead = false; }
				}
				return allDead;
			}
		}
		bool isBattleLost() {
			if (loseCondition == "STANDARD") {
				bool allDead = true;
				for (auto c : party1.internalList) {
					if (!c->c.isDead()) { allDead = false; }
				}
				return allDead;
			}
		}
		void playerQuitBattle() {
			for (auto c : party1.internalList) {
				c->c.combatStats["CURRENTLIFE"] = 0;
			}
		}
		bool areTherePreTurnEffectsToRun() {
			return currentEventStackObject.ongoingReport.size() > 0;
		}
		void addCombatMessage(string type, Map<string, string> combatMessageData, int verbosity) {
			// for top bar
			string language = combatMessageData["language"];
			wstring message = L"";
			if (type == "COMBATSTART") {
				int team1Size = party1.size();
				int team2Size = party2.size();
				if (team1Size == 1 and team2Size == 1) {
					message = strings[language]["Combat Messages"]["BATTLESTART1VS1"];
				}
				if (team1Size > 1 and team2Size == 1) {
					message = strings[language]["Combat Messages"]["BATTLESTARTTEAMVS1"];
				}
				if (team1Size == 1 and team2Size > 1) {
					message = strings[language]["Combat Messages"]["BATTLESTART1VSTEAM"];
				}
				if (team1Size > 1 and team2Size > 1) {
					message = strings[language]["Combat Messages"]["BATTLESTARTTEAMVSTEAM"];
				}
				message = WSReplace(message, L"$PLAYER$", StringToWString(saveContainer.getCurrentMainCharacter()));
				message = WSReplace(message, L"$OPPONENT$", strings[language]["NPCNames"][combatMessageData["opponentName"]]);
			}
			if (type == "PLAYERTURN") {
				message = strings[language]["Combat Messages"]["PLAYERTURN"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][combatMessageData["name"]]);
			}
			if (type == "AITURN") {
				message = strings[language]["Combat Messages"]["AITURN"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][combatMessageData["name"]]);
			}
			if (type == "STARTCASTING") {
				message = strings[language]["Skill Actions"][combatMessageData["skillType"] + "_STARTED"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$SKILL$", strings[language]["Skill Names"][combatMessageData["skill"]]);
			}
			if (type == "STILLCASTING") {
				message = strings[language]["Skill Actions"][combatMessageData["skillType"] + "_INPROGRESS"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$SKILL$", strings[language]["Skill Names"][combatMessageData["skill"]]);
			}
			if (type == "DIRECT") {
				message = StringToWString(combatMessageData["message"]);
			}
			if (type == "DAMAGE") {
				message = strings[language]["Combat Messages"]["DAMAGE"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(combatMessageData["damage"]));
				message = WSReplace(message, L"$TYPE$", strings[language]["Type Names"][combatMessageData["type"]]);
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][combatMessageData["target"]]);
			}
			if (type == "LIFESTEAL") {
				message = strings[language]["Combat Messages"]["LIFESTEAL"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(combatMessageData["damage"]));
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][combatMessageData["target"]]);
			}
			if (type == "LIFEHEAL") {
				if (combatMessageData["name"] == combatMessageData["target"]) {
					message = strings[language]["Combat Messages"]["LIFEHEAL_SELF"];
				}
				else {
					message = strings[language]["Combat Messages"]["LIFEHEAL_OTHER"];
				}
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(combatMessageData["damage"]));
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][combatMessageData["target"]]);
			}
			if (type == "MANAHEAL") {
				if (combatMessageData["name"] == combatMessageData["target"]) {
					message = strings[language]["Combat Messages"]["MANAHEAL_SELF"];
				}
				else {
					message = strings[language]["Combat Messages"]["MANAHEAL_OTHER"];
				}
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(combatMessageData["damage"]));
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][combatMessageData["target"]]);
			}
			if (type == "MANABURN") {
				message = strings[language]["Combat Messages"]["MANABURN"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(combatMessageData["damage"]));
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][combatMessageData["target"]]);
			}
			if (type == "ENERGYSTEAL") {
				message = strings[language]["Combat Messages"]["ENERGYSTEAL"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(combatMessageData["damage"]));
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][combatMessageData["target"]]);
			}
			if (type == "WAIT") {
				message = strings[language]["Combat Messages"]["WAIT"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][combatMessageData["name"]]);
			}
			if (type == "FAILEDSUMMONNOSPACE") {
				message = strings[language]["Skill Actions"]["FAILEDSUMMONNOSPACE"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$WHAT$", strings[language]["NPCNames"][combatMessageData["summon"]]);
			}
			if (type == "VICTORY") {
				message = strings[language]["Combat Messages"]["VICTORY"];
				string leader = saveContainer.getCurrentMainCharacter();

				if (!rewardForWinning.getKeys().empty()) {
					message += strings[language]["GUI"]["BATTLEREWARD"];
					for (auto item : rewardForWinning.getKeys().internalList) {
						wstring itemName = L"";
						if (item == "GOLD" and rewardForWinning[item] == 1) {
							itemName = strings[language]["Unique Item Strings"]["GOLD_1"];
						}
						if (item == "GOLD" and rewardForWinning[item] > 1) {
							itemName = strings[language]["Unique Item Strings"]["GOLD_X"];
						}
						if (item != "GOLD") {
							itemName = strings[language]["Item Names"][item];
						}
						message += to_wstring(rewardForWinning[item]) + L" " + itemName + L"\n";
					}
				}
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][leader]) + L"\n";
			}
			if (type == "DEFEAT") {
				string leader = saveContainer.getCurrentMainCharacter();
				message = strings[language]["Combat Messages"]["DEFEAT"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][leader]) + L"\n";
			}
			if (type == "FAILED_NO_TARGETS") {
				message = strings[language]["Combat Messages"]["FAILED_NO_TARGETS"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$SKILL$", strings[language]["Skill Names"][combatMessageData["skill"]]);
				message = WSReplace(message, L"$TARGET$", strings[language]["NPCNames"][combatMessageData["target"]]);
			}
			if (type == "FAILEDSKILL") {
				message = strings[language]["Combat Messages"]["SKILLFAIL"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$SKILL$", strings[language]["Skill Names"][combatMessageData["skill"]]);
				message = WSReplace(message, L"$REASON$", strings[language]["Combat Messages"]["DUETO" + combatMessageData["failReason"]]);
			}
			if (type == "INTERRUPTED") {
				message = strings[language]["Combat Messages"]["INTERRUPTED"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$SKILL$", strings[language]["Skill Names"][combatMessageData["skill"]]);
				message = WSReplace(message, L"$INTERRUPTER$", strings[language]["NPCNames"][combatMessageData["interrupter"]]);
				message = WSReplace(message, L"$INTERRUPTION$", strings[language]["Skill Names"][combatMessageData["interruption"]]);
			}
			if (type == "RESURRECT") {
				message = strings[language]["Combat Messages"]["RESURRECT"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(combatMessageData["damage"]));
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][combatMessageData["target"]]);
			}
			if (type == "DISEASE") {
				message = strings[language]["Combat Messages"]["DISEASE"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][combatMessageData["name"]]);
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][combatMessageData["target"]]);
			}
			combatMessages.push_front({ verbosity, message });
		}
		void announceCombatantTurn(string language) {
			string who = currentRound.whoseTurnIsIt();
			if (who == "WORLD") { return; }
			Map<string, string> data;
			data["language"] = language;
			data["name"] = all[who]->c.uniqueID;
			addCombatMessage("PLAYERTURN", data, 0);
			if (!playerIsInControl() and all[who]->c.currentlyCasting) {
				string skillType = "PHYSICAL";
				string skillName = all[who]->c.getSkillBeingCast().uniqueID;
				if (all[who]->c.getSkillBeingCast().skillTypeTags.contains("MAGICAL")) {
					skillType = "MAGICAL";
				}
				data["skillType"] = "MAGICAL";
				data["skill"] = skillName;
				addCombatMessage("STILLCASTING", data, 0);
			}
		}
		bool playerIsInControl() {
			string who = currentRound.whoseTurnIsIt();
			if (who == "WORLD") { return false; }
			string team = all[who]->team;
			bool controllableTeam = team == "TEAM1";
			bool casting = all[who]->c.currentlyCasting;
			bool thereAreEffectsToRun = areTherePreTurnEffectsToRun();
			bool isThisActorDead = all[who]->c.isDead();

			return controllableTeam and not casting and not thereAreEffectsToRun and not isThisActorDead;
		}
		bool doesThisCombatantExist(string name) {
			return all.getKeys().contains(name);
		}
		CombatantInstance* getThisCombatant(string name) {
			if (!all.hasKey(name)) {
				throw exception("No one with that name.");
			}
			return all[name];
		}
		CombatantInstance* getCurrentCombatant() {
			string currentActor = currentRound.whoseTurnIsIt();
			if (currentActor == "WORLD") {
				return NULL;
			}
			return getThisCombatant(currentActor);
		}
		List<CombatantInstance*> getAllCombatants() {
			return all.getValues();
		}
		List<CombatantInstance*> getAllValidTargetsForThisSkill(Combat& combat, CombatantInstance* combatant, Skill skillDefinition) {
			List<CombatantInstance*> results;
			string targetLogic = skillDefinition.targetLogic;
			if (combat.currentBattle != NULL) {
				if (combat.currentBattle->doesTargetXHaveStatusY(combatant->c.uniqueCombatID, "Stalked by Shadows")) {
					if (skillDefinition.skillTypeTags.contains("MAGICAL")) {
						return results; // can't cast any spells
					}
				}
			}
			if (targetLogic == "FOECASTINGASPELL") {
				results = combatant->getMyFoesThatAreCastingASpell(*&combat);
			}
			if (targetLogic == "SINGLEFOE") {
				results = combatant->getMyFoesThatAreAlive(*&combat);
			}
			if (targetLogic == "DEADFOE") {
				results = combatant->getMyDeadFoes(*&combat);
			}
			if (targetLogic == "SINGLEALLY") {
				results = combatant->getMyAlliesThatAreAlive(*&combat);
			}
			if (targetLogic == "SINGLEOTHERALLY") {
				results = combatant->getAllMyOtherAlliesNotMe(*&combat);
			}
			if (targetLogic == "SINGLEOTHERALLYWITHNOBOONS") {
				for (CombatantInstance* actor : combatant->getAllMyOtherAlliesNotMe(*&combat).internalList) {
					if (combat.currentBattle->getAllEffectsOnXThatAreThisType(actor->c.uniqueCombatID, "BOON").empty()) {
						results.push_back(actor);
					}
				}
			}
			if (targetLogic == "DEADPARTYMEMBER") {
				results = combatant->getMyDeadPartyMembers(*&combat);
			}
			if (targetLogic == "SINGLESUMMONEDFOE") {
				results = combatant->getMyFoesThatWereSummoned(*&combat);
			}
			if (targetLogic == "SELFIF<90LIFE") {
				if (combatant->c.getLifeAsPercentage() < 0.9) {
					results.push_back(combatant);
				}
			}
			if (List<string>({ "SELF", "ALL", "ALLALLIES", "ALLFOES", "ALLUNDEADORDEMONICFOES"}).contains(targetLogic)) {
				results.push_back(combatant); // use on "self" but the effect hits the right place
			}
		

			List<string> couldTargetASingleAlly = List<string>({"SINGLEALLY", "SINGLEOTHERALLY", "SINGLEOTHERALLYWITHNOBOONS"});

			// effects which make allies untargetable
			if (couldTargetASingleAlly.contains(targetLogic)) {
				for (CombatantInstance* actor : results.internalList) {
					if (combat.currentBattle->doesTargetXHaveStatusY(actor->c.uniqueCombatID, "I Shall Take Care of This!")) {
						results.forcibleRemove(actor);
					}
				}
			}

			return results;
		}
		List<CombatantInstance*> whoWillBeAffectedByThisSkill(Combat& combat, CombatantInstance* combatant, Skill skillDefinition) {
			List<CombatantInstance*> result;
			string targetLogic = skillDefinition.targetLogic;
			if (List<string>({ "SINGLEFOE","SINGLEALLY","SINGLEOTHERALLY", "SELF", "FOECASTINGASPELL"}).contains(targetLogic)) {
				return getAllValidTargetsForThisSkill(*&combat, combatant, skillDefinition);
			}
			if (targetLogic == "ALL") {
				return all.getValues();
			}
			if (targetLogic == "ALLALLIES") {
				return combatant->getAllMyAllies(*&combat);
			}
			if (targetLogic == "ALLFOES") {
				return combatant->getMyFoes(*&combat);
			}

			return result;
		}
		List<CombatantInstance*> whoIsNotInThisList(List<CombatantInstance*> rhs) {
			List<CombatantInstance*> result = all.getValues();
			for (auto c : rhs.internalList) {
				if (result.contains(c)) {
					result.forcibleRemove(c);
				}
			}
			return result;
		}
		bool isThisDamageElemental(string input) {
			List<string> elements = List<string>({"FIRE","ELECTRIC","COLD","EARTH"});
			for (auto element : elements.internalList) {
				if (input.find(element) != -1) { return true; }
			}
			return false;
		}
		bool isThisDamagePhysical(string input) {
			return input.find("PHYSICAL") != -1;
		}

		List<EffectObjectInstance*> getAllEffectsInTimeOrderOldestFirst() {
			List<EffectObjectInstance*> results;
			for (auto who : allEffectsInPlay.getKeys().internalList) {
				for (EffectObjectInstance* effect : getAllEffectsOnXInTimeOrderOldestFirst(who).internalList) {
					results.push_back(effect);
				}
			}
			results.internalList.sort();
			return results;
		}
		List<EffectObjectInstance*> getAllEffectsOnXInTimeOrderOldestFirst(string who) {
			List<EffectObjectInstance*> allEffectsOnX;
			List<EffectObjectInstance*> allEffectsInTimeOrder;
			List<int> timesInOrder;
			for (auto name : allEffectsInPlay[who].getKeys().internalList) {
				allEffectsOnX.push_back(allEffectsInPlay[who][name]);
				timesInOrder.push_back(allEffectsInPlay[who][name]->e.timeCreated);
			}
			timesInOrder.internalList.sort();
			for (auto time : timesInOrder.internalList) {
				for (EffectObjectInstance* effect : allEffectsOnX.internalList) {
					if (effect->e.timeCreated == time and !allEffectsInTimeOrder.contains(effect)) {
						allEffectsInTimeOrder.push_back(effect);
					}
				}
			}
			return allEffectsInTimeOrder;
		}
		List<EffectObjectInstance*> getAllEffectsOnXThatAreThisType(string who, string type) {
			List<EffectObjectInstance*> preliminary = getAllEffectsOnXInTimeOrderOldestFirst(who);
			List< EffectObjectInstance*> results;
			for (EffectObjectInstance* e : preliminary.internalList) {
				if (e->e.type == type) {
					results.push_back(e);
				}
			}
			return results;
		}
		List<string> getNamesOfEffectsOnX(string who) {
			List<EffectObjectInstance*> effectsAlreadyOnX = getAllEffectsOnXInTimeOrderOldestFirst(who);
			List<string> effectsOnXByName;
			for (EffectObjectInstance* effect : effectsAlreadyOnX.internalList) {
				effectsOnXByName.push_back(effect->e.uniqueID);
			}
			return effectsOnXByName;
		}

		bool doesTargetXHaveStatusY(string who, string effectName) {
			List<EffectObjectInstance*> effects = getAllEffectsOnXInTimeOrderOldestFirst(who);
			for (EffectObjectInstance* effect : effects.internalList) {
				if (effect->e.uniqueID == effectName) {
					return true;
				}
			}
			return false;
		}
		void determineCurrentSkillEffectStack(Combat& combat, string language) {
			// what will the execution of this skill trigger?
			List<EffectObjectInstance*> allEffectsInTimeOrder;
			for (auto who : allEffectsInPlay.getKeys().internalList) {
				for (auto name : allEffectsInPlay[who].getKeys().internalList) {
					allEffectsInTimeOrder.push_back(allEffectsInPlay[who][name]);
				}
			}
			allEffectsInTimeOrder.internalList.sort();
			CombatantInstance* actor = getCurrentCombatant();
			CombatantInstance* target = NULL;
			Skill skill;
			if (actor != NULL) {
				skill = actor->c.getSkillBeingCast();
				CombatantInstance* target = all[actor->c.currentTarget];
				actor->changeTargetOrCancelCastingIfCurrentTargetIsNowInapplicable(*&combat, language);
				if (actor->c.currentlyCasting) {
					currentEventStackObject = EventStackObject(actor->startSkillExecution(*&combat, language));
				}
				else {
					currentEventStackObject = EventStackObject();
				}
			}
			else {
				currentEventStackObject = EventStackObject();
			}
			currentEventStackObject.executeTheStack(*&combat);
			currentEventStackObject.executeTheResults(*&combat, language);
			actor->c.finishCasting(*&combat);
		}
		void determineCurrentSkillEffectStackPassive(Combat& combat, string language) {
			// if a skill effect chain is triggered by an effect starting, ticking, or ending, not the use of a skill
			currentEventStackObject = EventStackObject();
			string nextActor = currentRound.whoseTurnIsNext();
			for (EffectObjectInstance* effect : allEffectsInPlay[nextActor].getValues().internalList) {
				if (effect->e.triggers.contains("EVERYTURN")) {
					currentEventStackObject.ongoingReport.push_back(CombatEvent(*&combat, effect));
				}
			
				if (effect->e.roundsLeft == 0) {
					if (effect->e.triggers.contains("ONEND")) {
						currentEventStackObject.ongoingReport.push_back(CombatEvent(*&combat, effect));
					}
				}
			}
			currentEventStackObject.executeTheStack(*&combat);
			currentEventStackObject.executeTheResults(*&combat, language);
		}

		void applyAnEffect(Combat& combat, string effectName, EffectObject effect, string source, string target, Map<string, int> values) {
			if (allEffectsInPlay[target][effectName] != NULL) {
				delete allEffectsInPlay[target][effectName];
				allEffectsInPlay[target][effectName] = NULL;
				allEffectsInPlay[target].internalMap.erase(effectName);
			}
			allEffectsInPlay[target][effectName] = new EffectObjectInstance(*&combat, effectName, source, target, values);
			allEffectsInPlay[target][effectName]->e.timeCreated = addToEffectClock();

		}
		void removeAnEffect(string who, string name) {
			if (!allEffectsInPlay.hasKey(who)) { return; }
			if (!allEffectsInPlay[who].hasKey(name)) { return; }
			if (allEffectsInPlay[who][name] != NULL) {
				delete allEffectsInPlay[who][name];
				allEffectsInPlay[who][name] = NULL;
				allEffectsInPlay[who].internalMap.erase(name);
				string imageName = who + "$" + name + "$COMBATEFFECT";
				string borderName = who + "$" + name + "$COMBATBORDER";
				graphics.tearDownSpecifiedImage(imageName);
				graphics.tearDownSpecifiedImage(borderName);
			}
		}
		void transferAnEffect(string from, string to, string name) {
			if (doesTargetXHaveStatusY(to, name)) {
				removeAnEffect(to, name);
			}
			allEffectsInPlay[to][name] = allEffectsInPlay[from][name];
			allEffectsInPlay[from].internalMap.erase(name);
			allEffectsInPlay[to][name]->e.target = to;
			string imageName = from + "$" + name + "$COMBATEFFECT";
			string borderName = from + "$" + name + "$COMBATBORDER";
			graphics.tearDownSpecifiedImage(imageName);
			graphics.tearDownSpecifiedImage(borderName);
		}
		EffectObjectInstance* getThisEffect(string who, string name) {
			if (!allEffectsInPlay.hasKey(who)) {
				throw exception("No one with that name.");
			}
			if (!allEffectsInPlay[who].hasKey(name)) {
				throw exception("This combatant does not have this effect.");
			}
			return allEffectsInPlay[who][name];
		}
		int addToEffectClock() {
			effectClock.internalList.sort();
			if (effectClock.empty()) {
				effectClock.push_back(0);
			}
			int result = effectClock.back() + 1;
			effectClock.push_back(result);
			return result;
		}

		List<CombatantInstance*> party1;
		List<CombatantInstance*> party1allies;
		List<CombatantInstance*> party2allies;
		List<CombatantInstance*> party2;
		Map<string, string> data;
		List<int> effectClock; // used to know when an effect was made
		Round currentRound;
		string winCondition = "STANDARD";
		string loseCondition = "STANDARD";
		List<pair<int, wstring>> combatMessages; // verbosityLevel -> message. Higher level = less likely to show (more granular detail)
		List<pair<int, wstring>> messagesWaitingToBeAdded; // adding too many messages at once hides previous ones
		bool executingSomething = true; // set to true when player shouldn't be able to interact
		EventStackObject currentEventStackObject;
		Map<string, int> rewardForWinning;
		bool combatEnding = false;
		string playerDirectionBeforeBattle = "STAND_FRONT";
		pair<float, float> playerPositionBeforeBattle = { 50,50 };
	private:
		Map<string, CombatantInstance*> all;
		Map<string, Map<string, EffectObjectInstance*>> allEffectsInPlay; // who -> condition name -> condition object. who can include "TEAM1", "TEAM2", "WORLD"
	};

	void setUpBattle(List<Combatant> team1, List<Combatant> team1allies, List<Combatant> team2, List<Combatant> team2allies, Map<string, string> data) {
		currentBattle = new Battle(*this, team1, team1allies, team2, team2allies, data);
	};
	void tearDownBattle() {
		for (Graphics::Image* image : currentBattle->getAllImagesAssociatedWithThisBattle(*this).internalList) {
			graphics.tearDownSpecifiedImage(image->unique_ID);
		}
		currentBattle->endBattle();
		delete currentBattle;
		currentBattle = NULL;
	}

	Battle* currentBattle = NULL;

	Map<string, Map<string, pair<float, float>>> getCombatantPositionLookup() {
		Map<string, Map<string, pair<float, float>>> results;

		// positions as % of screen
		float party1Y = 60;
		float party1alliesY = 55;
		float party2alliesY = 35;
		float party2Y = 30;

		results["TEAM1_4"] = Map<string, pair<float, float>>({
				pair<string, pair<float, float>>("1", {12, party1Y}),
				pair<string, pair<float, float>>("2", {32, party1Y}),
				pair<string, pair<float, float>>("3", {52, party1Y}),
				pair<string, pair<float, float>>("4", {72, party1Y}),
			});
		results["TEAM1_3"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {17, party1Y}),
			pair<string, pair<float, float>>("2", {37, party1Y}),
			pair<string, pair<float, float>>("3", {57, party1Y}),
			});
		results["TEAM1_2"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {27, party1Y}),
			pair<string, pair<float, float>>("2", {47, party1Y}),
			});
		results["TEAM1_1"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {37, party1Y}),
			});
		results["TEAM1_ALLIES"] = Map<string, pair<float, float>>({
				pair<string, pair<float, float>>("1", {17, party1alliesY}),
				pair<string, pair<float, float>>("2", {37, party1alliesY}),
				pair<string, pair<float, float>>("3", {57, party1alliesY}),
				pair<string, pair<float, float>>("4", {77, party1alliesY}),
			});
		results["TEAM2_4"] = Map<string, pair<float, float>>({
				pair<string, pair<float, float>>("1", {27, party2Y}),
				pair<string, pair<float, float>>("2", {47, party2Y}),
				pair<string, pair<float, float>>("3", {67, party2Y}),
				pair<string, pair<float, float>>("4", {87, party2Y}),
			});
		results["TEAM2_3"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {37, party2Y}),
			pair<string, pair<float, float>>("2", {57, party2Y}),
			pair<string, pair<float, float>>("3", {77, party2Y}),
			});
		results["TEAM2_2"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {47, party2Y}),
			pair<string, pair<float, float>>("2", {67, party2Y}),
			});
		results["TEAM2_1"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {57, party2Y}),
			});
		results["TEAM2_ALLIES"] = Map<string, pair<float, float>>({
				pair<string, pair<float, float>>("1", {22, party2alliesY}),
				pair<string, pair<float, float>>("2", {42, party2alliesY}),
				pair<string, pair<float, float>>("3", {62, party2alliesY}),
				pair<string, pair<float, float>>("4", {82, party2alliesY}),
			});
		return results;

	}
	void defineAllSkills() {
		// DEFAULT
		skillDefinitions["DEFAULT_ATTACK"] = Skill("DEFAULT_ATTACK", "DEFAULT_ATTACK", "Default", SKILLICON_ATTACK, 0, 0, 0, "SINGLEFOE", list<string>({ "DAMAGE_SINGLE_PHYSICAL" }), list<string>({ "PHYSICAL","ATTACK" }), Map<string, PowerValue>({
			pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH"}))) }), list<string>({ "DEALDAMAGE","DEALDAMAGE_PHYSICAL" }), 5819);

		skillDefinitions["DEFAULT_WAIT"] = Skill("DEFAULT_WAIT", "DEFAULT_WAIT", "Default", SKILLICON_WAIT, 0, 0, 0, "SELF", {}, {}, {}, list<string>({ "WAIT" }), -1);

		// used by the animated mound of leeches
		skillDefinitions["DEFAULT_LEECHSKILL"] = Skill("DEFAULT_LEECHSKILL", "DEFAULT_LEECHSKILL", "Default", SKILLICON_WAIT, 0, 0, 0, "SINGLEFOE", {"STEALLIFEFORMASTER"}, list<string>({ "MAGICAL","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("STEALLIFEFORMASTER",PowerValue("STEALLIFEFORMASTER",7,0,999,true,list<string>({"INTELLIGENCE"}))) }), list<string>({"STEALLIFEFORMASTER"}), 1060);

		// debug
		skillDefinitions["Suicide"] = Skill("Suicide", "Suicide", "Debug", SKILLICON_WAIT, 0, 0, 0, "SELF", { "SUICIDE_SELF" }, list<string>({ "PHYSICAL","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("SUICIDE",PowerValue("STEALLIFEFORMASTER",0,0,0,true,list<string>({}))) }), list<string>({}), 5819);

		// CLEROMANCY
		skillDefinitions["Heal Wounds"] = Skill("Heal Wounds", "Heal Wounds", "Cleromancy", SKILLICON_HEALWOUNDS, 10, 1, 1, "SINGLEALLY",
			list<string>({ "LIFEHEAL_SINGLE_HOLY" }),
			list<string>({ "MAGICAL","HOLY", "HEAL","TARGETSALLIES" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("LIFEHEAL_SINGLE_HOLY", PowerValue("LIFEHEAL_SINGLE_HOLY", 60, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({ "HEALALLY", "HEALSELF" }), HEALWOUNDS_WAV);

		skillDefinitions["Laying of Hands"] = Skill("Laying of Hands", "Laying of Hands", "Cleromancy", SKILLICON_LAYINGOFHANDS, 10, 1, 1, "SINGLEOTHERALLY",
			list<string>({ "LIFEHEAL_SINGLE_HOLY" }),
			list<string>({ "MAGICAL","HOLY", "HEAL","TARGETSALLIES" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("LIFEHEAL_SINGLE_HOLY", PowerValue("LIFEHEAL_SINGLE_HOLY", 80, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({ "HEALALLY" }), HEALWOUNDS_WAV);

		skillDefinitions["Revitalise"] = Skill("Revitalise", "Revitalise", "Cleromancy", SKILLICON_REVITALISE, 25, 2, 5, "DEADPARTYMEMBER",
			list<string>({ "RESURRECT_SINGLE_HOLY" }),
			list<string>({ "MAGICAL","HOLY", "HEAL","TARGETSALLIES" }),
			Map<string, PowerValue>({ 
				pair<string, PowerValue>("RESURRECT_SINGLE_HOLY", PowerValue("RESURRECT_SINGLE_HOLY", 10, 0, 100, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"})))
				}),
			list<string>({ "RESURRECT" }), REVITALISE_WAV);

		skillDefinitions["Conciliatory Prayer"] = Skill("Conciliatory Prayer", "Conciliatory Prayer", "Cleromancy", SKILLICON_CONCILIATORYPRAYER, 15, 1, 4, "SINGLEOTHERALLY",
			list<string>({ "LIFEHEAL_SINGLE_HOLY" }),
			list<string>({ "MAGICAL","HOLY", "HEAL","TARGETSALLIES" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("LIFEHEAL_SINGLE_HOLY", PowerValue("LIFEHEAL_SINGLE_HOLY", 70, 0, 100, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({ "HEALALLY" }), CONCILIATORYPRAYER_WAV);

		skillDefinitions["Great Gospel"] = Skill("Great Gospel", "Great Gospel", "Cleromancy", SKILLICON_GREATGOSPEL, 30, 0, 5, "SELF",
			list<string>({ "APPLY_Great Gospel_ALLALLIES" }),
			list<string>({ "MAGICAL","HOLY","HEAL", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Great Gospel", PowerValue("DURATION_Great Gospel", 8, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Great Gospel", PowerValue("POWER_Great Gospel", 20, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "HEALSELF", "HEALALLY",}), GREATGOSPEL_WAV);

		skillDefinitions["Thoughtful Prayer"] = Skill("Thoughtful Prayer", "Thoughtful Prayer", "Cleromancy", SKILLICON_THOUGHTFULPRAYER, 20, 1, 3, "SINGLEALLY",
			list<string>({ "LIFEHEAL_SINGLE_HOLY", "REMOVEBANE_SINGLE_HOLY"}),
			list<string>({ "MAGICAL","HOLY", "HEAL","TARGETSALLIES" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("LIFEHEAL_SINGLE_HOLY", PowerValue("LIFEHEAL_SINGLE_HOLY", 60, 0, 100, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({ "HEALALLY", "HEALSELF"}), THOUGHTFULPRAYER_WAV);

		skillDefinitions["Apostle of Patience"] = Skill("Apostle of Patience", "Apostle of Patience", "Cleromancy", SKILLICON_APOSTLEOFPATIENCE, 20, 0, 2, "SINGLEALLY",
			list<string>({ "APPLY_Apostle of Patience_SINGLE" }),
			list<string>({ "MAGICAL","HOLY", "HEAL","TARGETSALLIES" }),
			Map<string, PowerValue>({ 
				pair<string, PowerValue>("DURATION_Apostle of Patience", PowerValue("DURATION_Apostle of Patience", 2, 2, 2, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Apostle of Patience", PowerValue("POWER_Apostle of Patience", 150, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
			list<string>({ "HEALALLY", "HEALSELF" }), THOUGHTFULPRAYER_WAV);

		skillDefinitions["Shield of a Goddess"] = Skill("Shield of a Goddess", "Shield of a Goddess", "Cleromancy", SKILLICON_SHIELDOFAGODDESS, 15, 0, 2, "SINGLEALLY",
			list<string>({ "APPLY_Shield of a Goddess_SINGLE" }),
			list<string>({ "MAGICAL","HOLY", "HEAL","TARGETSALLIES" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Shield of a Goddess", PowerValue("DURATION_Shield of a Goddess", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Shield of a Goddess", PowerValue("POWER_Shield of a Goddess", 33, 33, 33, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF", "ENCHANTALLY" }), SHIELDOFAGODDESS_WAV);

		skillDefinitions["Ivory Sanctuary"] = Skill("Ivory Sanctuary", "Ivory Sanctuary", "Cleromancy", SKILLICON_IVORYSANCTUARY, 15, 0, 2, "SINGLEALLY",
			list<string>({ "APPLY_Ivory Sanctuary_SINGLE" }),
			list<string>({ "MAGICAL","HOLY", "HEAL","TARGETSALLIES" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Ivory Sanctuary", PowerValue("DURATION_Ivory Sanctuary", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Ivory Sanctuary", PowerValue("POWER_Ivory Sanctuary", 33, 33, 33, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF", "ENCHANTALLY" }), IVORYSANCTUARY_WAV);

		skillDefinitions["Papalcy"] = Skill("Papalcy", "Papalcy", "Cleromancy", SKILLICON_PAPALCY, 15, 0, 2, "SELF",
			list<string>({ "APPLY_Papalcy_SINGLE" }),
			list<string>({ "MAGICAL","HOLY", "HEAL"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Papalcy", PowerValue("DURATION_Papalcy", 15, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Papalcy", PowerValue("POWER_Papalcy", 12, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF",}), PAPALCY_WAV);

		skillDefinitions["Incessant Devotion"] = Skill("Incessant Devotion", "Incessant Devotion", "Cleromancy", SKILLICON_INCESSANTDEVOTION, 10, 0, 0, "SELF",
			list<string>({ "APPLY_Incessant Devotion_SINGLE" }),
			list<string>({ "MAGICAL","HOLY", "HEAL" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Incessant Devotion", PowerValue("DURATION_Incessant Devotion", 15, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Incessant Devotion", PowerValue("POWER_Incessant Devotion", 50, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF", }), PAPALCY_WAV);

		skillDefinitions["Ambrosia"] = Skill("Ambrosia", "Ambrosia", "Cleromancy", SKILLICON_AMBROSIA, 10, 0, 4, "SINGLEALLY",
			list<string>({ "REMOVEXBANES_SINGLE" }),
			list<string>({ "MAGICAL","HOLY", "HEAL" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER_Ambrosia", PowerValue("POWER_Ambrosia", 2, 0, 10, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("power", PowerValue("power", 2, 0, 10, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "CUREALLY", }), AMBROSIA_WAV);

		skillDefinitions["Remedy Ward"] = Skill("Remedy Ward", "Remedy Ward", "Cleromancy", SKILLICON_REMEDYWARD, 30, 0, 4, "SELF",
			list<string>({ "APPLY_Remedy Ward_ALLALLIES" }),
			list<string>({ "MAGICAL","HOLY", "HEAL" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Remedy Ward", PowerValue("DURATION_Remedy Ward", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "CUREALLY", }), REMEDYWARD_WAV);

		skillDefinitions["Angelic Observatory"] = Skill("Angelic Observatory", "Angelic Observatory", "Cleromancy", SKILLICON_ANGELICOBSERVATORY, 60, 1, 5, "SELF",
			list<string>({ "APPLY_Angelic Observatory_ALLALLIES" }),
			list<string>({ "MAGICAL","HOLY", "HEAL", "ELITE"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Angelic Observatory", PowerValue("DURATION_Angelic Observatory", 3, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF", }), ANGELICOBSERVATORY_WAV);

		skillDefinitions["Blessed Light"] = Skill("Blessed Light", "Blessed Light", "Cleromancy", SKILLICON_BLESSEDLIGHT, 30, 0, 3, "SINGLEALLY",
			list<string>({ "REMOVEBANE_SINGLE", "LIFEHEAL_SINGLE_HOLY",}),
			list<string>({ "MAGICAL","HOLY", "HEAL", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFEHEAL_SINGLE_HOLY", PowerValue("LIFEHEAL_SINGLE_HOLY", 123, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "HEALSELF", "HEALALLY", "CURE"}), BLESSEDLIGHT_WAV);

		skillDefinitions["Gift of Knowledge"] = Skill("Gift of Knowledge", "Gift of Knowledge", "Cleromancy", SKILLICON_GIFTOFKNOWLEDGE, 10, 0, 10, "SELF",
			list<string>({ "APPLY_Gift of Knowledge_SINGLE", }),
			list<string>({ "MAGICAL","HOLY", "HEAL", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Gift of Knowledge", PowerValue("DURATION_Gift of Knowledge", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Gift of Knowledge", PowerValue("POWER_Gift of Knowledge", 33, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF",}), GIFTOFKNOWLEDGE_WAV);

		skillDefinitions["Paraclete's Invitation"] = Skill("Paraclete's Invitation", "Paraclete's Invitation", "Cleromancy", SKILLICON_PARACLETESINVITATION, 40, 0, 5, "SINGLEOTHERALLYWITHNOBOONS",
			list<string>({ "APPLY_Paraclete's Invitation_SINGLE", }),
			list<string>({ "MAGICAL","HOLY", "HEAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Paraclete's Invitation", PowerValue("DURATION_Paraclete's Invitation", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Paraclete's Invitation", PowerValue("POWER_Paraclete's Invitation", 10, 10, 10, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF", }), PARACLETESINVITATION_WAV);

		skillDefinitions["Iridescent Breath"] = Skill("Iridescent Breath", "Iridescent Breath", "Cleromancy", SKILLICON_IRIDESCENTBREATH, 10, 1, 3, "SELF",
			list<string>({ "REMOVEBANE_ALLALLIES", }),
			list<string>({ "MAGICAL","HOLY", "HEAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RECHARGE", PowerValue("RECHARGE", 2, 0, 10, false, list<string>({ "INTELLIGENCE", "HOLYBOOST", "MAGICALRECHARGE","HOLYRECHARGE"}))),
				}),
				list<string>({ "ENCHANTSELF", }), REMEDYWARD_WAV);

		skillDefinitions["Healing Winds"] = Skill("Healing Winds", "Healing Winds", "Cleromancy", SKILLICON_HEALINGWINDS, 25, 1, 3, "SELF",
			list<string>({ "LIFEHEAL_ALLALLIES_HOLY", }),
			list<string>({ "MAGICAL","HOLY", "HEAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFEHEAL_ALLALLIES_HOLY", PowerValue("LIFEHEAL_ALLALLIES_HOLY", 60, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST", }))),
				}),
				list<string>({ "ENCHANTSELF", }), HEALINGWINDS_WAV);

		skillDefinitions["Sanctum Shroud"] = Skill("Sanctum Shroud", "Sanctum Shroud", "Cleromancy", SKILLICON_SANCTUMSHROUD, 10, 0, 6, "SELF",
			list<string>({ "APPLY_Sanctum Shroud_SELF", }),
			list<string>({ "MAGICAL","HOLY", "HEAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Sanctum Shroud", PowerValue("DURATION_Sanctum Shroud", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST", }))),
				pair<string, PowerValue>("POWER_Sanctum Shroud", PowerValue("POWER_Sanctum Shroud", 8, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST", }))),
				}),
				list<string>({ "ENCHANTSELF", }), SANCTUMSHROUD_WAV);

		// HAGIOMANCY
		skillDefinitions["Heavenstrike"] = Skill("Heavenstrike", "Heavenstrike", "Hagiomancy", SKILLICON_HEAVENSTRIKE, 10, 1, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_HOLY", "HEAVENSTRIKE" }),
			list<string>({ "MAGICAL","HOLY", "ELITE", "TARGETSFOES" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_HOLY", PowerValue("DAMAGE_SINGLE_HOLY", 70, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("SKILLRECHARGEDELAY", PowerValue("DAMAGE_SINGLE_HOLY", 4, 4, 4, true, list<string>())),
				pair<string, PowerValue>("HEAVENSTRIKE", PowerValue("HEAVENSTRIKE", 50, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
				list<string>({ "DEALDAMAGE", "DEALDAMAGE_HOLY" }), HEALWOUNDS_WAV);

		skillDefinitions["Light of Day"] = Skill("Light of Day", "Light of Day", "Hagiomancy", SKILLICON_LIGHTOFDAY, 5, 0, 0, "ALLUNDEADORDEMONICFOES",
			list<string>({ "DAMAGE_HOLY_ALLUNDEADORDEMONICFOES" }),
			list<string>({ "MAGICAL","HOLY", "TARGETSFOES" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("DAMAGE_HOLY_SINGLE", PowerValue("DAMAGE_HOLY_SINGLE", 30, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({ "KILLUNDEAD", "KILLDEMONS" }), LIGHTOFDAY_WAV);

		skillDefinitions["Exile"] = Skill("Exile", "Exile", "Hagiomancy", SKILLICON_EXILE, 10, 0, 10, "SINGLESUMMONEDFOE",
			list<string>({ "EXILE_SINGLE" }),
			list<string>({ "MAGICAL","HOLY", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RECHARGE", PowerValue("RECHARGE", 10, 0, 10, false, list<string>({ "INTELLIGENCE", "HOLYBOOST", "MAGICALRECHARGE","HOLYRECHARGE"}))),
				}),
				list<string>({ "KILLSUMMON" }), EXILE_WAV);

		skillDefinitions["Strength of Reason"] = Skill("Strength of Reason", "Strength of Reason", "Hagiomancy", SKILLICON_STRENGTHOFREASON, 10, 0, 1, "SINGLEALLY",
			list<string>({ "APPLY_Strength of Reason_SINGLE" }),
			list<string>({ "MAGICAL","HOLY","BOON","TARGETSALLIES" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Strength of Reason", PowerValue("DURATION_Strength of Reason", 5, 0, 10, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Strength of Reason", PowerValue("POWER_Strength of Reason", 8, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))), }),
				list<string>({ "ENCHANTSELF","ENCHANTALLY", "PHYSICALBUFFSELF","PHYSICALBUFFALLY" }), HEALWOUNDS_WAV);

		skillDefinitions["Castigate Cruor"] = Skill("Castigate Cruor", "Castigate Cruor", "Hagiomancy", SKILLICON_CASTIGATECRUOR, 5, 0, 0, "SINGLEFOE",
			list<string>({ "APPLY_Castigate Cruor_SINGLE" }),
			list<string>({ "MAGICAL","HOLY",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Castigate Cruor", PowerValue("DURATION_Castigate Cruor", 15, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
 }),
				list<string>({ "CURSEFOE",}), CASTIGATECRUOR_WAV);

		skillDefinitions["Prophesized Return"] = Skill("Prophesized Return", "Prophesized Return", "Hagiomancy", SKILLICON_PROPHESIZEDRETURN, 25, 2, 10, "DEADPARTYMEMBER",
			list<string>({ "RESURRECT_SINGLE_HOLY", "DAMAGE_ALLFOES_HOLY"}),
			list<string>({ "MAGICAL","HOLY", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RESURRECT_SINGLE_HOLY", PowerValue("RESURRECT_SINGLE_HOLY", 8, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("DAMAGE_ALLFOES_HOLY", PowerValue("DAMAGE_ALLFOES_HOLY", 70, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "RESURRECT", }), PROPHESIZEDRETURN_WAV);

		skillDefinitions["Entomb Spirit"] = Skill("Entomb Spirit", "Entomb Spirit", "Hagiomancy", SKILLICON_ENTOMBSPIRIT, 5, 0, 0, "SINGLEFOE",
			list<string>({ "APPLY_Entomb Spirit_SINGLE",}),
			list<string>({ "MAGICAL","HOLY", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Entomb Spirit", PowerValue("DURATION_Entomb Spirit", 15, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "CURSEFOE", }), PROPHESIZEDRETURN_WAV);

		skillDefinitions["Exalted Smash"] = Skill("Exalted Smash", "Exalted Smash", "Hagiomancy", SKILLICON_EXALTEDSMASH, 15, 0, 3, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_HOLY", }),
			list<string>({ "MAGICAL","ATTACK", "HOLY", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_HOLY", PowerValue("DAMAGE_SINGLE_HOLY", 18, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), EXALTEDSMASH_WAV);

		skillDefinitions["Erase Evil"] = Skill("Erase Evil", "Erase Evil", "Hagiomancy", SKILLICON_ERASEEVIL, 20, 0, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_HOLY", }),
			list<string>({ "MAGICAL","HOLY", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_HOLY", PowerValue("DAMAGE_SINGLE_HOLY", 20, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), ERASEEVIL_WAV);

		skillDefinitions["Absolution"] = Skill("Absolution", "Absolution", "Hagiomancy", SKILLICON_ABSOLUTION, 25, 0, 2, "SINGLEALLY",
			list<string>({ "REMOVEXBANES_SINGLE", }),
			list<string>({ "MAGICAL","HOLY", "ELITE",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER_Absolution", PowerValue("POWER_Absolution", 15, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("power", PowerValue("power", 2, 2, 2, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "CUREALLY", }), ABSOLUTION_WAV);

		skillDefinitions["Adjudicate"] = Skill("Adjudicate", "Adjudicate", "Hagiomancy", SKILLICON_ADJUDICATE, 25, 0, 4, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_HOLY", "INTERRUPT_SINGLE_HOLY", }),
			list<string>({ "MAGICAL","HOLY", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_HOLY", PowerValue("DAMAGE_SINGLE_HOLY", 30, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), ADJUDICATE_WAV);

		skillDefinitions["Stalked by Vengeance"] = Skill("Stalked by Vengeance", "Stalked by Vengeance", "Hagiomancy", SKILLICON_STALKEDBYVENGEANCE, 20, 0, 8, "SINGLEFOE",
			list<string>({ "APPLY_Stalked by Vengeance_SINGLE" }),
			list<string>({ "MAGICAL","HOLY", "ELITE",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Stalked by Vengeance", PowerValue("DURATION_Stalked by Vengeance", 5, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Stalked by Vengeance", PowerValue("POWER_Stalked by Vengeance", 33, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "CURSEFOE",}), STALKEDBYVENGEANCE_WAV);

		skillDefinitions["Overrule"] = Skill("Overrule", "Overrule", "Hagiomancy", SKILLICON_OVERRULE, 25, 1, 5, "SINGLEFOE",
			list<string>({ "DAMAGE_ALLFOES_HOLY", "INTERRUPT_ALLFOES_HOLY", }),
			list<string>({ "MAGICAL","HOLY", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_HOLY", PowerValue("DAMAGE_SINGLE_HOLY", 30, 0, 99, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), OVERRULE_WAV);

		skillDefinitions["Bewrayment"] = Skill("Bewrayment", "Bewrayment", "Hagiomancy", SKILLICON_BEWRAYMENT, 5, 0, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_HOLY", }),
			list<string>({ "MAGICAL","HOLY", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_HOLY", PowerValue("DAMAGE_SINGLE_HOLY", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("bonus", PowerValue("bonus", 70, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), BEWRAYMENT_WAV);

		skillDefinitions["Avenger's Prayer"] = Skill("Avenger's Prayer", "Avenger's Prayer", "Hagiomancy", SKILLICON_AVENGERSPRAYER, 10, 0, 2, "SINGLEALLY",
			list<string>({ "APPLY_Avenger's Prayer_SINGLE" }),
			list<string>({ "MAGICAL","HOLY","BOON","TARGETSALLIES" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Avenger's Prayer", PowerValue("DURATION_Avenger's Prayer", 8, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Avenger's Prayer", PowerValue("POWER_Avenger's Prayer", 15, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))), }),
				list<string>({ "ENCHANTSELF","ENCHANTALLY", }), HEALWOUNDS_WAV);

		skillDefinitions["Proscribe"] = Skill("Proscribe", "Proscribe", "Hagiomancy", SKILLICON_PROSCRIBE, 5, 0, 2, "SINGLEFOE",
			list<string>({ "APPLY_Proscribe_SINGLE" }),
			list<string>({ "MAGICAL","HOLY",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Proscribe", PowerValue("DURATION_Proscribe", 8, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))), }),
				list<string>({ "CURSEFOE", }), OVERRULE_WAV);

		skillDefinitions["Conversion"] = Skill("Conversion", "Conversion", "Hagiomancy", SKILLICON_CONVERSION, 5, 0, 2, "SELF",
			list<string>({ "APPLY_Conversion_SINGLE" }),
			list<string>({ "MAGICAL","HOLY", "ELITE"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Conversion", PowerValue("DURATION_Conversion", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Conversion", PowerValue("POWER_Conversion", 15, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST" }))), }),
				list<string>({ "ENCHANTSELF", }), CONVERSION_WAV);

		skillDefinitions["Fading Justice"] = Skill("Fading Justice", "Fading Justice", "Hagiomancy", SKILLICON_FADINGJUSTICE, 5, 0, 0, "SINGLEALLY",
			list<string>({ "APPLY_Fading Justice_SINGLE" }),
			list<string>({ "MAGICAL","HOLY"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Fading Justice", PowerValue("DURATION_Fading Justice", 8, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Fading Justice", PowerValue("POWER_Fading Justice", 3, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST" }))), }),
				list<string>({ "ENCHANTALLY", }), FADINGJUSTICE_WAV);

		skillDefinitions["Suppress"] = Skill("Suppress", "Suppress", "Hagiomancy", SKILLICON_SUPPRESS, 20, 0, 2, "SINGLEFOE",
			list<string>({ "APPLY_Suppress_SINGLE" }),
			list<string>({ "MAGICAL","HOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Suppress", PowerValue("DURATION_Suppress", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Suppress", PowerValue("POWER_Suppress", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST" }))),
				pair<string, PowerValue>("DAMAGE_SINGLE_HOLY", PowerValue("DAMAGE_SINGLE_HOLY", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST" }))), }),
				list<string>({ "CURSEFOE", }), SUPPRESS_WAV);

		skillDefinitions["Godly Repulsion"] = Skill("Godly Repulsion", "Godly Repulsion", "Hagiomancy", SKILLICON_GODLYREPULSION, 20, 1, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_ALLFOES_HOLY" }),
			list<string>({ "MAGICAL","HOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_HOLY", PowerValue("DAMAGE_SINGLE_HOLY", 25, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST" }))), }),
				list<string>({ "DEALDAMAGE", }), GODLYREPULSION_WAV);

		// SANGROMANCY
		skillDefinitions["Life Drain"] = Skill("Life Drain", "Life Drain", "Sangromancy", SKILLICON_LIFEDRAIN, 10, 0, 1, "SINGLEFOE",
			list<string>({ "APPLY_Life Drain_SINGLE",}),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Life Drain", PowerValue("DURATION_Life Drain", 5, 5, 8, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("POWER_Life Drain", PowerValue("POWER_Life Drain", 8, 1, 10, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("LIFESTEAL_SINGLE_UNHOLY", PowerValue("POWER_Life Drain", 8, 1, 10, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"})))
				}),
			list<string>({ "DEALDAMAGE", "HEALSELF" }), 1060);

		skillDefinitions["Atrophy"] = Skill("Atrophy", "Atrophy", "Sangromancy", SKILLICON_ATROPHY, 5, 0, 0, "SINGLEFOE",
			list<string>({ "APPLY_WEAKENED_SINGLE" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({ 
				pair<string, PowerValue>("DURATION_WEAKENED", PowerValue("DURATION_WEAKENED", 5, 0, 15, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "CURSEFOE", }), ATROPHY_WAV);

		skillDefinitions["Vampiric Strike"] = Skill("Vampiric Strike", "Vampiric Strike", "Sangromancy", SKILLICON_VAMPIRICSTRIKE, 10, 0, 2, "SINGLEFOE",
			list<string>({ "APPLY_BLEEDING_SINGLE", }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({ 
				pair<string, PowerValue>("DURATION_BLEEDING", PowerValue("DURATION_BLEEDING", 3, 0, 15, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("LIFESTEAL_SINGLE_UNHOLY", PowerValue("LIFESTEAL_SINGLE_UNHOLY", 9, 0, 99, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "DEALDAMAGE", }), ATROPHY_WAV);

		skillDefinitions["Blade of Blood"] = Skill("Blade of Blood", "Blade of Blood", "Sangromancy", SKILLICON_BLADEOFBLOOD, 5, 0, 5, "SINGLEALLY",
			list<string>({ "APPLY_Blade of Blood_SINGLE" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY", "ELITE"}),
			Map<string, PowerValue>({ 
				pair<string, PowerValue>("DURATION_Blade of Blood", PowerValue("DURATION_Blade of Blood", 5, 0, 15, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("POWER_Blade of Blood", PowerValue("POWER_Blade of Blood", 16, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
					}),
			list<string>({ "ENCHANTSELF","ENCHANTALLY", "PHYSICALBUFFSELF","PHYSICALBUFFALLY", }), BLADEOFBLOOD_WAV);

		skillDefinitions["Aura Drain"] = Skill("Aura Drain", "Aura Drain", "Sangromancy", SKILLICON_AURADRAIN, 10, 0, 4, "SINGLEFOE",
			list<string>({ "REMOVEBOONANDMANASTEAL_SINGLE_UNHOLY" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("POWER_Aura Drain", PowerValue("Power_Aura Drain", 20, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "REMOVEBOON", }), AURADRAIN_WAV);

		skillDefinitions["Septicemia"] = Skill("Septicemia", "Septicemia", "Sangromancy", SKILLICON_SEPTICEMIA, 10, 0, 2, "SINGLEFOE",
			list<string>({ "APPLY_DISEASEDIF?TARGETHASBLEEDING_SINGLE" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("DURATION_DISEASED", PowerValue("DURATION_DISEASED", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "CURSEFOE", }), ATROPHY_WAV);

		skillDefinitions["Blood Boil"] = Skill("Blood Boil", "Blood Boil", "Sangromancy", SKILLICON_BLOODBOIL, 20, 0, 2, "SINGLEFOE",
			list<string>({ "APPLY_BURNINGIF?TARGETHASBLEEDING_SINGLE" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("DURATION_BURNING", PowerValue("DURATION_BURNING", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "CURSEFOE", }), ATROPHY_WAV);

		skillDefinitions["Curse from Beyond the Grave"] = Skill("Curse from Beyond the Grave", "Curse from Beyond the Grave", "Sangromancy", SKILLICON_CURSEFROMBEYONDTHEGRAVE, 15, 0, 0, "SINGLEFOE",
			list<string>({ "APPLY_Curse from Beyond the Grave_SINGLE" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY", "ELITE"}),
			Map<string, PowerValue>({ pair<string, PowerValue>("DURATION_Curse from Beyond the Grave", PowerValue("DURATION_Curse from Beyond the Grave", 8, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "CURSEFOE", }), CURSEFROMBEYONDTHEGRAVE_WAV);

		skillDefinitions["Brain Drain"] = Skill("Brain Drain", "Brain Drain", "Sangromancy", SKILLICON_BRAINDRAIN, 10, 0, 2, "DEADFOE",
			list<string>({ "MANAHEAL_SELF_UNHOLY" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("MANAHEAL_SINGLE_UNHOLY", PowerValue("MANAHEAL_SINGLE_UNHOLY", 15, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "HEALSELF", }), ATROPHY_WAV);

		skillDefinitions["Wishing Well"] = Skill("Wishing Well", "Wishing Well", "Sangromancy", SKILLICON_WISHINGWELL, 20, 0, 0, "DEADFOE",
			list<string>({ "WISHINGWELL_SELF" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY", "ELITE"}),
			Map<string, PowerValue>({ 
				pair<string, PowerValue>("RECHARGE", PowerValue("RECHARGE", 15, 1, 5, false, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "HEALSELF", }), WISHINGWELL_WAV);

		skillDefinitions["Blood Gift"] = Skill("Blood Gift", "Blood Gift", "Sangromancy", SKILLICON_BLOODGIFT, 10, 0, 4, "SINGLEOTHERALLY",
			list<string>({  "LIFEHEAL_SINGLE_UNHOLY", "APPLY_BLEEDING_SELF" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({ 
				pair<string, PowerValue>("LIFEHEAL_SINGLE_UNHOLY", PowerValue("LIFEHEAL_SINGLE_UNHOLY", 50, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("APPLY_BLEEDING_SELF", PowerValue("APPLY_BLEEDING_SELF", 2, 2, 2, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "HEALSELF", }), BLOODGIFT_WAV);

		skillDefinitions["Order of the Wasp"] = Skill("Order of the Wasp", "Order of the Wasp", "Sangromancy", SKILLICON_ORDEROFTHEWASP, 20, 0, 2, "SELF",
			list<string>({ "APPLY_Order of the Wasp_ALLALLIES" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Order of the Wasp", PowerValue("DURATION_Order of the Wasp", 2, 0, 5, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("POWER_Order of the Wasp", PowerValue("POWER_Order of the Wasp", 8, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
				list<string>({ "HEALSELF", }), ORDEROFTHEWASP_WAV);

		skillDefinitions["Viper Eyes"] = Skill("Viper Eyes", "Viper Eyes", "Sangromancy", SKILLICON_VIPEREYES, 20, 0, 5, "SELF",
			list<string>({ "APPLY_Viper Eyes_SELF" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Viper Eyes", PowerValue("DURATION_Viper Eyes", 5, 0, 20, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
				list<string>({ "HEALSELF", }), ATROPHY_WAV);

		skillDefinitions["Contract from Below"] = Skill("Contract from Below", "Contract from Below", "Sangromancy", SKILLICON_CONTRACTFROMBELOW, 1, 0, 10, "SELF",
			list<string>({ "LIFESACRIFICE_SELF_UNHOLY", "MANAHEAL_SELF_UNHOLY"}),
			list<string>({ "MAGICAL","BLOOD","UNHOLY"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFESACRIFICE_SELF_UNHOLY", PowerValue("LIFESACRIFICE", 50, 50, 999, true, list<string>({}))),
				pair<string, PowerValue>("MANAHEAL_SELF_UNHOLY", PowerValue("MANAHEAL_SELF_UNHOLY", 50, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
				list<string>({ "HEALSELF", }), ATROPHY_WAV);

		skillDefinitions["Hypoxia"] = Skill("Hypoxia", "Hypoxia", "Sangromancy", SKILLICON_HYPOXIA, 15, 0, 2, "SINGLEFOE",
			list<string>({ "INTERRUPTIF?TARGETHASWEAKENED_SINGLE" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFESTEAL_SINGLE_UNHOLY", PowerValue("LIFESTEAL_SINGLE_UNHOLY", 10, 0, 999, true, list<string>({}))),
				}),
				list<string>({ "DEALDAMAGE", }), HYPOXIA_WAV);

		skillDefinitions["Pact with Darkness"] = Skill("Pact with Darkness", "Pact with Darkness", "Sangromancy", SKILLICON_PACTWITHDARKNESS, 15, 0, 5, "SINGLEALLY",
			list<string>({ "LIFESACRIFICE_SELF_UNHOLY", "MANAHEAL_SINGLE_UNHOLY" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFESACRIFICE_SELF_UNHOLY", PowerValue("LIFESACRIFICE", 33, 33, 999, true, list<string>({}))),
				pair<string, PowerValue>("MANAHEAL_SINGLE_UNHOLY", PowerValue("MANAHEAL_SINGLE_UNHOLY", 20, 0, 999, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
				list<string>({ "HEALALLY", }), ATROPHY_WAV);

		skillDefinitions["Beggar's Blessing"] = Skill("Beggar's Blessing", "Beggar's Blessing", "Sangromancy", SKILLICON_BEGGARSBLESSING, 25, 0, 3, "SINGLEFOE",
			list<string>({ "APPLY_Beggar's Blessing_SINGLE" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Beggar's Blessing", PowerValue("DURATION_Beggar's Blessing", 5, 0, 999, true, list<string>({"INTELLIGENCE", "BLOODBOOST"}))),
				}),
				list<string>({ "CURSEFOE", }), BEGGARBLESSING_WAV);

		skillDefinitions["Botched Procedure"] = Skill("Botched Procedure", "Botched Procedure", "Sangromancy", SKILLICON_BOTCHEDPROCEDURE, 20, 0, 0, "SINGLEFOE",
			list<string>({ "REMOVEXBANES_LIFESTEAL_SINGLE_UNHOLY" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFESTEAL_SINGLE_UNHOLY", PowerValue("LIFESTEAL_SINGLE_UNHOLY", 5, 0, 999, true, list<string>({"INTELLIGENCE", "BLOODBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), BOTCHEDPROCEDURE_WAV);

		skillDefinitions["Cestodarian Siphon"] = Skill("Cestodarian Siphon", "Cestodarian Siphon", "Sangromancy", SKILLICON_CESTODARIANSIPHON, 20, 0, 4, "SINGLEFOE",
			list<string>({ "APPLY_Cestodarian Siphon_SINGLE" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER_Cestodarian Siphon", PowerValue("POWER_Cestodarian", 15, 0, 999, true, list<string>({"INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("DURATION_Cestodarian Siphon", PowerValue("POWER_Cestodarian", 8, 0, 999, true, list<string>({"INTELLIGENCE", "BLOODBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), CESTODARIANSIPHON_WAV);

		skillDefinitions["Lacrymactory"] = Skill("Lacrymactory", "Lacrymactory", "Sangromancy", SKILLICON_LACRYMACTORY, 5, 0, 10, "SELF",
			list<string>({ "APPLY_Lacrymactory_SELF" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER_Lacrymactory", PowerValue("POWER_Lacrymactory", 10, 0, 999, true, list<string>({"INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("DURATION_Lacrymactory", PowerValue("POWER_Lacrymactory", 10, 0, 999, true, list<string>({"INTELLIGENCE", "BLOODBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF", }), LACRYMACTORY_WAV);

		// NECROMANCY
		skillDefinitions["Animate Skeleton Warrior"] = Skill("Animate Skeleton Warrior", "Animate Skeleton Warrior", "Necromancy", SKILLICON_ANIMATESKELETONWARRIOR, 30, 1, 8, "SELF", // debug 0, real = 2
			list<string>({ "SUMMON_Skeleton Warrior_SELF" }),
			list<string>({ "MAGICAL","UNHOLY", "ELITE", "SUMMON" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFE", PowerValue("LIFE", 80, 0, 999, true, list<string>({ "INTELLIGENCE", "UNHOLYBOOST"}))),
				pair<string, PowerValue>("STRENGTH", PowerValue("STRENGTH", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "UNHOLYBOOST"}))),
				}),
				list<string>({ "SUMMON", }), 5686);

		skillDefinitions["Animate Mound of Leeches"] = Skill("Animate Mound of Leeches", "Animate Mound of Leeches", "Necromancy", SKILLICON_ANIMATEMOUNDOFLEECHES, 40, 1, 10, "SELF",
			list<string>({ "SUMMON_Mound of Leeches_SELF" }),
			list<string>({ "MAGICAL","UNHOLY", "SUMMON" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFE", PowerValue("LIFE", 25, 0, 999, true, list<string>({ "INTELLIGENCE", "UNHOLYBOOST"}))),
				pair<string, PowerValue>("INTELLIGENCE", PowerValue("INTELLIGENCE", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "UNHOLYBOOST"}))),
				}),
				list<string>({ "SUMMON", }), 7064);

		// METEOMANCY
		skillDefinitions["Rainstorm"] = Skill("Rainstorm", "Rainstorm", "Meteomancy", SKILLICON_RAINSTORM, 15, 0, 0, "ALL", 
			list<string>({ "APPLY_Rainstorm_WORLD", "APPLY_WET_ALL" }),
			list<string>({ "MAGICAL","WATER","ELEMENTAL" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Rainstorm", PowerValue("DURATION_Rainstorm", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Rainstorm", PowerValue("POWER_Rainstorm", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("DURATION_WET", PowerValue("DURATION_WET", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				}),
				list<string>({ "MAKEITRAIN", "WORLDEFFECT"}), RAINSTORM_WAV);

		skillDefinitions["Sandstorm"] = Skill("Sandstorm", "Sandstorm", "Meteomancy", SKILLICON_SANDSTORM, 15, 0, 0, "ALL", 
			list<string>({ "APPLY_Sandstorm_WORLD", "APPLY_DUSTY_ALL" }),
			list<string>({ "MAGICAL","EARTH","ELEMENTAL" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Sandstorm", PowerValue("DURATION_Sandstorm", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Sandstorm", PowerValue("POWER_Sandstorm", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("DURATION_DUSTY", PowerValue("DURATION_DUSTY", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				}),
				list<string>({ "MAKEITSAND", "WORLDEFFECT"}), SANDSTORM_WAV);

		skillDefinitions["Heatwave"] = Skill("Heatwave", "Heatwave", "Meteomancy", SKILLICON_HEATWAVE, 15, 0, 0, "ALL", 
			list<string>({ "APPLY_Heatwave_WORLD", "APPLY_PARCHED_ALL" }),
			list<string>({ "MAGICAL","FIRE","ELEMENTAL" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Heatwave", PowerValue("DURATION_Heatwave", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Heatwave", PowerValue("POWER_Heatwave", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("DURATION_PARCHED", PowerValue("DURATION_PARCHED", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				}),
				list<string>({ "MAKEITHOT", "WORLDEFFECT" }), SANDSTORM_WAV);

		skillDefinitions["Pressure Front"] = Skill("Pressure Front", "Pressure Front", "Meteomancy", SKILLICON_PRESSUREFRONT, 15, 0, 0, "ALL", 
			list<string>({ "APPLY_Pressure Front_WORLD", "APPLY_STATIC_ALL" }),
			list<string>({ "MAGICAL","ELECTRIC","ELEMENTAL" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Pressure Front", PowerValue("DURATION_Pressure Front", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Pressure Front", PowerValue("POWER_Pressure Front", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("DURATION_STATIC", PowerValue("DURATION_STATIC", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				}),
				list<string>({ "MAKEITSTATIC", "WORLDEFFECT" }), PRESSUREFRONT_WAV);

		skillDefinitions["Ice Age"] = Skill("Ice Age", "Ice Age", "Meteomancy", SKILLICON_ICEAGE, 15, 0, 0, "ALL",
			list<string>({ "APPLY_Ice AgeW_WORLD", "APPLY_Ice Age_ALL" }), // Ice AgeW = world effect that applies Ice Age
			list<string>({ "MAGICAL","WATER","ELEMENTAL", "ELITE",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Ice AgeW", PowerValue("DURATION_Ice AgeW", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Ice AgeW", PowerValue("POWER_Ice AgeW", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Ice Age", PowerValue("POWER_Ice Age", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("DURATION_Ice Age", PowerValue("DURATION_Ice Age", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				}),
				list<string>({ "WORLDEFFECT" }), ICEAGE_WAV);

		skillDefinitions["Global Warming"] = Skill("Global Warming", "Global Warming", "Meteomancy", SKILLICON_GLOBALWARMING, 15, 0, 0, "ALL",
			list<string>({ "APPLY_Global WarmingW_WORLD", "APPLY_Global Warming_ALL" }), // Ice AgeW = world effect that applies Ice Age
			list<string>({ "MAGICAL","FIRE","ELEMENTAL", "ELITE", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Global WarmingW", PowerValue("DURATION_Global WarmingW", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Global WarmingW", PowerValue("POWER_Global WarmingW", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Global Warming", PowerValue("POWER_Global Warming", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("DURATION_Global Warming", PowerValue("DURATION_Global Warming", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				}),
				list<string>({ "WORLDEFFECT" }), GLOBALWARMING_WAV);

		skillDefinitions["Tempest"] = Skill("Tempest", "Tempest", "Meteomancy", SKILLICON_TEMPEST, 15, 0, 0, "ALL",
			list<string>({ "APPLY_TempestW_WORLD", "APPLY_Tempest_ALL" }), // Ice AgeW = world effect that applies Ice Age
			list<string>({ "MAGICAL","FIRE","ELEMENTAL", "ELITE", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_TempestW", PowerValue("DURATION_TempestW", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_TempestW", PowerValue("POWER_TempestW", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Tempest", PowerValue("POWER_Tempest", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("DURATION_Tempest", PowerValue("DURATION_Tempest", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				}),
				list<string>({ "WORLDEFFECT" }), TEMPEST_WAV);

		skillDefinitions["Drought"] = Skill("Drought", "Drought", "Meteomancy", SKILLICON_DROUGHT, 15, 0, 0, "ALL",
			list<string>({ "APPLY_DroughtW_WORLD", "APPLY_Drought_ALL" }), // Ice AgeW = world effect that applies Ice Age
			list<string>({ "MAGICAL","FIRE","ELEMENTAL", "ELITE", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_DroughtW", PowerValue("DURATION_DroughtW", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_DroughtW", PowerValue("POWER_DroughtW", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("POWER_Drought", PowerValue("POWER_Drought", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("DURATION_Drought", PowerValue("DURATION_Drought", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				}),
				list<string>({ "WORLDEFFECT" }), DROUGHT_WAV);

		// ELECTROMANCY
		skillDefinitions["Plasma Pulse"] = Skill("Plasma Pulse", "Plasma Pulse", "Electromancy", SKILLICON_PLASMAPULSE, 25, 1, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_ELECTRIC", "APPLY_CONCUSSED_SINGLE" }),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_ELECTRIC", PowerValue("DAMAGE_SINGLE_ELECTRIC", 80, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DURATION_CONCUSSED", PowerValue("DURATION_CONCUSSED", 3, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), -1);

		skillDefinitions["Charge Bolt"] = Skill("Charge Bolt", "Charge Bolt", "Electromancy", SKILLICON_CHARGEBOLT, 20, 0, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_ELECTRIC", "APPLY_Charge Bolt2_SELF" }),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_ELECTRIC", PowerValue("DAMAGE_SINGLE_ELECTRIC", 20, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DURATION_Charge Bolt2", PowerValue("DURATION_Charge Bolt2", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER_Charge Bolt2",PowerValue("POWER_Charge Bolt2",2,0,999,true,list<string>({"INTELLIGENCE", "ELECTRICBOOST","ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), CHARGEBOLT_WAV);

		skillDefinitions["Shock Value"] = Skill("Shock Value", "Shock Value", "Electromancy", SKILLICON_SHOCKVALUE, 5, 0, 1, "SELF",
			list<string>({ "APPLY_Shock Value_SELF", }),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Shock Value", PowerValue("DURATION_Shock Value", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER1",PowerValue("POWER1",2,0,999,true,list<string>({"INTELLIGENCE", "ELECTRICBOOST","ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER2",PowerValue("POWER2",8,0,999,true,list<string>({"INTELLIGENCE", "ELECTRICBOOST","ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER1_Shock Value",PowerValue("POWER1_Shock Value",2,0,999,true,list<string>({"INTELLIGENCE", "ELECTRICBOOST","ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER2_Shock Value",PowerValue("POWER2_Shock Value",8,0,999,true,list<string>({"INTELLIGENCE", "ELECTRICBOOST","ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), CHARGEBOLT_WAV);

		skillDefinitions["Electrocute"] = Skill("Electrocute", "Electrocute", "Electromancy", SKILLICON_ELECTROCUTE, 25, 1, 3, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_ELECTRIC", "APPLY_CRIPPLED_SINGLE" }),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_ELECTRIC", PowerValue("DAMAGE_SINGLE_ELECTRIC", 50, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DURATION_CRIPPLED", PowerValue("DURATION_CRIPPLED", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), CHARGEBOLT_WAV);

		skillDefinitions["Ball Lightning"] = Skill("Ball Lightning", "Ball Lightning", "Electromancy", SKILLICON_BALLLIGHTNING, 25, 1, 3, "SINGLEFOE",
			list<string>({ "DAMAGE_TARGETPLUSSUMMONS_ELECTRIC",}),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_TARGETPLUSSUMMONS_ELECTRIC", PowerValue("DAMAGE_TARGETPLUSSUMMONS_ELECTRIC", 50, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), CHARGEBOLT_WAV);

		skillDefinitions["Double-Edged Lightning"] = Skill("Double-Edged Lightning", "Double-Edged Lightning", "Electromancy", SKILLICON_DOUBLEEDGEDLIGHTNING, 30, 0, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_ELECTRIC", "DAMAGE_SELF_ELECTRIC"}),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", "ELITE",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_ELECTRIC", PowerValue("DAMAGE_SINGLE_ELECTRIC", 80, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DAMAGE_SELF_ELECTRIC", PowerValue("DAMAGE_SELF_ELECTRIC", 15, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), DOUBLEEDGEDLIGHTNING_WAV);

		skillDefinitions["Chain Lightning"] = Skill("Chain Lightning", "Chain Lightning", "Electromancy", SKILLICON_CHAINLIGHTNING, 20, 1, 5, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_ELECTRIC",}),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_ELECTRIC", PowerValue("DAMAGE_SINGLE_ELECTRIC", 60, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), CHAINLIGHTNING_WAV);

		skillDefinitions["Blinding Flash"] = Skill("Blinding Flash", "Blinding Flash", "Electromancy", SKILLICON_BLINDINGFLASH, 20, 0, 4, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_ELECTRIC", "APPLY_BLIND_SINGLE"}),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_ELECTRIC", PowerValue("DAMAGE_SINGLE_ELECTRIC", 30, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DURATION_BLIND", PowerValue("DURATION_BLIND", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), BLINDINGFLASH_WAV);

		skillDefinitions["Electric Loop"] = Skill("Electric Loop", "Electric Loop", "Electromancy", SKILLICON_BLINDINGFLASH, 20, 0, 4, "SINGLEFOE",
			list<string>({ "APPLY_Electric Loop_SELF", }),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Electric Loop", PowerValue("DURATION_Electric Loop", 6, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER_Electric Loop", PowerValue("POWER_Electric Loop", 15, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), SKILLICON_ELECTRICLOOP);

		// MINOR ARMS
		skillDefinitions["Doublestrike"] = Skill("Doublestrike", "Doublestrike", "Minor Arms", SKILLICON_DOUBLESTRIKE, 15, 0, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", "DAMAGE_SINGLE_PHYSICAL" }),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL", PowerValue("DAMAGE_SINGLE_PHYSICAL", 11, 0, 999, true, list<string>({ "STRENGTH", "Minor ArmsBOOST"}))), }),
				list<string>({ "DEALDAMAGE" }), DOUBLESTRIKE_1_WAV);

		skillDefinitions["Serrated Strike"] = Skill("Serrated Strike", "Serrated Strike", "Minor Arms", SKILLICON_SERRATEDSTRIKE, 15, 0, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", "APPLY_BLEEDING_SINGLE" }),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL", PowerValue("DAMAGE_SINGLE_PHYSICAL", 12, 0, 999, true, list<string>({ "STRENGTH", "Minor ArmsBOOST"}))),
				pair<string, PowerValue>("DURATION_BLEEDING", PowerValue("DURATION_BLEEDING", 3, 0, 999, true, list<string>({ "STRENGTH", "Minor ArmsBOOST"}))), }),
				list<string>({ "DEALDAMAGE" }), SERRATEDSTRIKE_WAV);

		skillDefinitions["Deathdancer's Strike"] = Skill("Deathdancer's Strike", "Deathdancer's Strike", "Minor Arms", SKILLICON_DEATHDANCERSSTRIKE, 10, 0, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL", PowerValue("DAMAGE_SINGLE_PHYSICAL", 12, 0, 999, true, list<string>({ "STRENGTH", "Minor ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), DEATHDANCERSTRIKE_WAV);

		skillDefinitions["Natural Stab"] = Skill("Natural Stab", "Natural Stab", "Minor Arms", SKILLICON_NATURALSTAB, 10, 0, 3, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL", PowerValue("DAMAGE_SINGLE_PHYSICAL", 10, 0, 999, true, list<string>({ "STRENGTH", "Minor ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), NATURALSTAB_WAV);

		skillDefinitions["Platinum Lotus Strike"] = Skill("Platinum Lotus Strike", "Platinum Lotus Strike", "Minor Arms", SKILLICON_PLATINUMLOTUSSTRIKE, 5, 0, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", "MANAHEAL_SELF_SHADOW"}),
			list<string>({ "PHYSICAL","ATTACK", "ELITE",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL", PowerValue("DAMAGE_SINGLE_PHYSICAL", 30, 0, 999, true, list<string>({ "STRENGTH", "Minor ArmsBOOST"}))),
				pair<string, PowerValue>("MANAHEAL_SELF_SHADOW", PowerValue("MANAHEAL_SELF_SHADOW", 15, 0, 999, true, list<string>({ "STRENGTH", "Minor ArmsBOOST", "SHADOWBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), PLATINUMLOTUSSTRIKE_WAV);

		// UMBROMANCY
		skillDefinitions["Shadow Spike"] = Skill("Shadow Spike", "Shadow Spike", "Umbromancy", SKILLICON_SHADOWSPIKE, 15, 0, 0, "SINGLEFOE",
			list<string>({ "INTERRUPT_AOE_SHADOW", "APPLY_BLIND_AOE" }),
			list<string>({ "MAGICAL","SHADOW", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RECHARGE", PowerValue("RECHARGE", 8, 1, 99, false, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				pair<string, PowerValue>("DURATION_BLIND", PowerValue("DURATION_BLIND", 2, 0, 99, true, list<string>())),
				}),
				list<string>({ "BLINDFOE", }), SHADOWSPIKE_WAV);

		skillDefinitions["Chaos Storm"] = Skill("Chaos Storm", "Chaos Storm", "Umbromancy", SKILLICON_CHAOSSTORM, 10, 2, 10, "SINGLEFOE",
			list<string>({ "APPLY_Chaos Storm_WORLD", "MANABURN_ALLFOES_SHADOW"}),
			list<string>({ "MAGICAL","SHADOW", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Chaos Storm", PowerValue("DURATION_Chaos Storm", 8, 0, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				pair<string, PowerValue>("POWER_Chaos Storm", PowerValue("POWER_Chaos Storm", 10, 10, 10, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				pair<string, PowerValue>("MANABURN_ALLFOES_SHADOW", PowerValue("MANABURN_ALLFOES_SHADOW", 10, 0, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))), }),
				list<string>({ "MANADAMAGE" }), CHAOSSTORM_WAV);

		skillDefinitions["Night Fracture"] = Skill("Night Fracture", "Night Fracture", "Umbromancy", SKILLICON_NIGHTFRACTURE, 5, 0, 0, "SINGLEFOE",
			list<string>({ "MANABURN_SINGLE_SHADOW" }),
			list<string>({ "MAGICAL", "SHADOW" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("MANABURN_SINGLE_SHADOW", PowerValue("MANABURN_SINGLE_SHADOW", 10, 0, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))), }),
				list<string>({ "MANADAMAGE" }), NIGHTFRACTURE_WAV);

		skillDefinitions["Mug"] = Skill("Mug", "Mug", "Umbromancy", SKILLICON_MUG, 5, 0, 3, "SINGLEFOE",
			list<string>({ "MANASTEAL_SINGLE_SHADOW" }),
			list<string>({ "MAGICAL","SHADOW" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("MANASTEAL_SINGLE_SHADOW", PowerValue("MANASTEAL_SINGLE_SHADOW", 10, 0, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))), }),
				list<string>({ "MANADAMAGE" }), MUG_WAV);

		skillDefinitions["Steal Enchantment"] = Skill("Steal Enchantment", "Steal Enchantment", "Umbromancy", SKILLICON_STEALENCHANTMENT, 5, 0, 0, "SINGLEFOE",
			list<string>({ "STEALBOON_SINGLE_SHADOW", }),
			list<string>({ "MAGICAL","SHADOW" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RECHARGE", PowerValue("RECHARGE", 8, 1, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "REMOVEBOON" }), STEALENCHANTMENT_WAV);

		skillDefinitions["Charm Collapse"] = Skill("Charm Collapse", "Charm Collapse", "Umbromancy", SKILLICON_CHARMCOLLAPSE, 10, 0, 2, "SINGLEFOE",
			list<string>({ "APPLY_Charm Collapse_SINGLE", }),
			list<string>({ "MAGICAL", "SHADOW"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Charm Collapse", PowerValue("DURATION_Charm Collapse", 4, 0, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), CHARMCOLLAPSE_WAV);

		skillDefinitions["Stalked by Shadows"] = Skill("Stalked by Shadows", "Stalked by Shadows", "Umbromancy", SKILLICON_STALKEDBYSHADOWS, 15, 0, 8, "SINGLEFOE",
			list<string>({ "APPLY_Stalked by Shadows_SINGLE", }),
			list<string>({ "MAGICAL", "SHADOW", "ELITE"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Stalked by Shadows", PowerValue("DURATION_Stalked by Shadows", 3, 0, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), SHADOWONWALL_WAV);

		skillDefinitions["Marionette"] = Skill("Marionette", "Marionette", "Umbromancy", SKILLICON_MARIONETTE, 10, 0, 0, "SINGLEFOE",
			list<string>({ "COPYSPELL", }),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RECHARGE", PowerValue("RECHARGE", 8, 1, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), SHADOWONWALL_WAV);

		skillDefinitions["Shadow Puppet"] = Skill("Shadow Puppet", "Shadow Puppet", "Umbromancy", SKILLICON_SHADOWPUPPET, 10, 0, 0, "SINGLEFOE",
			list<string>({ "COPYPHYSICAL", }),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RECHARGE", PowerValue("RECHARGE", 8, 1, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), SHADOWONWALL_WAV);

		skillDefinitions["Psychic Pithing"] = Skill("Psychic Pithing", "Psychic Pithing", "Umbromancy", SKILLICON_PSYCHICPITHING, 10, 0, 5, "FOECASTINGASPELL",
			list<string>({ "INTERRUPT_SINGLE_SHADOW", "MANABURNIF?INTERRUPTED_SINGLE_SHADOW"}),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("MANABURN_SINGLE_SHADOW", PowerValue("MANABURN_SINGLE_SHADOW", 12, 1, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), NIGHTFRACTURE_WAV);

		skillDefinitions["Time Vortex"] = Skill("Time Vortex", "Time Vortex", "Umbromancy", SKILLICON_TIMEVORTEX, 25, 0, 2, "SINGLEFOE",
			list<string>({ "MANABURN_AOE_SHADOW" }),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("MANABURN_AOE_SHADOW", PowerValue("MANABURN_AOE_SHADOW", 12, 1, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), TIMEVORTEX_WAV);

		skillDefinitions["Mind Maze"] = Skill("Mind Maze", "Mind Maze", "Umbromancy", SKILLICON_MINDMAZE, 5, 0, 1, "SINGLEFOE",
			list<string>({ "APPLY_Mind Maze_SINGLE" }),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Mind Maze", PowerValue("DURATION_Mind Maze", 12, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				pair<string, PowerValue>("POWER_Mind Maze", PowerValue("POWER_Mind Maze", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), MINDMAZE_WAV);

		skillDefinitions["Blinded Eye"] = Skill("Blinded Eye", "Blinded Eye", "Umbromancy", SKILLICON_BLINDEDEYE, 5, 0, 1, "SINGLEFOE",
			list<string>({ "APPLY_BLIND_SINGLE" }),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_BLIND", PowerValue("DURATION_BLIND", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), BLINDEDEYE_WAV);

		skillDefinitions["Black Djinn's Breath"] = Skill("Black Djinn's Breath", "Black Djinn's Breath", "Umbromancy", SKILLICON_BLACKDJINNSBREATH, 20, 0, 5, "SINGLEFOE",
			list<string>({ "APPLY_WEAKENED_SINGLE", "APPLY_OVERWHELMED_SINGLE" }),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_WEAKENED", PowerValue("DURATION_WEAKENED", 3, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				pair<string, PowerValue>("DURATION_OVERWHELMED", PowerValue("DURATION_OVERWHELMED", 3, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), BLACKDJINNSBREATH_WAV);

		skillDefinitions["Wastrel's Comeuppance"] = Skill("Wastrel's Comeuppance", "Wastrel's Comeuppance", "Umbromancy", SKILLICON_WASTRELSCOMEUPPANCE, 5, 0, 5, "FOECASTINGASPELL",
			list<string>({ "INTERRUPT_SINGLE_SHADOW", "MANAHEALIF?INTERRUPTED_SELF_SHADOW" }),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("MANAHEAL_SELF_SHADOW", PowerValue("MANAHEAL_SELF_SHADOW", 20, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), WASTRELSCOMEUPPANCE_WAV);

		skillDefinitions["Petrifying Touch"] = Skill("Petrifying Touch", "Petrifying Touch", "Umbromancy", SKILLICON_PETRIFYINGTOUCH, 20, 0, 5, "SINGLEFOE",
			list<string>({ "APPLY_WEAKENED_SINGLE", "APPLY_DUSTY_SINGLE", "APPLY_CRIPPLED_SINGLE"}),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_WEAKENED", PowerValue("DURATION_WEAKENED", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				pair<string, PowerValue>("DURATION_DUSTY", PowerValue("DURATION_DUSTY", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				pair<string, PowerValue>("DURATION_CRIPPLED", PowerValue("DURATION_CRIPPLED", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), PETRIFYINGTOUCH_WAV);

		skillDefinitions["Rude Awakening"] = Skill("Rude Awakening", "Rude Awakening", "Umbromancy", SKILLICON_RUDEAWAKENING, 5, 0, 2, "FOECASTINGASPELL",
			list<string>({ "INTERRUPT_SINGLE_SHADOW"}),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DISABLEPOWER", PowerValue("DISABLEPOWER", 1, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), RUDEAWAKENING_WAV);

		skillDefinitions["Time Walk"] = Skill("Time Walk", "Time Walk", "Umbromancy", SKILLICON_TIMEWALK, 25, 0, 5, "SELF",
			list<string>({ "APPLY_Time Walk_SELF" }),
			list<string>({ "MAGICAL", "SHADOW","ELITE",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Time Walk", PowerValue("DURATION_Time Walk", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), TIMEVORTEX_WAV);

		skillDefinitions["Underworld Dreams"] = Skill("Underworld Dreams", "Underworld Dreams", "Umbromancy", SKILLICON_UNDERWORLDDREAMS, 25, 0, 2, "SINGLEFOE",
			list<string>({ "APPLY_Underworld Dreams_ALLFOES" }),
			list<string>({ "MAGICAL", "SHADOW", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Underworld Dreams", PowerValue("DURATION_Underworld Dreams", 6, 0, 999, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				}),
				list<string>({ "CURSEFOE" }), TIMEVORTEX_WAV);

		// ARMS
		skillDefinitions["Fine Strike"] = Skill("Fine Strike", "Fine Strike", "Arms", SKILLICON_FINESTRIKE, 5, 0, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL", "ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("CRITICALBOOST", PowerValue("CRITICALBOOST", 40, 0, 100, true, list<string>({ "STRENGTH", "1H Weapon MasteryBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), -1);

		skillDefinitions["Rotation Blade"] = Skill("Rotation Blade", "Rotation Blade", "Arms", SKILLICON_ROTATIONBLADE, 5, 0, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_ALLFOES_PHYSICAL",}),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("RECHARGE",PowerValue("RECHARGE",5,2,999,false,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), ROTATIONBLADE_WAV);

		skillDefinitions["Debilitating Smash"] = Skill("Debilitating Smash", "Debilitating Smash", "Arms", SKILLICON_DEBILITATINGSMASH, 10, 0, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL","APPLY_WEAKENED_SINGLE"}),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",20,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("DURATION_WEAKENED", PowerValue("DURATION_WEAKENED", 5, 0, 15, true, list<string>({ "STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), DEBILITATINGSMASH_WAV);

		skillDefinitions["Clobber"] = Skill("Clobber", "Clobber", "Arms", SKILLICON_CLOBBER, 30, 0, 5, "FOECASTINGASPELL",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL","APPLY_CONCUSSED_SINGLE","INTERRUPT_SINGLE_PHYSICAL"}),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",12,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("DURATION_CONCUSSED", PowerValue("DURATION_CONCUSSED", 1, 0, 15, true, list<string>({ "STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), CLOBBER_WAV);

		skillDefinitions["Cleave Armour"] = Skill("Cleave Armour", "Cleave Armour", "Arms", SKILLICON_CLEAVEARMOUR, 5, 0, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL","APPLY_Cleave Armour_SINGLE",}),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("DURATION_Cleave Armour", PowerValue("DURATION_Cleave Armour", 5, 0, 15, true, list<string>({ "STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("POWER_Cleave Armour", PowerValue("POWER_Cleave Armour", 10, 0, 99, true, list<string>({ "STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), CLEAVEARMOUR_WAV);

		skillDefinitions["Knee Crack"] = Skill("Knee Crack", "Knee Crack", "Arms", SKILLICON_KNEECRACK, 10, 0, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL","APPLY_CRIPPLED_SINGLE", }),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",12,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("DURATION_CRIPPLED", PowerValue("DURATION_CRIPPLED", 5, 0, 15, true, list<string>({ "STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), CLEAVEARMOUR_WAV);

		skillDefinitions["Bulldoze"] = Skill("Bulldoze", "Bulldoze", "Arms", SKILLICON_BULLDOZE, 5, 1, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",35,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), BULLDOZE_WAV);

		skillDefinitions["On My Target!"] = Skill("On My Target!", "On My Target!", "Arms", SKILLICON_ONMYTARGET, 5, 0, 5, "SINGLEFOE",
			list<string>({ "APPLY_On My Target!_SINGLE", }),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_On My Target!",PowerValue("DURATION_On My Target!",5,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("POWER_On My Target!",PowerValue("POWER_On My Target!",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), ONMYTARGET_WAV);

		skillDefinitions["Glass Sword"] = Skill("Glass Sword", "Glass Sword", "Arms", SKILLICON_GLASSSWORD, 5, 0, 5, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL","APPLY_BLEEDING_SINGLE"}),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",12,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("DURATION_BLEEDING",PowerValue("DURATION_BLEEDING",5,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), CLEAVEARMOUR_WAV);

		skillDefinitions["Hack"] = Skill("Hack", "Hack", "Arms", SKILLICON_HACK, 0, 0, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL","INTERRUPT_SINGLE_PHYSICAL" }),
			list<string>({ "PHYSICAL", "ATTACK", "ELITE", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",11,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), HACK_WAV);

		skillDefinitions["Reckless Swing"] = Skill("Reckless Swing", "Reckless Swing", "Arms", SKILLICON_RECKLESSSWING, 5, 0, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_ALLFOES_PHYSICAL","APPLY_Reckless Swing_SELF" }),
			list<string>({ "PHYSICAL","ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("DURATION_Reckless Swing",PowerValue("DURATION_Reckless",3,3,3,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), RECKLESSSWING_WAV);

		skillDefinitions["Smuggler's Gambit"] = Skill("Smuggler's Gambit", "Smuggler's Gambit", "Arms", SKILLICON_SMUGGLERSGAMBIT, 5, 0, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL","ATTACK", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",25,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), SMUGGLERSGAMBIT_WAV);

		skillDefinitions["Magebane Strike"] = Skill("Magebane Strike", "Magebane Strike", "Arms", SKILLICON_MAGEBANESTRIKE, 5, 0, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL", "ATTACK", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("BONUS",PowerValue("BONUS",7,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), MAGEBANESTRIKE_WAV);

		skillDefinitions["Crazed Chop"] = Skill("Crazed Chop", "Crazed Chop", "Arms", SKILLICON_CRAZEDCHOP, 5, 0, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL", "ATTACK", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("LOWER",PowerValue("LOWER",5,0,998,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("UPPER",PowerValue("UPPER",40,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), HACK_WAV);

		skillDefinitions["Skewer"] = Skill("Skewer", "Skewer", "Arms", SKILLICON_SKEWER, 5, 0, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL","APPLY_BLEEDING_SINGLE" }),
			list<string>({ "PHYSICAL", "ATTACK", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("DURATION_BLEEDING",PowerValue("DURATION_BLEEDING",8,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), SERRATEDSTRIKE_WAV);

		skillDefinitions["Dragon Smash"] = Skill("Dragon Smash", "Dragon Smash", "Arms", SKILLICON_DRAGONSMASH, 10, 0, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL","APPLY_OVERWHELMED_SINGLE" }),
			list<string>({ "PHYSICAL","ATTACK", "ELITE",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",20,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("DURATION_OVERWHELMED",PowerValue("DURATION_OVERWHELMED",3,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), DRAGONSMASH_WAV);

		skillDefinitions["Weaponsmithing"] = Skill("Weaponsmithing", "Weaponsmithing", "Arms", SKILLICON_WEAPONSMITHING, 10, 0, 2, "SELF",
			list<string>({ "APPLY_Weaponsmithing_SELF",}),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Weaponsmithing",PowerValue("DURATION_Weaponsmithing",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("POWER_Weaponsmithing",PowerValue("POWER_Weaponsmithing",5,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), WEAPONSMITHING_WAV);

		skillDefinitions["Excommunicative Assault"] = Skill("Excommunicative Assault", "Excommunicative Assault", "Arms", SKILLICON_EXCOMMUNICATIVEASSAULT, 5, 1, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_ALLFOES_PHYSICAL",}),
			list<string>({ "PHYSICAL","ATTACK", "ELITE"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				pair<string, PowerValue>("bonus",PowerValue("bonus",50,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), EXCOMMUNICATIVEASSAULT_WAV);

		skillDefinitions["Mourning Edge"] = Skill("Mourning Edge", "Mourning Edge", "Arms", SKILLICON_MOURNINGEDGE, 5, 0, 3, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL", "ATTACK" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",20,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), MOURNINGEDGE_WAV);

		skillDefinitions["Exemplar's Posture"] = Skill("Exemplar's Posture", "Exemplar's Posture", "Arms", SKILLICON_EXEMPLARSPOSTURE, 5, 0, 3, "SELF",
			list<string>({ "APPLY_Exemplar's Posture_SELF", }),
			list<string>({ "PHYSICAL" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Exemplar's Posture",PowerValue("DURATION_Exemplar's Posture",5,0,999,true,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), WEAPONSMITHING_WAV);

		// WAYFARING
		skillDefinitions["Gentleman's Riposte"] = Skill("Gentleman's Riposte", "Gentleman's Riposte", "Wayfaring", SKILLICON_GENTLEMANSRIPOSTE, 5, 0, 5, "SELF",
			list<string>({ "APPLY_Gentleman's Riposte_SELF", }),
			list<string>({ "PHYSICAL","ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Gentleman's Riposte", PowerValue("DURATION_Gentleman's Riposte", 2, 0, 15, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))), }),
				list<string>({ "ENCHANTSELF" }), GENTLEMANRIPOSTE_WAV);

		skillDefinitions["Trickblade"] = Skill("Trickblade", "Trickblade", "Wayfaring", SKILLICON_TRICKBLADE, 5, 0, 1, "SELF",
			list<string>({ "APPLY_Trickblade_SELF", }),
			list<string>({ "PHYSICAL"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Trickblade", PowerValue("DURATION_Trickblade", 5, 0, 99, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))), }),
				list<string>({ "ENCHANTSELF" }), TRICKBLADE_WAV);

		skillDefinitions["Don't Give Up!"] = Skill("Don't Give Up!", "Don't Give Up!", "Wayfaring", SKILLICON_DONTGIVEUP, 20, 0, 0, "DEADPARTYMEMBER",
			list<string>({ "RESURRECT_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL", "ELITE"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RESURRECT_SINGLE_PHYSICAL", PowerValue("RESURRECT_SINGLE_PHYSICAL", 10, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				pair<string, PowerValue>("RECHARGE",PowerValue("RECHARGE",12,5,12,false,list<string>({"STRENGTH", "ArmsBOOST"}))),
				}),
				list<string>({ "RESURRECT" }), REVITALISE_WAV);

		skillDefinitions["Knight Vision"] = Skill("Knight Vision", "Knight Vision", "Wayfaring", SKILLICON_KNIGHTVISION, 5, 0, 0, "SELF",
			list<string>({ "APPLY_Knight Vision_SELF", }),
			list<string>({ "PHYSICAL", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Knight Vision", PowerValue("DURATION_Knight", 10, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF" }), KNIGHTVISION_WAV);

		skillDefinitions["Bramble Cloak"] = Skill("Bramble Cloak", "Bramble Cloak", "Wayfaring", SKILLICON_BRAMBLECLOAK, 5, 0, 1, "SELF",
			list<string>({ "APPLY_Bramble Cloak_SELF", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Bramble Cloak", PowerValue("DURATION_Bramble Cloak", 10, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				pair<string, PowerValue>("POWER_Bramble Cloak", PowerValue("POWER_Bramble Cloak", 20, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF" }), BRAMBLECLOAK_WAV);

		skillDefinitions["Shield of the Messenger"] = Skill("Shield of the Messenger", "Shield of the Messenger", "Wayfaring", SKILLICON_SHIELDOFTHEMESSENGER, 10, 0, 5, "SELF",
			list<string>({ "APPLY_Shield of the Messenger_SELF", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Shield of the Messenger", PowerValue("DURATION_Shield of the Messenger", 5, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF" }), TRICKBLADE_WAV);

		skillDefinitions["I Shall Take Care of This!"] = Skill("I Shall Take Care of This!", "I Shall Take Care of This!", "Wayfaring", SKILLICON_ISHALLTAKECAREOFTHIS, 5, 0, 5, "SELF",
			list<string>({ "APPLY_I Shall Take Care of This!_SELF", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("POWER_I Shall Take Care of This!", PowerValue("POWER_I Shall Take Care of This!", 7, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				pair<string, PowerValue>("DURATION_I Shall Take Care of This!", PowerValue("DURATION_I Shall Take Care of This!", 5, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF" }), ISHALLTAKECAREOFTHIS_WAV);

		skillDefinitions["Song of Angels"] = Skill("Song of Angels", "Song of Angels", "Wayfaring", SKILLICON_SONGOFANGELS, 5, 0, 1, "SELF",
			list<string>({ "MANAHEAL_SINGLE_PHYSICAL", "LIFEHEAL_SINGLE_PHYSICAL" }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("MANAHEAL_SINGLE_PHYSICAL", PowerValue("MANAHEAL_SINGLE_PHYSICAL", 8, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				pair<string, PowerValue>("LIFEHEAL_SINGLE_PHYSICAL", PowerValue("LIFEHEAL_SINGLE_PHYSICAL", 8, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				}),
				list<string>({ "HEALSELF" }), HEALWOUNDS_WAV);

		skillDefinitions["Lord's Authority"] = Skill("Lord's Authority", "Lord's Authority", "Wayfaring", SKILLICON_LORDSAUTHORITY, 5, 0, 15, "SELF",
			list<string>({ "APPLY_Lord's Authority_SELF", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Lord's Authority", PowerValue("DURATION_Lord's Authority", 8, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF" }), LORDSAUTHORITY_WAV);

		skillDefinitions["Bailiff's Blade"] = Skill("Bailiff's Blade", "Bailiff's Blade", "Wayfaring", SKILLICON_BAILIFFSBLADE, 10, 0, 15, "SELF",
			list<string>({ "APPLY_Bailiff's Blade_SELF", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Bailiff's Blade", PowerValue("DURATION_Bailiff's Blade", 6, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				pair<string, PowerValue>("POWER_Bailiff's Blade", PowerValue("POWER_Bailiff's Blade", 2, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF" }), BAILIFFSBLADE_WAV);

		skillDefinitions["Fight the Pain!"] = Skill("Fight the Pain!", "Fight the Pain!", "Wayfaring", SKILLICON_FIGHTTHEPAIN, 5, 0, 10, "SELF",
			list<string>({ "APPLY_Fight the Pain!_SELF", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Fight the Pain!", PowerValue("DURATION_Fight the Pain!", 10, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				pair<string, PowerValue>("POWER_Fight the Pain!", PowerValue("POWER_Fight the Pain!", 5, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF" }), BLOODGIFT_WAV);

		skillDefinitions["No One Said You Could Touch!"] = Skill("No One Said You Could Touch!", "No One Said You Could Touch!", "Wayfaring", SKILLICON_NOONESAIDYOUCOULDTOUCH, 15, 0, 10, "SELF",
			list<string>({ "APPLY_No One Said You Could Touch!_ALLALLIES", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_No One Said You Could Touch!", PowerValue("DURATION_No One Said You Could Touch!", 10, 0, 100, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF" }), NOONESAIDYOUCOULDTOUCH_WAV);

		skillDefinitions["Fencer's Flash"] = Skill("Fencer's Flash", "Fencer's Flash", "Wayfaring", SKILLICON_FENCERSFLASH, 5, 0, 5, "SELF",
			list<string>({ "APPLY_Fencer's Flash_SELF", }),
			list<string>({ "PHYSICAL",}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Fencer's Flash", PowerValue("DURATION_Fencer's Flash", 5, 0, 15, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))), }),
				list<string>({ "ENCHANTSELF" }), GENTLEMANRIPOSTE_WAV);

		skillDefinitions["You're Worthless!"] = Skill("You're Worthless!", "You're Worthless!", "Wayfaring", SKILLICON_YOUREWORTHLESS, 5, 0, 5, "SINGLEFOE",
			list<string>({ "APPLY_WEAKENED_SINGLE", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_WEAKENED", PowerValue("DURATION_WEAKENED", 5, 0, 15, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))), }),
				list<string>({ "ENCHANTSELF" }), YOUREWORTHLESS_WAV);

		skillDefinitions["Go On Without Me!"] = Skill("Go On Without Me!", "Go On Without Me!", "Wayfaring", SKILLICON_GOONWITHOUTME, 20, 0, 10, "SELFIF<90LIFE",
			list<string>({ "APPLY_Go On Without Me!_ALLOTHERALLIES", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Go On Without Me!", PowerValue("DURATION_Go On Without Me!", 2, 0, 15, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))), }),
				list<string>({ "ENCHANTSELF" }), GOONWITHOUTME_WAV);

		skillDefinitions["You're Revolting!"] = Skill("You're Revolting!", "You're Revolting!", "Wayfaring", SKILLICON_YOUREREVOLTING, 25, 0, 10, "SINGLEFOE",
			list<string>({ "APPLY_OVERWHELMED_SINGLE", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_OVERWHELMED", PowerValue("DURATION_OVERWHELMED", 2, 0, 15, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))), }),
				list<string>({ "CURSEFOE" }), YOUREREVOLTING_WAV);

		// PYROMANCY
		skillDefinitions["Brilliant Spark"] = Skill("Brilliant Spark", "Brilliant Spark", "Pyromancy", SKILLICON_BRILLIANTSPARK, 20, 0, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_FIRE", "APPLY_BURNING_SINGLE" }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_FIRE", PowerValue("DAMAGE_SINGLE_FIRE", 25, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DURATION_BURNING", PowerValue("DURATION_BURNING", 3, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", "APPLY_BURNING_SINGLE" }), BRILLIANTSPARK_WAV);

		skillDefinitions["Rageflame"] = Skill("Rageflame", "Rageflame", "Pyromancy", SKILLICON_RAGEFLAME, 25, 1, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_ALLFOES_FIRE", "MANAHEAL_ALLFOES_FIRE" }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_ALLFOES_FIRE", PowerValue("DAMAGE_ALLFOES_FIRE", 25, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("MANAHEAL_ALLFOES_FIRE", PowerValue("MANAHEAL_ALLFOES_FIRE", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), RAGEFLAME_WAV);

		skillDefinitions["Summer Strike"] = Skill("Summer Strike", "Summer Strike", "Pyromancy", SKILLICON_SUMMERSTRIKE, 30, 2, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_FIRE", }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_FIRE", PowerValue("DAMAGE_SINGLE_FIRE", 95, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), SUMMERSTRIKE_WAV);

		skillDefinitions["Wildfire"] = Skill("Wildfire", "Wildfire", "Pyromancy", SKILLICON_WILDFIRE, 20, 0, 0, "SINGLEFOE",
			list<string>({ "APPLY_Wildfire_SINGLE", "DAMAGE_SINGLE_FIRE"}),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Wildfire", PowerValue("DURATION_Wildfire", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER_Wildfire", PowerValue("POWER_Wildfire", 15, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DAMAGE_SINGLE_FIRE", PowerValue("DAMAGE_SINGLE_FIRE", 20, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "CURSEFOE", }), SUMMERSTRIKE_WAV);

		skillDefinitions["Cataclysm"] = Skill("Cataclysm", "Cataclysm", "Pyromancy", SKILLICON_CATACLYSM, 40, 2, 10, "SINGLEFOE",
			list<string>({ "APPLY_Cataclysm_WORLD", "DAMAGE_ALLFOES_FIRE", }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("Duration_Cataclysm", PowerValue("Duration_Cataclysm Storm", 10, 10, 10, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER_Cataclysm", PowerValue("POWER_Cataclysm", 18, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DAMAGE_ALLFOES_FIRE", PowerValue("DAMAGE_ALLFOES_FIRE", 18, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), CATACLYSM_WAV);

		skillDefinitions["Ring of Ash"] = Skill("Ring of Ash", "Ring of Ash", "Pyromancy", SKILLICON_RINGOFASH, 5, 0, 0, "SELF",
			list<string>({ "APPLY_Ring of Ash_SELF", }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Ring of Ash", PowerValue("DURATION_Ring of Ash", 10, 10, 10, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER_Ring of Ash", PowerValue("POWER_Ring of Ash", 1, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("charges", PowerValue("charges", 1, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), RINGOFASH_WAV);

		// TERRAMANCY
		skillDefinitions["Stone Strike"] = Skill("Stone Strike", "Stone Strike", "Terramancy", SKILLICON_STONESTRIKE, 20, 0, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_EARTH", "APPLY_BLEEDINGIF?TARGETHASDUSTY_SINGLE" }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_EARTH", PowerValue("DAMAGE_SINGLE_EARTH", 25, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DURATION_BLEEDING", PowerValue("DURATION_BLEEDING", 3, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE"}), STONESTRIKE_WAV);

		skillDefinitions["Stone Curse"] = Skill("Stone Curse", "Stone Curse", "Terramancy", SKILLICON_STONECURSE, 5, 0, 0, "SINGLEFOE",
			list<string>({"APPLY_Stone Curse_SINGLE" }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Stone Curse", PowerValue("DURATION_Stone Curse", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), STONECURSE_WAV);

		skillDefinitions["Basalt Bastion"] = Skill("Basalt Bastion", "Basalt Bastion", "Terramancy", SKILLICON_BASALTBASTION, 10, 0, 5, "SELF",
			list<string>({ "APPLY_Basalt Bastion_SINGLE" }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Basalt Bastion", PowerValue("DURATION_Basalt Bastion", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "EARTHBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "ENCHANTSELF", }), BASALTBASTION_WAV);

		// HYDROMANCY
		skillDefinitions["Healing Rain"] = Skill("Healing Rain", "Healing Rain", "Hydromancy", SKILLICON_HEALINGRAIN, 25, 1, 10, "SELF",
			list<string>({ "APPLY_Healing Rain_WORLD", "LIFEHEAL_AOE_WATER"}),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Healing Rain", PowerValue("DURATION_Healing Rain", 10, 0, 10, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER_Healing Rain", PowerValue("POWER_Healing Rain", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("LIFEHEAL_AOE_WATER", PowerValue("LIFEHEAL_AOE_WATER", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "HEALALLY", "HEALSELF" }), BASALTBASTION_WAV);

		skillDefinitions["Winter Blast"] = Skill("Winter Blast", "Winter Blast", "Hydromancy", SKILLICON_WINTERBLAST, 30, 1, 3, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_COLD", "APPLY_FROZEN_SINGLE" }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_COLD", PowerValue("DAMAGE_SINGLE_COLD", 50, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DURATION_FROZEN", PowerValue("DURATION_FROZEN", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), WINTERBLAST_WAV);

		skillDefinitions["Vapour Blade"] = Skill("Vapour Blade", "Vapour Blade", "Hydromancy", SKILLICON_VAPOURBLADE, 30, 1, 4, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_COLD", }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_COLD", PowerValue("DAMAGE_SINGLE_COLD", 42, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), VAPOURBLADE_WAV);

		skillDefinitions["Light from the Other Side"] = Skill("Light from the Other Side", "Light from the Other Side", "Hydromancy", SKILLICON_LFROMTHEOTHERS, 15, 0, 3, "SINGLEALLY",
			list<string>({ "REMOVEBANE_SINGLE", "LIFEHEAL_SINGLE_COLD", "MANAHEAL_SELF_COLD"}),
			list<string>({ "MAGICAL", "ELITE"}),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFEHEAL_SINGLE_COLD", PowerValue("LIFEHEAL_SINGLE_COLD", 100, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("MANAHEAL_SELF_COLD", PowerValue("MANAHEAL_SELF_COLD", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), LFROMTHEOTHERS_WAV);

		skillDefinitions["Ice Storm"] = Skill("Ice Storm", "Ice Storm", "Hydromancy", SKILLICON_ICESTORM, 30, 2, 10, "SINGLEFOE",
			list<string>({ "APPLY_Ice Storm_WORLD", "DAMAGE_ALLFOES_COLD", }),
			list<string>({ "MAGICAL", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("Duration_Ice Storm", PowerValue("Duration_Ice Storm", 10, 10, 10, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("POWER_Ice Storm", PowerValue("POWER_Ice Storm", 23, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DAMAGE_ALLFOES_COLD", PowerValue("DAMAGE_ALLFOES_COLD", 23, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), ICESTORM_WAV);

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
			equipmentDefinitions["Tome of " + skill.uniqueID] = Equipment(*this, "Tome of " + skill.uniqueID, "Tome", UNIMPLEMENTED_IMAGE, {}, colour, price);
			for (auto const& [language, value] : strings) {
				wstring name = strings[language]["Unique Item Strings"]["TOMEOF"] + strings[language]["Skill Names"][skill.uniqueID];
				wstring description = WSReplace(strings[language]["Unique Item Strings"]["TEACHES"], L"$REPLACE$", strings[language]["Skill Names"][skill.uniqueID]);
				strings[language]["Item Names"][tomeOf + skill.uniqueID] = name;
				strings[language]["Item Descriptions"][tomeOf + skill.uniqueID] = description;
			}
		}

		// WEAPONS
		equipmentDefinitions["Withered Secespita"] = Equipment(*this, "Withered Secespita", "Weapon", CODEXPAGE_SECESPITA, List<Combat::Effect>({
			Combat::Effect("PIETY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Suero's Blade"] = Equipment(*this, "Suero's Blade", "Weapon", CODEXPAGE_SWORD1, List<Combat::Effect>({
			Combat::Effect("STRENGTH",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Blades of House JaqMaq"] = Equipment(*this, "Blades of House JaqMaq", "Weapon", CODEXPAGE_SCIMITARS, List<Combat::Effect>({
			Combat::Effect("AGILITY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Tibetan Tie Bian"] = Equipment(*this, "Tibetan Tie Bian", "Weapon", CODEXPAGE_TIEBAN1, List<Combat::Effect>({
			Combat::Effect("ENERGYREGENPLUS",5.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Roger Bacon's Quill"] = Equipment(*this, "Roger Bacon's Quill", "Weapon", CODEXPAGE_QUILL, List<Combat::Effect>({
			Combat::Effect("INTELLIGENCE",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Licinia Eucharis' Wand"] = Equipment(*this, "Licinia Eucharis' Wand", "Weapon", CODEXPAGE_WAND1, List<Combat::Effect>({
			Combat::Effect("PIETY",1.1f,true,true),
			}), "EQUIPMENTBLUE", 280);
		equipmentDefinitions["William's Left-Hand Sword"] = Equipment(*this, "William's Left-Hand Sword", "Weapon", CODEXPAGE_SWORD2, List<Combat::Effect>({
			Combat::Effect("STRENGTH",1.1f,true,true),
			Combat::Effect("BLOODBOOST",2.0f,true,false),
			}), "EQUIPMENTBLUE", 500);
		equipmentDefinitions["William's Right-Hand Sword"] = Equipment(*this, "William's Right-Hand Sword", "Weapon", CODEXPAGE_SWORD2, List<Combat::Effect>({
			Combat::Effect("STRENGTH",1.1f,true,true),
			Combat::Effect("ArmsBOOST",2.0f,true,false),
			}), "EQUIPMENTBLUE", 500);

		// ARMOUR
		equipmentDefinitions["Vatican Vestiments"] = Equipment(*this, "Vatican Vestiments", "Armour", CODEXPAGE_VESTIMENTS, List<Combat::Effect>({
			Combat::Effect("HOLYBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Martin's Cloak"] = Equipment(*this, "Martin's Cloak", "Armour", CODEXPAGE_CLOAK1, List<Combat::Effect>({
			Combat::Effect("PHYSICALARMOUR",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Alhambran Tunic"] = Equipment(*this, "Alhambran Tunic", "Armour", CODEXPAGE_HAREM, List<Combat::Effect>({
			Combat::Effect("SHADOWBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Insulating Gloves"] = Equipment(*this, "Insulating Gloves", "Armour", CODEXPAGE_GLOVES1, List<Combat::Effect>({
			Combat::Effect("ARMOURVSELECTRIC",5.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Ming Theatre Costume"] = Equipment(*this, "Ming Theatre Costume", "Armour", CODEXPAGE_MING, List<Combat::Effect>({
			Combat::Effect("BLOODBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);

		// ACCESSORIES
		equipmentDefinitions["Cross of St Jeanne-Marie"] = Equipment(*this, "Cross of St Jeanne-Marie", "Accessory", CODEXPAGE_CROSS1, List<Combat::Effect>({
			Combat::Effect("PIETY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Matteo Carreri's Locket"] = Equipment(*this, "Matteo Carreri's Locket", "Accessory", CODEXPAGE_LOCKET1, List<Combat::Effect>({
			Combat::Effect("WAYFARINGBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Theoricae Novae Planetarum"] = Equipment(*this, "Theoricae Novae Planetarum", "Accessory", CODEXPAGE_BOOK1, List<Combat::Effect>({
			Combat::Effect("INTELLIGENCE",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Lunyu Page Fragment"] = Equipment(*this, "Lunyu Page Fragment", "Accessory", CODEXPAGE_BOOK2, List<Combat::Effect>({
			Combat::Effect("UNHOLYBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Mask of House JaqMaq"] = Equipment(*this, "Mask of House JaqMaq", "Accessory", CODEXPAGE_MASK1, List<Combat::Effect>({
			Combat::Effect("AGILITY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Joan of Arc's Necklace"] = Equipment(*this, "Joan of Arc's Necklace", "Accessory", CODEXPAGE_NECKLACE1, List<Combat::Effect>({
			Combat::Effect("INTELLIGENCE",5.0f,true,true),
			Combat::Effect("FIREBOOST",1.5f,true,false),
			Combat::Effect("SELFIMMOLATE",0.0f,true,true),
			}), "ELITESKILLYELLOW", 9999);
		equipmentDefinitions["The Eyes of St Lucy"] = Equipment(*this, "The Eyes of St Lucy", "Accessory", CODEXPAGE_EYE1, List<Combat::Effect>({
			Combat::Effect("HOLYBOOST",0.5f,true,false),
			Combat::Effect("PIETY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 150);
		equipmentDefinitions["al-Hajar al-Aswad"] = Equipment(*this, "al-Hajar al-Aswad", "Accessory", CODEXPAGE_ROCK1, List<Combat::Effect>({
			Combat::Effect("HOLYBOOST",1.3f,true,false),
			Combat::Effect("UNHOLYBOOST",1.3f,true,false),
			}), "ELITESKILLYELLOW", 9999);
	}
	void defineAllCombatants() {
		//DEBUG
		definedCombatants["Tomes"] = Combatant("Tomes", "Tomes", Map<string, int>({}), Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("SadBag", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("SadBag", "COMBAT_FRONT")),
					})),
					pair<string, Map<string, string>>("Effects", Map<string, string>({})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_WAIT"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
				}));

		definedCombatants["SadBag"] = Combatant("SadBag", "SadBag", Map<string, int>({
				pair<string, int>("PIETY", 100),
			}),
			Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("SadBag", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("SadBag", "COMBAT_FRONT")),
					})),
					pair<string, Map<string, string>>("Effects", Map<string, string>({
						pair<string, string>("Fragile", "1"),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_WAIT"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
				}));
		definedCombatants["DebugWoman"] = Combatant("DebugWoman", "DebugWoman", Map<string, int>({
				pair<string, int>("PIETY", 20),
			}),
			Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedNoblewoman", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedNoblewoman", "COMBAT_FRONT")),
					})),
					pair<string, Map<string, string>>("Effects", Map<string, string>({
						//pair<string, string>("BLIND", "999"),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_WAIT"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
				}));


		// SUMMONS
		definedCombatants["Skeleton Warrior"] = Combatant("Skeleton Warrior", "Skeleton Warrior", {},
			Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("Skeleton Warrior", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("Skeleton Warrior", "COMBAT_FRONT")),
					})),
					pair<string, Map<string, string>>("Effects", Map<string, string>({
						pair<string, string>("UNDEAD", "1"),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_ATTACK"),
						})),
				}));

		definedCombatants["Mound of Leeches"] = Combatant("Mound of Leeches", "Mound of Leeches", {},
			Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("Mound of Leeches", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("Mound of Leeches", "COMBAT_FRONT")),
					})),
					pair<string, Map<string, string>>("Effects", Map<string, string>({
						
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_LEECHSKILL"),
						pair<string, string>("6", "DEFAULT_LEECHSKILL"),
						})),
				}));

		// ALLIES
		definedCombatants["Father Michelet"] = Combatant("Father Michelet", "Father Michelet",
			Map<string, int>({
				pair<string, int>("AGILITY", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("Father Michelet", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("Father Michelet", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "Strength of Reason"),
						pair<string, string>("2", "Heal Wounds"),
						pair<string, string>("3", "Laying of Hands"),
						})),
					}));

		// ENRAGED
		definedCombatants["EnragedVilomah"] = Combatant("EnragedVilomah", "EnragedVilomah",
			Map<string, int>({
				pair<string, int>("AGILITY", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedVilomah", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedVilomah", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "Serrated Strike"),
						})),
					}));
		definedCombatants["EnragedPriest"] = Combatant("EnragedPriest", "EnragedPriest",
			Map<string, int>({
				pair<string, int>("PIETY", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedPriest", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedPriest", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "Heal Wounds"),
						})),
					}));
		definedCombatants["EnragedVagrant"] = Combatant("EnragedVagrant", "EnragedVagrant",
			Map<string, int>({
				pair<string, int>("INTELLIGENCE", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedVagrant", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedVagrant", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "Atrophy"),
						})),
					}));
		definedCombatants["EnragedMagician"] = Combatant("EnragedMagician", "EnragedMagician",
			Map<string, int>({
				pair<string, int>("INTELLIGENCE", 2),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedMagician", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedMagician", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "Brilliant Spark"),
						pair<string, string>("2", "Heatwave"),
						})),
					}));
		definedCombatants["EnragedNe'erDoWell"] = Combatant("EnragedNe'erDoWell", "EnragedNe'erDoWell",
			Map<string, int>({
				pair<string, int>("STRENGTH", 2),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedNe'erDoWell", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedNe'erDoWell", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "DEFAULT_ATTACK"),
						pair<string, string>("2", "Mug"),
						pair<string, string>("3", "Natural Stab"),
						})),
					}));
		definedCombatants["EnragedDeaconess"] = Combatant("EnragedDeaconess", "EnragedDeaconess",
			Map<string, int>({
				pair<string, int>("PIETY", 2),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedDeaconess", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedDeaconess", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "Erase Evil"),
						pair<string, string>("2", "Overrule"),
						})),
					}));
		definedCombatants["EnragedNoblewoman"] = Combatant("EnragedNoblewoman", "EnragedNoblewoman",
			Map<string, int>({
				pair<string, int>("PIETY", 2),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedNoblewoman", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedNoblewoman", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "Sandstorm"),
						pair<string, string>("2", "Stone Strike"),
						})),
					}));
		definedCombatants["EnragedButcher"] = Combatant("EnragedButcher", "EnragedButcher",
			Map<string, int>({
				pair<string, int>("STRENGTH", 3),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedButcher", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedButcher", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "DEFAULT_ATTACK"),
						pair<string, string>("2", "Bulldoze"),
						})),
					}));
		definedCombatants["EnragedRider"] = Combatant("EnragedRider", "EnragedRider",
			Map<string, int>({
				pair<string, int>("STRENGTH", 20),
				pair<string, int>("INTELLIGENCE", 20),
				pair<string, int>("VITALITY", 20),
				pair<string, int>("PIETY", 20),
				pair<string, int>("AGILITY", 20),
				pair<string, int>("LUCK", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("WIlliamDeVaines", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("WIlliamDeVaines", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "Blade of Blood"),
						pair<string, string>("2", "Rotation Blade"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
					}));
		definedCombatants["FakeRider"] = Combatant("EnragedRider", "EnragedRider",
			Map<string, int>({}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("WIlliamDeVaines", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("WIlliamDeVaines", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "DEFAULT_WAIT"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
					}));

	 // SHADOW
		definedCombatants["DarkAngela"] = Combatant("DarkAngela", "DarkAngela",
			Map<string, int>({
				pair<string, int>("PIETY", 1),
				pair<string, int>("INTELLIGENCE", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("Dark Angela Fleuret", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("Dark Angela Fleuret", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_ATTACK"),
						pair<string, string>("1", "Heal Wounds"),
						pair<string, string>("5", "Heavenstrike"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
					}));

		definedCombatants["DarkTianshun"] = Combatant("DarkTianshun", "DarkTianshun",
			Map<string, int>({
				pair<string, int>("PIETY", 1),
				pair<string, int>("INTELLIGENCE", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("Dark Tianshun Song", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("Dark Tianshun Song", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_ATTACK"),
						pair<string, string>("1", "Drain Life"),
						pair<string, string>("5", "Animate Skeleton Warrior"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
					}));

		definedCombatants["DarkOlyver"] = Combatant("DarkOlyver", "DarkOlyver",
			Map<string, int>({
				pair<string, int>("PIETY", 1),
				pair<string, int>("INTELLIGENCE", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("Dark Olyver Sumner", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("Dark Olyver Sumner", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_ATTACK"),
						pair<string, string>("1", "Rainstorm"),
						pair<string, string>("5", "Plasma Pulse"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
					}));

		definedCombatants["DarkHernando"] = Combatant("DarkHernando", "DarkHernando",
			Map<string, int>({
				pair<string, int>("STRENGTH", 1),
				pair<string, int>("AGILITY", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("Dark Hernando Pizarro", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("Dark Hernando Pizarro", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_ATTACK"),
						pair<string, string>("1", "Fine Strike"),
						pair<string, string>("5", "Gentleman's Riposte"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
					}));

		definedCombatants["DarkGihat"] = Combatant("DarkGihat", "DarkGihat",
			Map<string, int>({
				pair<string, int>("AGILITY", 3),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("Dark Gihat al-Din Jaqmaq", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("Dark Gihat al-Din Jaqmaq", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "DEFAULT_ATTACK"),
						pair<string, string>("1", "Doublestrike"),
						pair<string, string>("5", "Shadow Spike"),
						pair<string, string>("6", "DEFAULT_WAIT"),
						})),
					}));

	}
	void defineAllTeams() {
		// pair is leader -> team
		// DEBUG
		definedTeams["DEBUG"] = { "DebugWoman", List<Combatant>({
			definedCombatants["SadBag"], definedCombatants["DebugWoman"]}) };

		definedTeams["DEBUG2"] = { "FakeRider", List<Combatant>({
			definedCombatants["FakeRider"]}) };

		// EVENT
		definedTeams["EVENT1"] = { "EnragedVilomah", List<Combatant>({
			definedCombatants["EnragedMagician"],
			definedCombatants["EnragedPriest"],
			definedCombatants["EnragedVilomah"],
			definedCombatants["EnragedVagrant"], }) };

		definedTeams["EVENT2"] = { "Father Michelet", List<Combatant>({
			definedCombatants["Father Michelet"],}) };

		definedTeams["EVENT3"] = { "EnragedRider", List<Combatant>({
			definedCombatants["EnragedRider"],}) };

		definedTeams["EVENT4"] = { "EnragedMagician", List<Combatant>({
				definedCombatants["EnragedMagician"],
				definedCombatants["EnragedMagician"],
				definedCombatants["EnragedMagician"],
				definedCombatants["EnragedPriest"],
			}) };

		definedTeams["EVENT5"] = { "EnragedButcher", List<Combatant>({
				definedCombatants["EnragedButcher"],
				definedCombatants["EnragedNe'erDoWell"],
				definedCombatants["EnragedDeaconess"],
				definedCombatants["EnragedNoblewoman"],
			}) };

		definedTeams["SHADOWTEAM"] = { "DarkAngela", List<Combatant>({
			definedCombatants["DarkAngela"], definedCombatants["DarkOlyver"], definedCombatants["DarkTianshun"], definedCombatants["DarkGihat"]}) };

		definedTeams["SHADOWTEAM2"] = { "DarkTianshun", List<Combatant>({
			definedCombatants["DarkHernando"],}) };

		// ENRAGED

	}
	void defineAllEffectDefinitions() {
		// originate from skills
		allEffectDefinitions["Life Drain"] = EffectObject("Life Drain", "BANE", SKILLICON_LIFEDRAIN, "LIFESTEAL_SINGLE_UNHOLY", false,
			List<string>(list<string>({ "LIFESTEAL_SINGLE_UNHOLY", })),
			List<string>(list<string>({ "EVERYTURN","ONAPPLY", })));

		allEffectDefinitions["Gentleman's Riposte"] = EffectObject("Gentleman's Riposte", "BOON", SKILLICON_GENTLEMANSRIPOSTE, "Gentleman's Riposte", false,
			List<string>(list<string>({ "Gentleman's Riposte", })),
			List<string>(list<string>({ "ANYALLYATTACKEDPHYSICAL", })));

		allEffectDefinitions["Trickblade"] = EffectObject("Trickblade", "BOON", SKILLICON_TRICKBLADE, "Trickblade", false,
			List<string>(list<string>({ "Trickblade", })),
			List<string>(list<string>({ "SELFATTACKEDPHYSICAL", })));

		allEffectDefinitions["Rainstorm"] = EffectObject("Rainstorm", "NEUTRAL", SKILLICON_RAINSTORM, "APPLY_WET_ALL", false,
			List<string>(list<string>({ "Rainstorm", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["Sandstorm"] = EffectObject("Sandstorm", "NEUTRAL", SKILLICON_SANDSTORM, "APPLY_DUSTY_ALL", false,
			List<string>(list<string>({ "Sandstorm", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["Heatwave"] = EffectObject("Heatwave", "NEUTRAL", SKILLICON_HEATWAVE, "APPLY_PARCHED_ALL", false,
			List<string>(list<string>({ "Heatwave", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["Pressure Front"] = EffectObject("Pressure Front", "NEUTRAL", SKILLICON_PRESSUREFRONT, "APPLY_STATIC_ALL", false,
			List<string>(list<string>({ "Pressure Front", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["Healing Rain"] = EffectObject("Healing Rain", "NEUTRAL", SKILLICON_HEALINGRAIN, "LIFEHEAL_AOE_WATER", false,
			List<string>(list<string>({ "Healing Rain", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["Ice Storm"] = EffectObject("Ice Storm", "NEUTRAL", SKILLICON_ICESTORM, "DAMAGE_ALLFOES_COLD", false,
			List<string>(list<string>({ "Ice Storm", })),
			List<string>(list<string>({ "EVERYTURN", })));

		allEffectDefinitions["Chaos Storm"] = EffectObject("Chaos Storm", "NEUTRAL", SKILLICON_CHAOSSTORM, "MANABURN_ALLFOES_SHADOW", false,
			List<string>(list<string>({ "Chaos Storm", })),
			List<string>(list<string>({ "EVERYTURN", })));

		allEffectDefinitions["Ice AgeW"] = EffectObject("Ice AgeW", "NEUTRAL", SKILLICON_ICEAGE, "APPLY_Ice Age_ALL", false,
			List<string>(list<string>({ "Ice AgeW", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["Ice Age"] = EffectObject("Ice Age", "NEUTRAL", SKILLICON_ICEAGE, "Ice Age", false,
			List<string>(list<string>({ "Ice Age", })),
			List<string>(list<string>({ "ONTAKINGCOLDDAMAGE" })));

		allEffectDefinitions["Global WarmingW"] = EffectObject("Global WarmingW", "NEUTRAL", SKILLICON_GLOBALWARMING, "APPLY_Global Warming_ALL", false,
			List<string>(list<string>({ "Global WarmingW", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["Global Warming"] = EffectObject("Global Warming", "NEUTRAL", SKILLICON_GLOBALWARMING, "Global Warming", false,
			List<string>(list<string>({ "Global Warming", })),
			List<string>(list<string>({ "ONTAKINGFIREDAMAGE" })));

		allEffectDefinitions["TempestW"] = EffectObject("TempestW", "NEUTRAL", SKILLICON_TEMPEST, "APPLY_Tempest_ALL", false,
			List<string>(list<string>({ "TempestW", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["Tempest"] = EffectObject("Tempest", "NEUTRAL", SKILLICON_TEMPEST, "Tempest", false,
			List<string>(list<string>({ "Tempest", })),
			List<string>(list<string>({ "ONTAKINGELECTRICDAMAGE" })));

		allEffectDefinitions["DroughtW"] = EffectObject("DroughtW", "NEUTRAL", SKILLICON_DROUGHT, "APPLY_Drought_ALL", false,
			List<string>(list<string>({ "DroughtW", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["Drought"] = EffectObject("Drought", "NEUTRAL", SKILLICON_DROUGHT, "Drought", false,
			List<string>(list<string>({ "Drought", })),
			List<string>(list<string>({ "ONTAKINGEARTHDAMAGE" })));

		allEffectDefinitions["Strength of Reason"] = EffectObject("Strength of Reason", "BOON", SKILLICON_STRENGTHOFREASON, "Strength of Reason", false,
			List<string>(list<string>({ "Strength of Reason", })),
			List<string>(list<string>({ "ONDOINGPHYSICALATTACK", })));

		allEffectDefinitions["Blade of Blood"] = EffectObject("Blade of Blood", "BOON", SKILLICON_BLADEOFBLOOD, "Blade of Blood", false,
			List<string>(list<string>({ "Blade of Blood", })),
			List<string>(list<string>({ "ONDOINGPHYSICALATTACK", })));

		allEffectDefinitions["Stone Curse"] = EffectObject("Stone Curse", "BANE", SKILLICON_STONECURSE, "Stone Curse", false,
			List<string>(list<string>({ "Stone Curse", })),
			List<string>(list<string>({ "ONTRYINGTOINTERRUPT", })));

		allEffectDefinitions["Basalt Bastion"] = EffectObject("Basalt Bastion", "BOON", SKILLICON_BASALTBASTION, "Basalt Bastion", false,
			List<string>(list<string>({ "PROTECTVSINTERRUPTION", })),
			List<string>(list<string>({ "ONBEINGINTERRUPTED", })));

		allEffectDefinitions["Order of the Wasp"] = EffectObject("Order of the Wasp", "BOON", SKILLICON_ORDEROFTHEWASP, "Order of the Wasp", false,
			List<string>(list<string>({ "Order of the Wasp", })),
			List<string>(list<string>({ "ONDOINGPHYSICALATTACK", })));

		allEffectDefinitions["Curse from Beyond the Grave"] = EffectObject("Curse from Beyond the Grave", "BANE", SKILLICON_CURSEFROMBEYONDTHEGRAVE, "Curse from Beyond the Grave", false,
			List<string>(list<string>({ "Curse from Beyond the Grave", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE", })));

		allEffectDefinitions["Viper Eyes"] = EffectObject("Viper Eyes", "BOON", SKILLICON_VIPEREYES, "Viper Eyes", false,
			List<string>(list<string>({ "Viper Eyes", })),
			List<string>(list<string>({ "ONSTEALINGLIFE", })));

		allEffectDefinitions["Beggar's Blessing"] = EffectObject("Beggar's Blessing", "BANE", SKILLICON_BEGGARSBLESSING, "Beggar's Blessing", false,
			List<string>(list<string>({ "Beggar's Blessing", })),
			List<string>(list<string>({ "ONBEINGHEALED", })));

		allEffectDefinitions["Cestodarian Siphon"] = EffectObject("Cestodarian Siphon", "BANE", SKILLICON_CESTODARIANSIPHON, "Cestodarian Siphon", false,
			List<string>(list<string>({ "Cestodarian Siphon", })),
			List<string>(list<string>({ "WHENCASTINGASPELL", })));

		allEffectDefinitions["Great Gospel"] = EffectObject("Great Gospel", "BOON", SKILLICON_GREATGOSPEL, "LIFEHEAL_SINGLE_HOLY", false,
			List<string>(list<string>({ "Great Gospel", })),
			List<string>(list<string>({ "EVERYTURN", })));

		allEffectDefinitions["Apostle of Patience"] = EffectObject("Apostle of Patience", "BOON", SKILLICON_APOSTLEOFPATIENCE, "LIFEHEAL_SINGLE_HOLY", false,
			List<string>(list<string>({ "Apostle of Patience", })),
			List<string>(list<string>({ "ONEND", })));

		allEffectDefinitions["Shield of a Goddess"] = EffectObject("Shield of a Goddess", "BOON", SKILLICON_SHIELDOFAGODDESS, "Shield of a Goddess", false,
			List<string>(list<string>({ "Shield of a Goddess", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE", })));

		allEffectDefinitions["Ivory Sanctuary"] = EffectObject("Ivory Sanctuary", "BOON", SKILLICON_IVORYSANCTUARY, "Ivory Sanctuary", false,
			List<string>(list<string>({ "Ivory Sanctuary", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE", })));

		allEffectDefinitions["Papalcy"] = EffectObject("Papalcy", "BOON", SKILLICON_PAPALCY, "Papalcy", false,
			List<string>(list<string>({ "Papalcy", })),
			List<string>(list<string>({ "ONUSINGSKILLONALLY", })));

		allEffectDefinitions["Incessant Devotion"] = EffectObject("Incessant Devotion", "BOON", SKILLICON_INCESSANTDEVOTION, "Incessant Devotion", false,
			List<string>(list<string>({ "Incessant Devotion", })),
			List<string>(list<string>({ "ONUSINGSKILLONALLY", "ONTAKINGDAMAGE"})));

		allEffectDefinitions["Remedy Ward"] = EffectObject("Remedy Ward", "BOON", SKILLICON_REMEDYWARD, "Remedy Ward", false,
			List<string>(list<string>({ "Remedy Ward", })),
			List<string>(list<string>({  "ONBEINGCURSED" })));

		allEffectDefinitions["Angelic Observatory"] = EffectObject("Angelic Observatory", "BOON", SKILLICON_ANGELICOBSERVATORY, "Angelic Observatory", false,
			List<string>(list<string>({ "Angelic Observatory", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE" })));

		allEffectDefinitions["Gift of Knowledge"] = EffectObject("Gift of Knowledge", "BOON", SKILLICON_GIFTOFKNOWLEDGE, "Gift of Knowledge", false,
			List<string>(list<string>({ "Gift of Knowledge", })),
			List<string>(list<string>({ "ONUSINGSKILL" })));

		allEffectDefinitions["Paraclete's Invitation"] = EffectObject("Paraclete's Invitation", "BOON", SKILLICON_PARACLETESINVITATION, "Paraclete's Invitation", false,
			List<string>(list<string>({ "Paraclete's Invitation", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE" })));

		allEffectDefinitions["Castigate Cruor"] = EffectObject("Castigate Cruor", "BANE", SKILLICON_CASTIGATECRUOR, "Castigate Cruor", false,
			List<string>(list<string>({ "Castigate Cruor", })),
			List<string>(list<string>({ "ONSTEALINGLIFE", "ONSACRIFICE"})));

		allEffectDefinitions["Entomb Spirit"] = EffectObject("Entomb Spirit", "BANE", SKILLICON_ENTOMBSPIRIT, "Entomb Spirit", false,
			List<string>(list<string>({ "Entomb Spirit", })),
			List<string>(list<string>({ "ONTRYINGTORES", })));

		allEffectDefinitions["Stalked by Vengeance"] = EffectObject("Stalked by Vengeance", "BANE", SKILLICON_STALKEDBYVENGEANCE, "Stalked by Vengeance", false,
			List<string>(list<string>({ "Stalked by Vengeance", })),
			List<string>(list<string>({ "ONDEALINGDAMAGE", })));

		allEffectDefinitions["Cleave Armour"] = EffectObject("Cleave Armour", "BANE", SKILLICON_CLEAVEARMOUR, "Cleave Armour", false,
			List<string>(list<string>({ "Cleave Armour", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE", })));

		allEffectDefinitions["Knight Vision"] = EffectObject("Knight Vision", "BOON", SKILLICON_KNIGHTVISION, "Knight Vision", false,
			List<string>(list<string>({ "Knight Vision", })),
			List<string>(list<string>({ "ONATTACKING", })));

		allEffectDefinitions["On My Target!"] = EffectObject("On My Target!", "BANE", SKILLICON_ONMYTARGET, "On My Target!", false,
			List<string>(list<string>({ "On My Target!", })),
			List<string>(list<string>({ "ONTAKINGPHYSICALDAMAGE", })));

		allEffectDefinitions["Bramble Cloak"] = EffectObject("Bramble Cloak", "BOON", SKILLICON_BRAMBLECLOAK, "Bramble Cloak", false,
			List<string>(list<string>({ "Bramble Cloak", })),
			List<string>(list<string>({ "ONTAKINGELEMENTALDAMAGE", })));

		allEffectDefinitions["Shield of the Messenger"] = EffectObject("Shield of the Messenger", "BOON", SKILLICON_SHIELDOFTHEMESSENGER, "Shield of the Messenger", false,
			List<string>(list<string>({ "Shield of the Messenger", })),
			List<string>(list<string>({ "ONBEINGATTACKED", })));

		allEffectDefinitions["Reckless Swing"] = EffectObject("Reckless Swing", "BANE", SKILLICON_RECKLESSSWING, "Reckless Swing", false,
			List<string>(list<string>({ "Reckless Swing", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE", })));

		allEffectDefinitions["Weaponsmithing"] = EffectObject("Weaponsmithing", "BOON", SKILLICON_WEAPONSMITHING, "Weaponsmithing", false,
			List<string>(list<string>({ "Weaponsmithing", })),
			List<string>(list<string>({ "ONUSINGSKILLS", })));

		allEffectDefinitions["Sanctum Shroud"] = EffectObject("Sanctum Shroud", "BOON", SKILLICON_SANCTUMSHROUD, "Sanctum Shroud", false,
			List<string>(list<string>({ "Sanctum Shroud", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE", })));

		allEffectDefinitions["Lacrymactory"] = EffectObject("Lacrymactory", "BOON", SKILLICON_LACRYMACTORY, "Lacrymactory", false,
			List<string>(list<string>({ "Lacrymactory", })),
			List<string>(list<string>({ "WHENSOMEONEDIES", })));

		allEffectDefinitions["Exemplar's Posture"] = EffectObject("Exemplar's Posture", "BOON", SKILLICON_EXEMPLARSPOSTURE, "Exemplar's Posture", false,
			List<string>(list<string>({ "Exemplar's Posture", })),
			List<string>(list<string>({ "ONATTACKING", })));

		allEffectDefinitions["Avenger's Prayer"] = EffectObject("Avenger's Prayer", "BOON", SKILLICON_AVENGERSPRAYER, "Avenger's Prayer", false,
			List<string>(list<string>({ "Avenger's Prayer", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE", })));

		allEffectDefinitions["Proscribe"] = EffectObject("Proscribe", "BANE", SKILLICON_PROSCRIBE, "Proscribe", false,
			List<string>(list<string>({ "Proscribe", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE", })));

		allEffectDefinitions["Conversion"] = EffectObject("Conversion", "BOON", SKILLICON_PROSCRIBE, "Conversion", false,
			List<string>(list<string>({ "Conversion", })),
			List<string>(list<string>({ "ONDEALINGDAMAGE", })));

		allEffectDefinitions["Fading Justice"] = EffectObject("Fading Justice", "BOON", SKILLICON_FADINGJUSTICE, "Fading Justice", false,
			List<string>(list<string>({ "Fading Justice", })),
			List<string>(list<string>({ "ONDOINGPHYSICALATTACK", })));

		allEffectDefinitions["Suppress"] = EffectObject("Suppress", "BANE", SKILLICON_SUPPRESS, "DAMAGE_SINGLE_HOLY", false,
			List<string>(list<string>({ "DAMAGE_SINGLE_HOLY", })),
			List<string>(list<string>({ "EVERYTURN","ONTRYINGTOINTERRUPT"})));

		allEffectDefinitions["I Shall Take Care of This!"] = EffectObject("I Shall Take Care of This!", "BOON", SKILLICON_ISHALLTAKECAREOFTHIS , "I Shall Take Care of This!", false,
			List<string>(list<string>({ "I Shall Take Care of This!", })),
			List<string>(list<string>({ "ONDOINGPHYSICALATTACK" })));

		allEffectDefinitions["Lord's Authority"] = EffectObject("Lord's Authority", "BOON", SKILLICON_LORDSAUTHORITY, "Lord's Authority", false,
			List<string>(list<string>({ "Lord's Authority", })),
			List<string>(list<string>({ "ONAPPLYINGEFFECTTOSELF" })));

		allEffectDefinitions["Bailiff's Blade"] = EffectObject("Bailiff's Blade", "BOON", SKILLICON_BAILIFFSBLADE, "Bailiff's Blade", false,
			List<string>(list<string>({ "Bailiff's Blade", })),
			List<string>(list<string>({ "ONDOINGPHYSICALATTACK" })));

		allEffectDefinitions["Fight the Pain!"] = EffectObject("Fight the Pain!", "BOON", SKILLICON_FIGHTTHEPAIN, "LIFEHEAL_SINGLE_HOLY", false,
			List<string>(list<string>({ "Fight the Pain!", })),
			List<string>(list<string>({ "EVERYTURN" })));

		allEffectDefinitions["No One Said You Could Touch!"] = EffectObject("No One Said You Could Touch!", "BOON", SKILLICON_NOONESAIDYOUCOULDTOUCH, "No One Said You Could Touch!", false,
			List<string>(list<string>({ "No One Said You Could Touch!", })),
			List<string>(list<string>({ "ONBEINGATTACKED" })));

		allEffectDefinitions["Fencer's Flash"] = EffectObject("Fencer's Flash", "BOON", SKILLICON_FENCERSFLASH, "Fencer's Flash", false,
			List<string>(list<string>({ "Fencer's Flash", })),
			List<string>(list<string>({ "ONBEINGATTACKED" })));

		allEffectDefinitions["Go On Without Me!"] = EffectObject("Go On Without Me!", "BOON", SKILLICON_GOONWITHOUTME, "Go On Without Me!", false,
			List<string>(list<string>({ "Go On Without Me!", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE" })));

		allEffectDefinitions["Charm Collapse"] = EffectObject("Charm Collapse", "BANE", SKILLICON_CHARMCOLLAPSE, "Charm Collapse", false,
			List<string>(list<string>({ "Go On Without Me!", })),
			List<string>(list<string>({ "ONBEINGMANABURNED" })));

		allEffectDefinitions["Stalked by Shadows"] = EffectObject("Stalked by Shadows", "BANE", SKILLICON_STALKEDBYSHADOWS, "Stalked by Shadows", false,
			List<string>(list<string>({ "Stalked by Shadows", })),
			List<string>(list<string>({ "ONTRYINGTOCASTASPELL" })));

		allEffectDefinitions["Mind Maze"] = EffectObject("Mind Maze", "BANE", SKILLICON_MINDMAZE, "Mind Maze", false,
			List<string>(list<string>({ "Mind Maze", })),
			List<string>(list<string>({ "ONTRYINGTOCASTASPELL" })));

		allEffectDefinitions["Time Walk"] = EffectObject("Time Walk", "BOON", SKILLICON_TIMEWALK, "Time Walk", false,
			List<string>(list<string>({ "Time Walk", })),
			List<string>(list<string>({ "ONCASTINGASPELL" })));

		allEffectDefinitions["Underworld Dreams"] = EffectObject("Underworld Dreams", "BANE", SKILLICON_UNDERWORLDDREAMS, "Underworld Dreams", false,
			List<string>(list<string>({ "Underworld Dreams", })),
			List<string>(list<string>({ "ONAPPLYINGABANE" })));

		allEffectDefinitions["Wildfire"] = EffectObject("Wildfire", "BANE", SKILLICON_WILDFIRE, "Wildfire", false,
			List<string>(list<string>({ "Wildfire", })),
			List<string>(list<string>({ "EVERYTURN" })));

		allEffectDefinitions["Cataclysm"] = EffectObject("Cataclysm", "NEUTRAL", SKILLICON_ICESTORM, "DAMAGE_ALLFOES_FIRE", false,
			List<string>(list<string>({ "Cataclysm", })),
			List<string>(list<string>({ "EVERYTURN", })));

		allEffectDefinitions["Ring of Ash"] = EffectObject("Ring of Ash", "NEUTRAL", SKILLICON_RINGOFASH, "Ring of Ash", false,
			List<string>(list<string>({ "Ring of Ash", })),
			List<string>(list<string>({ "ONBEINGCURSED", })));

		allEffectDefinitions["Charge Bolt2"] = EffectObject("Charge Bolt2", "BOON", SKILLICON_CHARGEBOLT, "Charge Bolt2", false,
			List<string>(list<string>({ "Charge Bolt2", })),
			List<string>(list<string>({ "ONUSINGSKILLS", })));

		allEffectDefinitions["Shock Value"] = EffectObject("Shock Value", "BOON", SKILLICON_SHOCKVALUE, "Shock Value", false,
			List<string>(list<string>({ "Shock Value", })),
			List<string>(list<string>({ "ONUSINGSKILLS", })));

		allEffectDefinitions["Electric Loop"] = EffectObject("Electric Loop", "BOON", SKILLICON_ELECTRICLOOP, "Electric Loop", false,
			List<string>(list<string>({ "Electric Loop", })),
			List<string>(list<string>({ "ONUSINGSKILLS", })));

		// sourced from equipment
		allEffectDefinitions["ARMOURVSELECTRIC"] = EffectObject("ARMOURVSELECTRIC", "BOON", EFFECTICON_ARMOURVSELECTRIC, "ARMOURVSELECTRIC", true,
			List<string>(list<string>({ "ARMOURVSELECTRIC", })),
			List<string>(list<string>({ "ONTAKINGELECTRICDAMAGE"})));
		

		// neutral conditions
		allEffectDefinitions["WET"] = EffectObject("WET", "NEUTRAL", EFFECTICON_WET, "WET", false,
			List<string>(list<string>({ "WET", })),
			List<string>(list<string>({ "ONTAKINGCOLDDAMAGE",  "ONTAKINGELECTRICDAMAGE", "ONTAKINGFIREDAMAGE"})));

		allEffectDefinitions["DUSTY"] = EffectObject("DUSTY", "NEUTRAL", EFFECTICON_DUSTY, "DUSTY", false,
			List<string>(list<string>({ "DUSTY", })),
			List<string>(list<string>({ "ONTAKINGCOLDDAMAGE",  "ONTAKINGEARTHDAMAGE", "ONTAKINGELECTRICEDAMAGE" })));

		allEffectDefinitions["PARCHED"] = EffectObject("PARCHED", "NEUTRAL", EFFECTICON_PARCHED, "PARCHED", false,
			List<string>(list<string>({ "PARCHED", })),
			List<string>(list<string>({ "ONTAKINGCOLDDAMAGE",  "ONTAKINGEARTHDAMAGE", "ONTAKINGFIREDAMAGE" })));

		allEffectDefinitions["STATIC"] = EffectObject("STATIC", "NEUTRAL", EFFECTICON_STATIC, "STATIC", false,
			List<string>(list<string>({ "STATIC", })),
			List<string>(list<string>({ "ONTAKINGEARTHDAMAGE",  "ONTAKINGELECTRICDAMAGE", "ONTAKINGFIREDAMAGE" })));


		// negative conditions
		allEffectDefinitions["CONCUSSED"] = EffectObject("CONCUSSED", "BANE", EFFECTICON_CONCUSSED, "CONCUSSED", false,
			List<string>(list<string>({ "CONCUSSED", })),
			List<string>(list<string>({ "WHENCASTINGASPELL" })));

		allEffectDefinitions["BLEEDING"] = EffectObject("BLEEDING", "BANE", EFFECTICON_BLEEDING, "BLEEDING", false,
			List<string>(list<string>({ "BLEEDING", })),
			List<string>(list<string>({ "WHENCASTINGASPELL" })));

		allEffectDefinitions["FROZEN"] = EffectObject("FROZEN", "BANE", EFFECTICON_FROZEN, "FROZEN", false,
			List<string>(list<string>({ "FROZEN", })),
			List<string>(list<string>({ "WHENCASTINGASPELL"})));

		allEffectDefinitions["BLIND"] = EffectObject("BLIND", "BANE", EFFECTICON_BLIND, "BLIND", false,
			List<string>(list<string>({ "BLIND", })),
			List<string>(list<string>({ "WHENATTACKING" })));

		allEffectDefinitions["BURNING"] = EffectObject("BURNING", "BANE", EFFECTICON_BURNING, "BURNING", false,
			List<string>(list<string>({ "BURNING", })),
			List<string>(list<string>({ "EVERYTURN" })));

		allEffectDefinitions["WEAKENED"] = EffectObject("WEAKENED", "BANE", EFFECTICON_WEAKNESS, "WEAKENED", false,
			List<string>(list<string>({ "WEAKENED", })),
			List<string>(list<string>({  })));

		allEffectDefinitions["DISEASED"] = EffectObject("DISEASED", "BANE", EFFECTICON_DISEASED, "DISEASED", false,
			List<string>(list<string>({ "DISEASED", })),
			List<string>(list<string>({  })));

		allEffectDefinitions["POISONED"] = EffectObject("POISONED", "BANE", EFFECTICON_POISONED, "POISONED", false,
			List<string>(list<string>({ "POISONED", })),
			List<string>(list<string>({  })));

		allEffectDefinitions["CRIPPLED"] = EffectObject("CRIPPLED", "BANE", EFFECTICON_CRIPPLED, "CRIPPLED", false,
			List<string>(list<string>({ "CRIPPLED", })),
			List<string>(list<string>({  })));

		allEffectDefinitions["OVERWHELMED"] = EffectObject("OVERWHELMED", "BANE", EFFECTICON_OVERWHELMED, "OVERWHELMED", false,
			List<string>(list<string>({ "OVERWHELMED", })),
			List<string>(list<string>({  })));


		// permanent
		allEffectDefinitions["UNDEAD"] = EffectObject("UNDEAD", "PERM", EFFECTICON_UNDEAD, "UNDEAD", true,
			List<string>(list<string>({ "UNDEAD", })),
			List<string>(list<string>({ "ONTAKINGHOLYDAMAGE", "ONBEINGHEALED" })));

		allEffectDefinitions["Fragile"] = EffectObject("Fragile", "PERM", EFFECTICON_UNDEAD, "Fragile", true,
			List<string>(list<string>({ "Fragile", })),
			List<string>(list<string>({ "ONTAKINGDAMAGE" })));
	}
	void newGame() {
		saveContainer.current = SaveContainer::SaveFile();
		saveContainer.current.equippedSkills = defaultSkillChoices.internalMap;
		saveContainer.current.attributeInvestments = defaultAttInvestments.internalMap;
		saveContainer.current.equippedSkillTrees = defaultSkillTreeChoices.internalMap;
		saveContainer.current.equippedItems = defaultEquipment.internalMap;
		List<string> allStartingCharacters = saveContainer.getAllPlayableCharacters();
		// add all to allCharacters then remove them when game actually starts
		saveContainer.current.allCharacters = allStartingCharacters.internalList;
		for (auto character : allStartingCharacters.internalList) {
			for (auto skill : defaultSkillChoices[character]) {
					saveContainer.current.knownSkills[character].push_back(skill.second);
			}
		}
		saveContainer.current.party = { "Angela Fleuret" };
	}
	List<string> getAllStartingCharacters() {
		return saveContainer.getAllStartingCharacters();
	}
	List<string> getAllLegalSkills() {
		List<string> result;
		for (auto s : skillDefinitions.getKeys().internalList) {
			if (skillDefinitions[s].skillTree != "Default") {
				result.push_back(s);
			}
		}
		return result;
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
	List<Combatant> getThisTeam(string instruction, string teamName) {
		List<Combatant> result;
		if (instruction == "") {
			return result;
		}
		if (instruction == "PARTY") {
			for (auto member : saveContainer.current.party) {
				result.push_back(loadPartyMemberAsCombatant(member));
			}
		}
		else {
			result = definedTeams[instruction].second;
		}

		for (int x = 0; x < result.size(); x++) {
			result.at(x).uniqueCombatID = teamName + "_" + to_string(x + 1) + "_" + result.at(x).uniqueID;
			result.at(x).slot = to_string(x + 1);
			result.at(x).setCombatStats(*this, true);
		}
		return result;
	}
	Combatant loadPartyMemberAsCombatant(string name) {
		if (currentBattle != NULL) {
			// if in a battle, use the one from the battle instead so it picks up any status changes
			for (auto* c : currentBattle->getAllCombatants().internalList) {
				if (c->c.uniqueID == name) {
					return c->c;
				}
			}
		}
		Map<string, string> equippedSkillNames; equippedSkillNames.internalMap = saveContainer.current.equippedSkills[name];
		Map<string, string> equippedSkillTreeNames; equippedSkillTreeNames.internalMap = saveContainer.current.equippedSkillTrees[name];
		Map<string, string> equipment; equipment.internalMap = saveContainer.current.equippedItems[name];
		Map<string, int> attributeInvestments; attributeInvestments.internalMap = saveContainer.current.attributeInvestments[name];
		Map<string, Map<string, string>> data;
		data["equippedSkillNames"] = equippedSkillNames;
		data["equipmentNames"] = equipment;
		data["equippedSkillTreeNames"] = equippedSkillTreeNames;
		data["Images"]["Back"] = imageLookup.getSequenceAsString(name, "COMBAT_BACK");
		data["Images"]["Front"] = imageLookup.getSequenceAsString(name, "COMBAT_FRONT");
		Combatant result = Combatant(name, name, attributeInvestments, data);
		return result;
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
	Map<string, Combatant> definedCombatants;
	Map<string, pair<string, List<Combatant>>> definedTeams;
	Map<string, EffectObject> allEffectDefinitions;
	string language = "ENG";
	Map<string, string> layerScaleLookup;
};
Combat combat;
