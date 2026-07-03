#include <WorldMap.h>
#include <Role.h>
#include <Game.h>
#include <Enemy.h>
#include <WorldMap.h>
#include <Role.h>
#include <KeyBoard.h>
#include <EquipmentTable.h>
#include "Dice.h"
#include "Color.h"
#include "Field.h"
#include "EquipmentTable.h"
#include "ItemTable.h"
#include "Tent.h"
#include "Chest.h"
#include "Store.h"
#include "Backpack.h"
#include "Item.h"
#include "SkillTable.h"

void Game::Initialize() {
    SkillTable::Initialize();
    EquipmentTable::Initialize();
    ItemTable::Initialize();

    static Role Kazusa("杏山千紗", 30, 14);
    Kazusa.equipForce("WoodenSword");
    Kazusa.equipForce("LaurelWreath");
    Kazusa.equipForce("Bracelet");
    static Role Shiroko("砂狼白子", 34, 14);
    Shiroko.equipForce("RitualSword");
    Shiroko.equipForce("Robe");
    Shiroko.equipForce("Shoes");
    static Role Hoshino("小鳥游星野", 32, 16);
    Hoshino.equipForce("Hammer");
    Hoshino.equipForce("PlateArmor");
    Hoshino.equipForce("HolyGrail");

    Kazusa.GetAttribute().SetMaxFocus(5);
    Kazusa.GetAttribute().SetFocus(5);
    Kazusa.GetAttribute().SetSPD(70);
    Shiroko.GetAttribute().SetMaxFocus(5);
    Shiroko.GetAttribute().SetFocus(5);
    Shiroko.GetAttribute().SetSPD(70);
    Hoshino.GetAttribute().SetMaxFocus(5);
    Hoshino.GetAttribute().SetFocus(5);
    Hoshino.GetAttribute().SetSPD(70);

    Kazusa.addItemToBackpack("Tent");
    Kazusa.addItemToBackpack("Tent");
    Kazusa.addItemToBackpack("Tent");
    Kazusa.addItemToBackpack("Godsbeard");
    Kazusa.addItemToBackpack("Godsbeard");
    Kazusa.addItemToBackpack("Godsbeard");
    Kazusa.addItemToBackpack("GoldenRoot");
    Kazusa.addItemToBackpack("GoldenRoot");
    Kazusa.addItemToBackpack("GoldenRoot");
    Kazusa.addItemToBackpack("TeleportScroll");
    Kazusa.addItemToBackpack("TeleportScroll");
    Kazusa.addItemToBackpack("TeleportScroll");

    Kazusa.GetSkill().pushActive(SkillTable::activeMap.find("燕返．TsubameGaeshi")->second);
    Kazusa.GetSkill().pushActive(SkillTable::activeMap.find("拔刀．Battojutsu")->second);
    Hoshino.GetSkill().pushActive(SkillTable::activeMap.find("燕返．TsubameGaeshi")->second);
    Hoshino.GetSkill().pushActive(SkillTable::activeMap.find("拔刀．Battojutsu")->second);
    Shiroko.GetSkill().pushActive(SkillTable::activeMap.find("燕返．TsubameGaeshi")->second);
    Shiroko.GetSkill().pushActive(SkillTable::activeMap.find("拔刀．Battojutsu")->second);

    Kazusa.addItemToBackpack("浪人宗劍．ProdigalKatana");
    Kazusa.addItemToBackpack("浪人宗劍．ProdigalKatana");
    Kazusa.addItemToBackpack("浪人宗劍．ProdigalKatana");

    Kazusa.renewPlayer();
    Shiroko.renewPlayer();
    Hoshino.renewPlayer();

    roles = { &Kazusa ,&Shiroko ,&Hoshino };

    sortExecutionRole();

    static Enemy EnemyA1("城堡守衛-盾", 28, 5);
    static Enemy EnemyA2("城堡守衛-劍", 26, 7);
    static Enemy EnemyA3("城堡守衛-魔法", 24, 5);

    EnemyA1.equipForce("WoodenShield");
    EnemyA2.equipForce("WoodenSword");
    EnemyA3.equipForce("MagicWand");

    //吸血鬼洞穴
    static Enemy EnemyB1("穴居魔物", 7, 3);
    static Enemy EnemyB2("蝙蝠眷屬", 5, 6);
    static Enemy EnemyB3("蝙蝠眷屬", 3, 4);
    static Enemy EnemyB4("洞穴哨兵", 13, 3);

    EnemyB1.equipForce("Hammer");
    EnemyB1.GetAttribute().SetMaxHP(99);
    EnemyB1.GetAttribute().SetHP(99);
    EnemyB1.GetAttribute().SetPA(10);
    EnemyB1.GetAttribute().SetPD(10);
    EnemyB1.GetAttribute().SetACC(20);
    EnemyB1.GetAttribute().SetSPD(20);
    EnemyB1.renewPlayer();
    EnemyB2.GetAttribute().SetMaxHP(20);
    EnemyB2.GetAttribute().SetHP(20);
    EnemyB3.GetAttribute().SetMaxHP(20);
    EnemyB3.GetAttribute().SetHP(20);

    //不死族地堡
    static Enemy EnemyC1("骷髏守衛A", 4, 10);
    static Enemy EnemyC2("骷髏守衛B", 4, 11);
    static Enemy EnemyC3("骷髏守衛C", 4, 12);
    static Enemy EnemyC4("精銳守衛", 1, 11);
    static Enemy EnemyC5("殭屍哨兵", 9, 9);
    static Enemy EnemyC6("殭屍哨兵", 11, 15);

    EnemyC1.GetAttribute().SetPA(10);
    EnemyC1.equipForce("LeatherArmor");
    EnemyC2.GetAttribute().SetPA(10);
    EnemyC2.equipForce("LeatherArmor");
    EnemyC3.GetAttribute().SetPA(10);
    EnemyC3.equipForce("LeatherArmor");
    EnemyC4.GetAttribute().SetSPD(25);
    EnemyC4.GetAttribute().SetPA(30);
    EnemyC4.equipForce("GiantHammer");
    EnemyC5.equipForce("Shoes");
    EnemyC5.equipForce("RitualSword");
    EnemyC6.equipForce("Shoes");
    EnemyC6.equipForce("RitualSword");

    //哥布林堡壘
    static Enemy EnemyD1("哥布林A", 44, 6);
    static Enemy EnemyD2("哥布林B", 44, 6);
    static Enemy EnemyD3("哥布林C", 44, 6);
    static Enemy EnemyD4("哥布林D", 44, 2);
    static Enemy EnemyD5("哥布林E", 44, 2);

    EnemyD1.GetAttribute().SetSPD(99);
    EnemyD2.GetAttribute().SetSPD(99);
    EnemyD3.GetAttribute().SetSPD(99);
    EnemyD4.GetAttribute().SetSPD(99);
    EnemyD5.GetAttribute().SetSPD(99);

    //森林區
    static Enemy EnemyE1("野狼", 51, 15);
    static Enemy EnemyE2("野豬A", 45, 14);
    static Enemy EnemyE3("野豬B", 46, 12);
    static Enemy EnemyE4("野豬C", 46, 16);
    static Enemy EnemyE5("野狼", 21, 13);
    static Enemy EnemyE6("野豬", 24, 15);
    static Enemy EnemyE7("野狼", 38, 20);
    static Enemy EnemyE8("野豬", 26, 23);

    //水域
    static Enemy EnemyF1("海妖守衛", 63, 1);
    static Enemy EnemyF2("迷路的海妖", 58, 2);
    static Enemy EnemyF3("海妖", 55, 9);

    //赤龍峽谷
    static Enemy EnemyG1("迷路的赤龍", 65, 14);
    static Enemy EnemyG2("赤龍幼龍", 62, 19);
    static Enemy EnemyG3("赤龍幼龍", 57, 21);
    static Enemy EnemyG4("\033[31m赤龍\033[0m", 55, 25);
    static Enemy EnemyG5("\033[31m赤龍\033[0m", 48, 25);
    static Enemy EnemyG6("\033[31m赤龍\033[0m", 59, 27);
    static Enemy EnemyG7("\033[31m赤龍\033[0m", 64, 22);
    static Enemy EnemyG8("\033[31m赤龍\033[0m", 61, 23);

    //陰暗地牢
    static Enemy EnemyH1("女巫", 22, 24);
    static Enemy EnemyH2("邪惡巫師", 19, 22);
    static Enemy EnemyH3("巨劍騎士", 9, 24);
    static Enemy EnemyH4("邪教教徒", 20, 27);
    static Enemy EnemyH5("惡魔術士", 25, 27);
    static Enemy EnemyH6("魅魔", 13, 21);

    //野生BOSS
    static Enemy Boss1("【強敵】浪人劍豪", 4, 19); //浪人宗劍 ProdigalKatana 拔刀 居合 燕返
    static Enemy Boss2("【強敵】至尊魔導", 58, 15); //禁忌秘典 TabooGrimoire 殞星 雷擊 霜凍
    static Enemy Boss3("【強敵】銀狼", 53, 4); //銀狼毛皮 SilverWolfFur 
    static Enemy Boss4("【強敵】青龍", 18, 7); //青龍鱗鎧 DragonScaleArmor 
    static Enemy Boss5("【強敵】墮落的聖騎士", 36, 5); //聖芒鎖鍊 SacredChain 
    static Enemy Boss6("【強敵】無頭騎士", 35, 28); //詛咒盾牌 CursedShield 挑釁 

    //主題BOSS
    static Enemy BossA("【威脅】巨錘領主", 26, 3); //黃金巨錘 GoldenHammer 
    static Enemy BossB("【威脅】吸血鬼", 4, 5); //暗夜披風 CloakOfDarkness 吸血 
    static Enemy BossC("【威脅】不死族之源", 0, 15); //不朽圖騰 UndeadTotem 吸血 汙染

    static Enemy BossD("【威脅】哥布林祭司", 44, 2); //祭司權杖 PriestScepter 殞星
    BossD.GetAttribute().SetSPD(99);
    BossD.equipForce("HolyGrail");
    BossD.equipForce("MagicWand");

    static Enemy BossE1("【威脅】魔化野獸", 49, 9); //野獸皮毛 Animalfur 
    static Enemy BossE2("【威脅】森之妖精", 21, 17); //王♂之財寶 Gate♂OfBabylon
    static Enemy BossF1("【威脅】海妖之王", 69, 3); //漩湧之眼 EyeOfMaelstrom
    static Enemy BossG1("【威脅】燄之赤龍", 68, 23); //燄龍聖劍 FlameRetardantDragonSword
    static Enemy BossG2("【威脅】龍族後裔", 69, 17); //龍族秘卷 ScrollOfTheDragon
    static Enemy BossG3("【威脅】墮落屠龍者", 59, 27); //赤龍血鎧 CrimsonDragonArmor
    static Enemy BossH1("【威脅】巫妖", 4, 25); //褻瀆的吊墜 ProfanePendant
    static Enemy BossH2("【威脅】石像鬼", 16, 27); //惡魔之翼 WingsOfDevil

    //帳篷
    static Tent tentA1("\033[33m聖城大本營\033[0m", 32, 14, 999);
    static Tent tentA2("\033[33m被佔領的城堡\033[0m", 26, 5, 999);
    static Tent tentB("\033[31m穴地前哨站\033[0m", 15, 1, 999);
    static Tent tentC("\033[31m地堡前哨站\033[0m", 6, 13, 999);
    static Tent tentD("\033[32m哥布林巢穴\033[0m", 44, 0, 999);
    static Tent tentE("\033[32m森林隘口營地\033[0m", 42, 14, 999);
    static Tent tentF("\033[46m淺水區前哨站\033[0m", 50, 6, 999);
    static Tent tentG("\033[1m\033[35m陰暗潮濕洞穴\033[0m", 34, 28, 999);
    static Tent tentH("\033[33m地牢哨站\033[0m", 23, 21, 999);

    static Store storeA("城堡商店", 32, 13);
    static Store storeB("穴地雜貨商", 16, 1);

    static Chest chestA1(3, 6);

    roles = { &Kazusa ,&Shiroko ,&Hoshino };
    //stores = { &storeA };
    //enemys = {  };
    //tents = { &tentA1 ,&tentA2 };
    chests = { &chestA1 };

    WorldMap::SetTents(tents);
    WorldMap::SetRoles(roles);
    WorldMap::SetEnemys(enemys);
    WorldMap::SetStores(stores);
    WorldMap::SetChests(chests);

    WorldMap::loadMap("map.txt");

    return;
}
