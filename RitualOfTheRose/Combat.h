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
		influenceLookups["Dual Weapon MasteryBOOST"] = 0.06;
		influenceLookups["1H Weapon MasteryBOOST"] = 0.06;

		// skillValueBoosts
		Map<string, wstring> types; types.internalMap = strings["ENG"]["Type Names"];
		for (auto type : types.getKeys().internalList) {
			influenceLookups[type + "COST"] = 0.05;
			influenceLookups[type + "ACTIVATION"] = 0.05;
			influenceLookups[type + "RECHARGE"] = 0.05;
		}

		// armour boosts
		influenceLookups["PHYSICALARMOUR"] = 0.05;
		influenceLookups["ELECTRICARMOUR"] = 0.05;

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
		defineAllCombatants();
		defineAllTeams();
		defineAllEffectDefinitions();
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
	class Equipment {
	public:
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
				// description comes from a skill
				wstring result = strings[language]["Skill Descriptions"][uniqueID];
				for (auto value : values.getKeys().internalList) {
					wstring name = StringToWString(toUpper(value));
					result = WSReplace(result, L"$" + name + L"_" + StringToWString(uniqueID) + L"$", to_wstring(values[value]));
				}
				return result;
			}
			// add stuff for effects that aren't also skills
			wstring result = strings[language]["Effect Descriptions"][uniqueID];
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
				if (subeffect == "Rainstorm") {
					result["APPLY_WET_ALL"] = e.values["duration"];
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
			defaultStats["ENERGYREGEN"] = PowerValue("ENERGYREGEN", 1, 0, 9999, true, list<string>({ "PIETY", "ENERGYREGENPLUS" }));
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
			if (which.powerValues.hasKey("COST")) {
				return getPowerOfThis(which.powerValues["COST"], includeOtherInfluences, *&combat);
			}
			return which.baseCost;
		}
		int getSkillActivationTime(Skill which, bool includeOtherInfluences, Combat& combat) {
			if (which.powerValues.hasKey("ACTIVATION")) {
				return getPowerOfThis(which.powerValues["ACTIVATION"], includeOtherInfluences, *&combat);
			}
			return which.baseCastingTime;
		}
		int getSkillRecharge(Skill which, bool includeOtherInfluences, Combat& combat) {
			if (which.powerValues.hasKey("RECHARGE")) {
				return getPowerOfThis(which.powerValues["RECHARGE"], includeOtherInfluences, *&combat);
			}
			return which.baseRecharge;
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
			for (auto report : combat.currentBattle->currentEventStackObject.ongoingReport.internalList) {
				if (report.vData.hasKey("DELAYTHISSKILLRECHARGE")) {
					rechargeTime += report.vData["DELAYTHISSKILLRECHARGE"];
				}
			}
			currentlyCasting = false;
			combatSkills[indexOfSkillCurrentlyBeingCast].timeToRecharge = rechargeTime;
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
					for (EffectObjectInstance* effect : combat.currentBattle->allEffectsInPlay[actor->c.uniqueCombatID].getValues().internalList) {
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
				bool ICouldBuff = false;
				for (CombatantInstance* actor : me->getAllMyOtherAlliesNotMe(*&combat).internalList) {
					if (actor->c.getLifeAsPercentage() < 0.9 and !actor->c.isDead()) {
						result.addToBackIfNotAlreadyInList("HEALALLY");
					}
					if (actor->c.isDead()) {
						result.addToBackIfNotAlreadyInList("RESURRECT");
					}
					List<string> effectsOnThisActor = combat.currentBattle->allEffectsInPlay[actor->c.uniqueCombatID].getKeys();
					for (auto s : buffsICouldUseOnAllies.internalList) {
						if (!effectsOnThisActor.contains(s)) {
							ICouldBuff = true;
						}
					}
				}
				if (ICouldBuff) {
					result.push_back("ENCHANTALLY");
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
								targetProbability[actor->c.uniqueCombatID] = 0;
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
								if (buff->e.uniqueID == skillDecision and buff->e.duration > 1 and not buff->e.infinite) {
									targetProbability[actor->c.uniqueCombatID] = 0;
								}
							}
						}

					}
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
							allStrategiesICouldFollow.push_back(p);
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
					pair<string, int>("DEALDAMAGE",20),
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
							}
						}
					}
				}
				string decision = RANDOM.diceRollChoice(chanceOfUsingSkill);
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
		}

		List<CombatEvent> startSkillExecution(Combat& combat, string language) {
			List<CombatEvent> results;
			Map<string, string> skillSData;
			Map<string, int> skillVData;
			for (auto [key, value] : c.combatSkills[c.indexOfSkillCurrentlyBeingCast].powerValues.internalMap) {
				skillVData[key] = c.getPowerOfThis(value, true, *&combat);
			}
			bool magical = c.combatSkills[c.indexOfSkillCurrentlyBeingCast].skillTypeTags.contains("MAGICAL");
			string target = combat.currentBattle->getThisCombatant(c.currentTarget)->c.uniqueCombatID;
			wstring skillName = strings[language]["Skill Names"][c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID];
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
			skillSData["success"] = "1";
			if (c.combatSkills[c.indexOfSkillCurrentlyBeingCast].audioSource != -1) {
				skillSData["audioSource"] = to_string(c.combatSkills[c.indexOfSkillCurrentlyBeingCast].audioSource);
			}

			for (auto skillLogicName : c.combatSkills[c.indexOfSkillCurrentlyBeingCast].skillLogicNames.internalList) {
				List<string> combatantsAffected;
				Map<string, string> sData = skillSData;
				Map<string, int> vData = skillVData;

				if (skillLogicName.find("_SINGLE") != -1 or skillLogicName.find("_SELF") != -1) {
					combatantsAffected.push_back(combat.currentBattle->getThisCombatant(c.currentTarget)->c.uniqueCombatID);
					sData["message"] = SReplace(skillSData["message"], "$TARGET$",
						" " + WStringToString(strings[language]["Skill Actions"]["ON"]) + " " + WStringToString(targetName)
					);
					skillSData["message"] = sData["message"];
					results.push_back(CombatEvent(skillLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, combatantsAffected, sData, vData));
				}
				if (skillLogicName.find("_WORLD") != -1) {
					combatantsAffected.push_back("WORLD");
					sData["message"] = SReplace(WStringToString(strings[language]["Skill Actions"]["WORLD_DONE"]), "$PLAYER$", WStringToString(userName));
					sData["message"] = SReplace(sData["message"], "$SKILL$", WStringToString(skillName));
					skillSData["message"] = sData["message"];
					results.push_back(CombatEvent(skillLogicName, "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, combatantsAffected, sData, vData));
				}
				if (skillLogicName.find("_ALL") != -1) {
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
				// add other targeting systems
			}
			if (c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID == "DEFAULT_WAIT") {
				combat.currentBattle->addCombatMessage("WAIT", List<pair<string, string>>({
					pair<string, string>("language", language),
					pair<string, string>("name", c.uniqueID),
					}), 0);
				results.push_back(CombatEvent("DEFAULT_WAIT", "SKILL", c.combatSkills[c.indexOfSkillCurrentlyBeingCast].uniqueID, c.uniqueCombatID, { c.uniqueCombatID }, {}, {}));
			}
			else {
				combat.currentBattle->addCombatMessage("DIRECT", pair<string, string>("message", skillSData["message"]), 0);
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
				for (auto& report : ongoingReport.internalList) {
					List<EffectObjectInstance*> allEffects = combat.currentBattle->getAllEffectsInTimeOrderOldestFirst();
					allEffects.internalList.reverse();
					CombatantInstance* user = combat.currentBattle->getThisCombatant(report.originalUser);
					List<CombatantInstance* > targets;
					if (report.type == "SKILL" and user->c.currentlyCasting) {
						for (EffectObjectInstance * effect : allEffects.internalList) {
							if (effect->e.target == report.originalUser) {
								if (effect->e.logicName == "CONCUSSED" and user->c.getSkillBeingCast().skillTypeTags.contains("MAGICAL")) {
									report.vData["FAILCHANCE"] = 50;
								}
							}
						}
					}

					if (report.combatantsAffected.contains("WORLD")) {
						report.sData["success"] = "1";
						return;
					}
					for (auto t : report.combatantsAffected.internalList) {
						if (combat.currentBattle->doesThisCombatantExist(t)) {
							targets.push_back(combat.currentBattle->getThisCombatant(t));
						}
					}
					List<string> reportKeys = report.vData.getKeys();

					if (report.logic.find("LIFEHEAL_SINGLE") != -1) {
						for (auto v : reportKeys.internalList) {
							for (EffectObjectInstance* effect : allEffects.internalList) {
								CombatantInstance* target = combat.currentBattle->getThisCombatant(effect->e.target);
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

					if (report.logic.find("LIFESTEAL_SINGLE") != -1) {
						for (EffectObjectInstance* effect : allEffects.internalList) {
							if (effect->e.target == "WORLD") { continue; }
							CombatantInstance* target = combat.currentBattle->getThisCombatant(effect->e.target);
							if (targets.contains(target)) {
								if (effect->e.logicName == "LIFESTEAL_SINGLE_UNHOLY") {
									report.vData["LIFESTEAL_SINGLE_UNHOLY"] = effect->e.values["power"];
								}
							}
						}
					}

					if (report.logic.find("DAMAGE_SINGLE") != -1) {
						for (auto v : reportKeys.internalList) {
							for (EffectObjectInstance* effect : allEffects.internalList) {
								if (effect->e.target == report.originalUser) { // this effect is on the user
									if (effect->e.triggers.contains("ONDOINGPHYSICALATTACK")) {
										if (effect->e.logicName == "Strength of Reason") {
											if (report.vData.hasKey("DAMAGE_SINGLE_PHYSICAL")) {
												report.vData["DAMAGE_SINGLE_HOLY"] = effect->e.values["power"];
											}
										}
									}
								}
								if (effect->e.target == "WORLD") {
									// any world effects that would happen at this stage (probably none)
									continue;
								}
								CombatantInstance* target = combat.currentBattle->getThisCombatant(effect->e.target);
								if (targets.contains(target)) { // this effect is on the victim
									if (v == "DAMAGE_SINGLE_ELECTRIC" and effect->e.triggers.contains("ONTAKINGELECTRICDAMAGE")) {
										if (effect->e.logicName == "WET") {
											report.vData["DAMAGE_SINGLE_ELECTRIC"] *= 2;
										}
									}
									if (v == "DAMAGE_SINGLE_COLD" and effect->e.triggers.contains("ONTAKINGCOLDDAMAGE")) {
										if (effect->e.logicName == "WET") {
											report.vData["DAMAGE_SINGLE_COLD"] *= 2;
										}
									}
									if (v == "DAMAGE_SINGLE_HOLY" and effect->e.triggers.contains("ONTAKINGHOLYDAMAGE")) {
										if (effect->e.logicName == "UNDEAD") {
											report.vData["DAMAGE_SINGLE_HOLY"] *= 2;
										}
									}
								}
							}
						}

					}
				
					
				}

				// predict if targets will die during resolution
				Map<string, int> projectedLifeLoss;
				for (auto& report : ongoingReport.internalList) {
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
						if (projectedLifeLoss[name] > combat.currentBattle->getThisCombatant(name)->c.combatStats["CURRENTLIFE"]) {
							report.sData["THISPERSONDIED"] += name + "$";
						}
					}
				}

				// consider any effects that happen if a target dies
				for (auto& report : ongoingReport.internalList) {
					if (report.sData.hasKey("THISPERSONDIED")) {
						List<string> whoDied = split(report.sData["THISPERSONDIED"], "$");
						if (report.vData.hasKey("HEAVENSTRIKE")) {
							ongoingReport.push_back(CombatEvent("DELAYTHISSKILLRECHARGE", "SKILL", "HEAL WOUNDS", report.originalUser, {}, {}, List<pair<string, int>>(
								pair<string, int>("DELAYTHISSKILLRECHARGE", report.vData["SKILLRECHARGEDELAY"]))));
							List<CombatantInstance*> targets = combat.currentBattle->getThisCombatant(report.originalUser)->getMyTeam(*&combat);
							for (CombatantInstance* target : targets.internalList) {
								ongoingReport.push_back(CombatEvent("LIFEHEAL_SINGLE_HOLY", "HEAL WOUNDS", "HEAL WOUNDS", report.originalUser, List<string>(target->c.uniqueCombatID), List<pair<string,string>>(
									pair<string, string>("success", "1")), List<pair<string, int>>(
									pair<string, int>("LIFEHEAL_SINGLE_HOLY", report.vData["HEAVENSTRIKE"]))));
							}
						}
					}
				}
			};

			void executeTheResults(Combat& combat, string language) {
				// execute the final results of what happened in the stack
				for (auto& report : ongoingReport.internalList) {
					string user = report.originalUser;
					if (report.sData["success"] == "1") {
						if (report.logic.find("APPLY_") != -1) {
							List<string> effectData = split(report.logic, "_");
							if (report.combatantsAffected.contains("ALL") or report.logic.find("_ALL") != -1) {
								report.combatantsAffected = combat.currentBattle->all.getKeys();
							}
							for (auto currentTarget : report.combatantsAffected.internalList) {
								string effectName = effectData.at(1);
								int duration = report.vData["DURATION_" + effectName];
								int power = report.vData["POWER_" + effectName];
								Map<string, int> values = { List<pair<string, int>>({
									pair<string, int>("duration", duration),
									pair<string, int>("power", power),
									}) };
								combat.currentBattle->applyAnEffect(*&combat, effectName, combat.allEffectDefinitions[effectName], report.originalUser, currentTarget, values);
								EffectObjectInstance* theEffect = combat.currentBattle->allEffectsInPlay[currentTarget][effectName];
								if (theEffect->e.triggers.contains("ONAPPLY")) {
									Map<string, int> subeffects = theEffect->executeDirectly(*&combat);
									for (auto subeffect : subeffects.getKeys().internalList) {
										report.vData[subeffect] = subeffects[subeffect];
									}
								}
							}
						}
						if (report.logic.find("LIFEHEAL_") != -1) {
							for (auto effect : report.vData.getKeys().internalList) {
								if (effect.find("LIFEHEAL_") != -1) {
									string type = split(effect, "_").at(2);
									for (auto currentTarget : report.combatantsAffected.internalList) {
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
						if (report.logic.find("LIFESTEAL_") != -1) {
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
					}
				}
			}

			CombatEvent getCurrentForAnimation() {
				return ongoingReport.at(counter);
			}

			void animationTick() {
				counter++;
				if (counter == 0 or counter == ongoingReport.size()) {
					return;
				}
				string previousAnimation = ongoingReport.at(counter - 1).getTemporaryID();
				string currentAnimation = getCurrentForAnimation().getTemporaryID();
				while (previousAnimation == currentAnimation and counter < ongoingReport.size()) {
					animationTick();
				}

			}
			bool isAnimationFinished() {
				return counter == ongoingReport.size();
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
			graphics.tearDownSpecifiedText("");
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
			if (actor->c.data.hasKey("Effects")) {
				for (auto effect : actor->c.data["Effects"].getKeys().internalList) {
					applyAnEffect(*&combat, effect, combat.allEffectDefinitions[effect], actor->c.uniqueCombatID, actor->c.uniqueCombatID, List<pair<string, int>>({
							pair<string, int>("duration",stoi(actor->c.data["Effects"][effect])),
						}));
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
		string tick() {
			currentEventStackObject = EventStackObject();
			int whichRound = currentRound.roundNumber;
			int currentAct = currentRound.tick();
			if (currentAct == currentRound.actorOrder.size()) {
				currentRound = Round(whichRound + 1, decideTurnOrder());
			}
			string who = currentRound.whoseTurnIsIt();
			if (all.getKeys().contains(who)) {
				all[who]->c.tickDownIfCasting();
				all[who]->c.tickDownRechargingSkills();
				all[who]->c.passiveManaRegen();
			}
			return battleStatusCheck();
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
		bool areTherePreTurnEffectsToRun() {
			return currentEventStackObject.ongoingReport.size() > 0;
		}
		void addCombatMessage(string type, Map<string, string> data, int verbosity) {
			// for top bar
			string language = data["language"];
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
				message = WSReplace(message, L"$OPPONENT$", strings[language]["NPCNames"][data["opponentName"]]);
			}
			if (type == "PLAYERTURN") {
				message = strings[language]["Combat Messages"]["PLAYERTURN"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][data["name"]]);
			}
			if (type == "AITURN") {
				message = strings[language]["Combat Messages"]["AITURN"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][data["name"]]);
			}
			if (type == "STARTCASTING") {
				message = strings[language]["Skill Actions"][data["skillType"] + "_STARTED"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][data["name"]]);
				message = WSReplace(message, L"$SKILL$", strings[language]["Skill Names"][data["skill"]]);
			}
			if (type == "STILLCASTING") {
				message = strings[language]["Skill Actions"][data["skillType"] + "_INPROGRESS"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][data["name"]]);
				message = WSReplace(message, L"$SKILL$", strings[language]["Skill Names"][data["skill"]]);
			}
			if (type == "DIRECT") {
				message = StringToWString(data["message"]);
			}
			if (type == "DAMAGE") {
				message = strings[language]["Combat Messages"]["DAMAGE"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][data["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(data["damage"]));
				message = WSReplace(message, L"$TYPE$", strings[language]["Type Names"][data["type"]]);
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][data["target"]]);
			}
			if (type == "LIFESTEAL") {
				message = strings[language]["Combat Messages"]["LIFESTEAL"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][data["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(data["damage"]));
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][data["target"]]);
			}
			if (type == "LIFEHEAL") {
				if (data["name"] == data["target"]) {
					message = strings[language]["Combat Messages"]["LIFEHEAL_SELF"];
				}
				else {
					message = strings[language]["Combat Messages"]["LIFEHEAL_OTHER"];
				}
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][data["name"]]);
				message = WSReplace(message, L"$X$", StringToWString(data["damage"]));
				message = WSReplace(message, L"$2$", strings[language]["NPCNames"][data["target"]]);
			}
			if (type == "WAIT") {
				message = strings[language]["Combat Messages"]["WAIT"];
				message = WSReplace(message, L"$1$", strings[language]["NPCNames"][data["name"]]);
			}
			if (type == "FAILEDSUMMONNOSPACE") {
				message = strings[language]["Skill Actions"]["FAILEDSUMMONNOSPACE"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][data["name"]]);
				message = WSReplace(message, L"$WHAT$", strings[language]["NPCNames"][data["summon"]]);
			}
			if (type == "VICTORY") {
				message = strings[language]["Combat Messages"]["VICTORY"];
				string leader = saveContainer.getCurrentMainCharacter();
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"][leader]);
				combatMessages.push_front({ verbosity, message });

				if (!rewardForWinning.getKeys().empty()) {
					message = strings[language]["GUI"]["BATTLEREWARD"];
					combatMessages.push_front({ verbosity, message });
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
						message = to_wstring(rewardForWinning[item]) + L" " + itemName;
						combatMessages.push_front({ verbosity, message });
					}
					return;
				}
			}
			if (type == "FAILED_NO_TARGETS") {
				message = strings[language]["Combat Messages"]["FAILED_NO_TARGETS"];
				message = WSReplace(message, L"$PLAYER$", strings[language]["NPCNames"]["name"]);
				message = WSReplace(message, L"$SKILL$", strings[language]["NPCNames"]["skill"]);
				message = WSReplace(message, L"$TARGET$", strings[language]["NPCNames"]["target"]);
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
			if (targetLogic == "SINGLEFOE") {
				results = combatant->getMyFoesThatAreAlive(*&combat);
			}
			if (targetLogic == "SINGLEALLY") {
				results = combatant->getAllMyAllies(*&combat);
			}
			if (targetLogic == "SINGLEOTHERALLY") {
				results = combatant->getAllMyOtherAlliesNotMe(*&combat);
			}
			if (List<string>({ "SELF", "ALL", "ALLALLIES", "ALLFOES" }).contains(targetLogic)) {
				results.push_back(combatant); // use on "self" but the effect hits the right place
			}

			return results;
		}
		List<CombatantInstance*> whoWillBeAffectedByThisSkill(Combat& combat, CombatantInstance* combatant, Skill skillDefinition) {
			List<CombatantInstance*> result;
			string targetLogic = skillDefinition.targetLogic;
			if (List<string>({ "SINGLEFOE","SINGLEALLY","SINGLEOTHERALLY", "SELF" }).contains(targetLogic)) {
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
				currentEventStackObject = EventStackObject(actor->startSkillExecution(*&combat, language));
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
				if (effect->e.triggers.contains("ONEND")) {}
				if (effect->e.expired()) {
					removeAnEffect(nextActor, effect->e.uniqueID);
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
			if (allEffectsInPlay[who][name] != NULL) {
				delete allEffectsInPlay[who][name];
				allEffectsInPlay[who][name] = NULL;
				allEffectsInPlay[who].internalMap.erase(name);
			}
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
		Map<string, Map<string, EffectObjectInstance*>> allEffectsInPlay; // who -> condition name -> condition object. who can include "TEAM1", "TEAM2", "WORLD"
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
		float party2alliesY = 25;
		float party2Y = 30;

		results["TEAM1_4"] = Map<string, pair<float, float>>({
				pair<string, pair<float, float>>("1", {10, party1Y}),
				pair<string, pair<float, float>>("2", {30, party1Y}),
				pair<string, pair<float, float>>("3", {50, party1Y}),
				pair<string, pair<float, float>>("4", {70, party1Y}),
			});
		results["TEAM1_3"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {15, party1Y}),
			pair<string, pair<float, float>>("2", {35, party1Y}),
			pair<string, pair<float, float>>("3", {55, party1Y}),
			});
		results["TEAM1_2"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {25, party1Y}),
			pair<string, pair<float, float>>("2", {45, party1Y}),
			});
		results["TEAM1_1"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {35, party1Y}),
			});
		results["TEAM1_ALLIES"] = Map<string, pair<float, float>>({
				pair<string, pair<float, float>>("1", {15, party1alliesY}),
				pair<string, pair<float, float>>("2", {35, party1alliesY}),
				pair<string, pair<float, float>>("3", {55, party1alliesY}),
				pair<string, pair<float, float>>("4", {75, party1alliesY}),
			});
		results["TEAM2_4"] = Map<string, pair<float, float>>({
				pair<string, pair<float, float>>("1", {25, party2Y}),
				pair<string, pair<float, float>>("2", {45, party2Y}),
				pair<string, pair<float, float>>("3", {65, party2Y}),
				pair<string, pair<float, float>>("4", {85, party2Y}),
			});
		results["TEAM2_3"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {35, party2Y}),
			pair<string, pair<float, float>>("2", {55, party2Y}),
			pair<string, pair<float, float>>("3", {75, party2Y}),
			});
		results["TEAM2_2"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {45, party2Y}),
			pair<string, pair<float, float>>("2", {65, party2Y}),
			});
		results["TEAM2_1"] = Map<string, pair<float, float>>({
			pair<string, pair<float, float>>("1", {55, party2Y}),
			});
		results["TEAM2_ALLIES"] = Map<string, pair<float, float>>({
				pair<string, pair<float, float>>("1", {20, party2alliesY}),
				pair<string, pair<float, float>>("2", {40, party2alliesY}),
				pair<string, pair<float, float>>("3", {60, party2alliesY}),
				pair<string, pair<float, float>>("4", {80, party2alliesY}),
			});
		return results;

	}
	void defineAllSkills() {
		// DEFAULT
		skillDefinitions["DEFAULT_ATTACK"] = Skill("DEFAULT_ATTACK", "DEFAULT_ATTACK", "Default", SKILLICON_ATTACK, 0, 0, 0, "SINGLEFOE", list<string>({ "DAMAGE_SINGLE_PHYSICAL" }), list<string>({ "PHYSICAL","ATTACK" }), Map<string, PowerValue>({
			pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH"}))) }), list<string>({ "DEALDAMAGE","DEALDAMAGE_PHYSICAL" }), 5819);

		skillDefinitions["DEFAULT_WAIT"] = Skill("DEFAULT_WAIT", "DEFAULT_WAIT", "Default", SKILLICON_WAIT, 0, 0, 0, "SELF", {}, {}, {}, list<string>({ "WAIT" }), -1);

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

		skillDefinitions["Revitalise"] = Skill("Revitalise", "Revitalise", "Cleromancy", SKILLICON_REVITALISE, 25, 2, 10, "DEADPARTYMEMBER",
			list<string>({ "RESURRECT" }),
			list<string>({ "MAGICAL","HOLY", "HEAL","TARGETSALLIES" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("POWER_1", PowerValue("POWER_1", 10, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({ "RESURRECT" }), HEALWOUNDS_WAV);

		// HAGIOMANCY
		skillDefinitions["Heavenstrike"] = Skill("Heavenstrike", "Heavenstrike", "Hagiomancy", SKILLICON_HEAVENSTRIKE, 10, 2, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_HOLY", "HEAVENSTRIKE" }),
			list<string>({ "MAGICAL","HOLY", "ELITE", "TARGETSFOES" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_HOLY", PowerValue("DAMAGE_SINGLE_HOLY", 70, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("SKILLRECHARGEDELAY", PowerValue("DAMAGE_SINGLE_HOLY", 4, 4, 4, true, list<string>())),
				pair<string, PowerValue>("HEAVENSTRIKE", PowerValue("HEAVENSTRIKE", 30, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
				list<string>({ "DEALDAMAGE", "DEALDAMAGE_HOLY" }), -1);

		skillDefinitions["Light of Day"] = Skill("Light of Day", "Light of Day", "Hagiomancy", SKILLICON_LIGHTOFDAY, 5, 0, 0, "ALLUNDEADORDEMONICFOES",
			list<string>({ "LIGHT OF DAY" }),
			list<string>({ "MAGICAL","HOLY", "TARGETSFOES" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("DAMAGE_HOLY_1", PowerValue("DAMAGE_HOLY_1", 30, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))) }),
			list<string>({ "KILLUNDEAD", "KILLDEMONS" }), -1);

		skillDefinitions["Exile"] = Skill("Exile", "Exile", "Hagiomancy", SKILLICON_EXILE, 10, 0, 10, "SINGLESUMMONEDCREATURE",
			list<string>({ "EXILE" }),
			list<string>({ "MAGICAL","HOLY", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RECHARGE", PowerValue("RECHARGE", 10, 0, 10, false, list<string>({ "INTELLIGENCE", "HOLYBOOST", "MAGICALRECHARGE","HOLYRECHARGE"}))),
				}),
				list<string>({ "KILLSUMMON" }), -1);

		skillDefinitions["Strength of Reason"] = Skill("Strength of Reason", "Strength of Reason", "Hagiomancy", SKILLICON_STRENGTHOFREASON, 10, 0, 1, "SINGLEALLY",
			list<string>({ "APPLY_Strength of Reason_SINGLE" }),
			list<string>({ "MAGICAL","HOLY","BOON","TARGETSALLIES" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Strength of Reason", PowerValue("DURATION_Strength of Reason", 5, 0, 10, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))),
				pair<string, PowerValue>("POWER_Strength of Reason", PowerValue("POWER_Strength of Reason", 8, 0, 999, true, list<string>({ "INTELLIGENCE", "HOLYBOOST"}))), }),
				list<string>({ "ENCHANTSELF","ENCHANTALLY", "PHYSICALBUFFSELF","PHYSICALBUFFALLY" }), HEALWOUNDS_WAV);

		// SANGROMANCY
		skillDefinitions["Life Drain"] = Skill("Life Drain", "Life Drain", "Sangromancy", SKILLICON_LIFEDRAIN, 10, 0, 1, "SINGLEFOE",
			list<string>({ "APPLY_Life Drain_SINGLE", "LIFESTEAL_SINGLE_UNHOLY" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Life Drain", PowerValue("DURATION_Life Drain", 5, 5, 8, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("POWER_Life Drain", PowerValue("POWER_Life Drain", 5, 1, 10, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				pair<string, PowerValue>("LIFESTEAL_SINGLE_UNHOLY", PowerValue("POWER_Life Drain", 5, 1, 10, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"})))
				}),
			list<string>({ "DEALDAMAGE", "HEALSELF" }), 1060);

		skillDefinitions["Atrophy"] = Skill("Atrophy", "Atrophy", "Sangromancy", SKILLICON_ATROPHY, 5, 0, 1, "SINGLEFOE",
			list<string>({ "APPLY_WEAKNESS_SINGLE" }),
			list<string>({ "MAGICAL","BLOOD","UNHOLY" }),
			Map<string, PowerValue>({ pair<string, PowerValue>("DURATION_1", PowerValue("DURATION_1", 3, 0, 8, true, list<string>({ "INTELLIGENCE", "BLOODBOOST"}))),
				}),
			list<string>({ "CURSEFOE", }), -1);

		// NECROMANCY
		skillDefinitions["Animate Skeleton Warrior"] = Skill("Animate Skeleton Warrior", "Animate Skeleton Warrior", "Necromancy", SKILLICON_ANIMATESKELETONWARRIOR, 55, 1, 8, "SELF", // debug 0, real = 2
			list<string>({ "SUMMON_Skeleton Warrior" }),
			list<string>({ "MAGICAL","UNHOLY", "ELITE", "SUMMON" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("LIFE", PowerValue("LIFE", 80, 0, 999, true, list<string>({ "INTELLIGENCE", "UNHOLYBOOST"}))),
				pair<string, PowerValue>("STRENGTH", PowerValue("STRENGTH", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "UNHOLYBOOST"}))),
				}),
				list<string>({ "SUMMON", }), 5686);

		// METEOMANCY
		skillDefinitions["Rainstorm"] = Skill("Rainstorm", "Rainstorm", "Meteomancy", SKILLICON_RAINSTORM, 15, 0, 0, "ALL", // 1
			list<string>({ "APPLY_Rainstorm_WORLD", "APPLY_WET_ALL" }),
			list<string>({ "MAGICAL","WATER","ELEMENTAL" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Rainstorm", PowerValue("DURATION_Rainstorm", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				pair<string, PowerValue>("DURATION_WET", PowerValue("DURATION_WET", 5, 0, 999, true, list<string>({ "INTELLIGENCE", "WATERBOOST", "ELEMENTALBOOST", "WEATHERBOOST"}))),
				}),
				list<string>({ "MAKEITRAIN" }), RAINSTORM_WAV);

		// ELECTROMANCY
		skillDefinitions["Plasma Pulse"] = Skill("Plasma Pulse", "Plasma Pulse", "Electromancy", SKILLICON_PLASMAPULSE, 35, 2, 5, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_ELECTRIC", "APPLY_CONCUSSED_SINGLE" }),
			list<string>({ "MAGICAL","ELECTRIC", "ELEMENTAL", "ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_ELECTRIC", PowerValue("DAMAGE_SINGLE_ELECTRIC", 70, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DURATION_CONCUSSED", PowerValue("DURATION_CONCUSSED", 2, 0, 999, true, list<string>({ "INTELLIGENCE", "ELECTRICBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", }), -1);

		// DUAL WEP MASTERY
		skillDefinitions["Doublestrike"] = Skill("Doublestrike", "Doublestrike", "Dual Weapon Mastery", SKILLICON_DOUBLESTRIKE, 20, 0, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", "DAMAGE_SINGLE_PHYSICAL" }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL", PowerValue("DAMAGE_SINGLE_PHYSICAL", 11, 0, 999, true, list<string>({ "STRENGTH", "Dual Weapon MasteryBOOST"}))), }),
				list<string>({ "DEALDAMAGE" }), DOUBLESTRIKE_1_WAV);

		skillDefinitions["Serrated Strike"] = Skill("Serrated Strike", "Serrated Strike", "Dual Weapon Mastery", SKILLICON_SERRATEDSTRIKE, 15, 0, 2, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", "APPLY_BLEEDING_SINGLE" }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL", PowerValue("DAMAGE_SINGLE_PHYSICAL", 12, 0, 999, true, list<string>({ "STRENGTH", "Dual Weapon MasteryBOOST"}))),
				pair<string, PowerValue>("DURATION_BLEEDING", PowerValue("DURATION_BLEEDING", 3, 0, 999, true, list<string>({ "STRENGTH", "Dual Weapon MasteryBOOST"}))), }),
				list<string>({ "DEALDAMAGE" }), SERRATEDSTRIKE_WAV);

		// UMBROMANCY
		skillDefinitions["Shadow Spike"] = Skill("Shadow Spike", "Shadow Spike", "Umbromancy", SKILLICON_SHADOWSPIKE, 15, 0, 0, "SINGLEFOE",
			list<string>({ "INTERRUPT_AOE_SHADOW", "APPLY_BLIND_AOE" }),
			list<string>({ "MAGICAL","ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("RECHARGE", PowerValue("RECHARGE", 8, 1, 5, false, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))),
				pair<string, PowerValue>("DURATION_BLIND", PowerValue("DURATION_BLIND", 1, 1, 1, false, list<string>())),
				}),
				list<string>({ "BLINDFOE", "INTERRUPTFOE" }), SHADOWSPIKE_WAV);

		skillDefinitions["Chaos Storm"] = Skill("Chaos Storm", "Chaos Storm", "Umbromancy", SKILLICON_CHAOSSTORM, 30, 1, 5, "ALLFOES",
			list<string>({ "CHAOSSTORM" }),
			list<string>({ "MAGICAL","ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_MANA", PowerValue("DAMAGE_MANA", 12, 1, 99, true, list<string>({ "INTELLIGENCE", "SHADOWBOOST"}))), }),
				list<string>({ "MANADAMAGE" }), -1);

		// 1H WEP MASTERY
		skillDefinitions["Fine Strike"] = Skill("Fine Strike", "Fine Strike", "1H Weapon Mastery", SKILLICON_FINESTRIKE, 5, 0, 0, "SINGLEFOE",
			list<string>({ "DAMAGE_SINGLE_PHYSICAL", }),
			list<string>({ "PHYSICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_SINGLE_PHYSICAL",PowerValue("DAMAGE_SINGLE_PHYSICAL",10,0,999,true,list<string>({"STRENGTH"}))),
				pair<string, PowerValue>("CRITICALBOOST", PowerValue("CRITICALBOOST", 40, 0, 100, true, list<string>({ "STRENGTH", "1H Weapon MasteryBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE" }), -1);

		// WAYFARING
		skillDefinitions["Gentleman's Riposte"] = Skill("Gentleman's Riposte", "Gentleman's Riposte", "Wayfaring", SKILLICON_GENTLEMANSRIPOSTE, 5, 0, 10, "SELF",
			list<string>({ "APPLY_Gentleman's Riposte_SELF", }),
			list<string>({ "PHYSICAL","ELITE" }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DURATION_Gentleman's Riposte", PowerValue("DURATION_Gentleman's Riposte", 2, 0, 15, true, list<string>({ "STRENGTH", "WayfaringBOOST"}))), }),
				list<string>({ "DEALDAMAGE" }), GENTLEMANRIPOSTE_WAV);

		// PYROMANCY
		skillDefinitions["Brilliant Spark"] = Skill("Brilliant Spark", "Brilliant Spark", "Pyromancy", SKILLICON_BRILLIANTSPARK, 20, 0, 1, "SINGLEFOE",
			list<string>({ "DAMAGE_FIRE_SINGLE", "APPLY_BURNING_SINGLE" }),
			list<string>({ "MAGICAL", }),
			Map<string, PowerValue>({
				pair<string, PowerValue>("DAMAGE_FIRE_SINGLE", PowerValue("DAMAGE_FIRE_SINGLE", 25, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				pair<string, PowerValue>("DURATION_BURNING", PowerValue("DURATION_BURNING", 3, 0, 999, true, list<string>({ "INTELLIGENCE", "FIREBOOST", "ELEMENTALBOOST"}))),
				}),
				list<string>({ "DEALDAMAGE", "APPLY_BURNING_SINGLE" }), BRILLIANTSPARK_WAV);

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
		equipmentDefinitions["Withered Secespita"] = Equipment("Withered Secespita", "Weapon", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("PIETY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Suero's Blade"] = Equipment("Suero's Blade", "Weapon", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("STRENGTH",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Blades of House JaqMaq"] = Equipment("Blades of House JaqMaq", "Weapon", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("AGILITY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Tibetan Tie Bian"] = Equipment("Tibetan Tie Bian", "Weapon", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("ENERGYREGENPLUS",5.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Roger Bacon's Quill"] = Equipment("Roger Bacon's Quill", "Weapon", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("INTELLIGENCE",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);

		// ARMOUR
		equipmentDefinitions["Vatican Vestiments"] = Equipment("Vatican Vestiments", "Armour", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("HOLYBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Martin's Cloak"] = Equipment("Martin's Cloak", "Armour", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("PHYSICALARMOUR",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Alhambran Tunic"] = Equipment("Alhambran Tunic", "Armour", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("SHADOWBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Insulating Gloves"] = Equipment("Insulating Gloves", "Armour", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("ELECTRICARMOUR",5.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Ming Theatre Costume"] = Equipment("Ming Theatre Costume", "Armour", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("BLOODBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);

		// ACCESSORIES
		equipmentDefinitions["Cross of St Jeanne-Marie"] = Equipment("Cross of St Jeanne-Marie", "Accessory", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("PIETY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Matteo Carreri's Locket"] = Equipment("Matteo Carreri's Locket", "Accessory", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("WAYFARINGBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Theoricae Novae Planetarum"] = Equipment("Theoricae Novae Planetarum", "Accessory", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("INTELLIGENCE",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Lunyu Page Fragment"] = Equipment("Lunyu Page Fragment", "Accessory", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("UNHOLYBOOST",1.0f,true,false),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Mask of House JaqMaq"] = Equipment("Mask of House JaqMaq", "Accessory", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("AGILITY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 250);
		equipmentDefinitions["Joan of Arc's Necklace"] = Equipment("Joan of Arc's Necklace", "Accessory", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("INTELLIGENCE",5.0f,true,true),
			Combat::Effect("FIREBOOST",1.5f,true,false),
			Combat::Effect("SELFIMMOLATE",0.0f,true,true),
			}), "ELITESKILLYELLOW", 9999);
		equipmentDefinitions["The Eyes of St Lucy"] = Equipment("The Eyes of St Lucy", "Accessory", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("HOLYBOOST",0.5f,true,false),
			Combat::Effect("PIETY",1.0f,true,true),
			}), "EQUIPMENTBLUE", 150);
		equipmentDefinitions["al-Hajar al-Aswad"] = Equipment("al-Hajar al-Aswad", "Accessory", UNIMPLEMENTED_IMAGE, List<Combat::Effect>({
			Combat::Effect("HOLYBOOST",1.3f,true,false),
			Combat::Effect("UNHOLYBOOST",1.3f,true,false),
			}), "ELITESKILLYELLOW", 9999);

	}
	void defineAllCombatants() {
		//DEBUG
		definedCombatants["SadBag"] = Combatant("SadBag", "SadBag", Map<string, int>({
				pair<string, int>("VITALITY", 0),
			}),
			Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("SadBag", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("SadBag", "COMBAT_FRONT")),
					})),
					pair<string, Map<string, string>>("Effects", Map<string, string>({
						pair<string, string>("UNDEAD", "1"),
						pair<string, string>("CONCUSSED", "999"),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("0", "Brilliant Spark"),
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
				pair<string, int>("INTELLIGENCE", 1),
				}),
				Map<string, Map<string, string>>({
					pair<string, Map<string, string>>("Images", Map<string, string>({
						pair<string, string>("Back", imageLookup.getSequenceAsString("EnragedMagician", "COMBAT_BACK")),
						pair<string, string>("Front", imageLookup.getSequenceAsString("EnragedMagician", "COMBAT_FRONT")),
					})),
					pair <string,Map<string, string>>("equippedSkillNames", Map<string, string>({
						pair<string, string>("1", "Brilliant Spark"),
						})),
					}));
	}
	void defineAllTeams() {
		// pair is leader -> team
		// DEBUG
		definedTeams["DEBUG"] = { "SadBag", List<Combatant>({
			definedCombatants["SadBag"], definedCombatants["SadBag"] }) };

		// EVENT
		definedTeams["EVENT1"] = { "EnragedVilomah", List<Combatant>({
			definedCombatants["EnragedMagician"],
			definedCombatants["EnragedPriest"],
			definedCombatants["EnragedVilomah"],
			definedCombatants["EnragedVagrant"], }) };

		definedTeams["EVENT2"] = { "Father Michelet", List<Combatant>({
			definedCombatants["Father Michelet"],}) };

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

		allEffectDefinitions["Rainstorm"] = EffectObject("Rainstorm", "NEUTRAL", SKILLICON_RAINSTORM, "APPLY_WET_ALL", false,
			List<string>(list<string>({ "Rainstorm", })),
			List<string>(list<string>({ "EVERYTURN", "ONAPPLY" })));

		allEffectDefinitions["WET"] = EffectObject("WET", "NEUTRAL", EFFECTICON_WET, "WET", false,
			List<string>(list<string>({ "WET", })),
			List<string>(list<string>({ "ONTAKINGCOLDDAMAGE",  "ONTAKINGELECTRICDAMAGE" })));

		allEffectDefinitions["CONCUSSED"] = EffectObject("CONCUSSED", "BANE", EFFECTICON_CONCUSSED, "CONCUSSED", false,
			List<string>(list<string>({ "CONCUSSED", })),
			List<string>(list<string>({ "WHENCASTINGASPELL" })));

		allEffectDefinitions["BLEEDING"] = EffectObject("BLEEDING", "BANE", EFFECTICON_BLEEDING, "BLEEDING", false,
			List<string>(list<string>({ "BLEEDING", })),
			List<string>(list<string>({ "WHENCASTINGASPELL" })));

		allEffectDefinitions["BLIND"] = EffectObject("BLIND", "BANE", EFFECTICON_BLIND, "BLIND", false,
			List<string>(list<string>({ "BLIND", })),
			List<string>(list<string>({ "WHENATTACKING" })));

		allEffectDefinitions["BURNING"] = EffectObject("BURNING", "BANE", EFFECTICON_BURNING, "BURNING", false,
			List<string>(list<string>({ "BURNING", })),
			List<string>(list<string>({ "EVERYTURN" })));

		allEffectDefinitions["Strength of Reason"] = EffectObject("Strength of Reason", "BOON", SKILLICON_STRENGTHOFREASON, "Strength of Reason", false,
			List<string>(list<string>({ "Strength of Reason", })),
			List<string>(list<string>({ "ONDOINGPHYSICALATTACK", })));

		// permanent
		allEffectDefinitions["UNDEAD"] = EffectObject("UNDEAD", "PERM", EFFECTICON_UNDEAD, "UNDEAD", true,
			List<string>(list<string>({ "UNDEAD", })),
			List<string>(list<string>({ "ONTAKINGHOLYDAMAGE", "ONBEINGHEALED" })));
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
};
Combat combat;
