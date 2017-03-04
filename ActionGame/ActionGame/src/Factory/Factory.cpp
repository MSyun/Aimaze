//	ファクトリークラス
// 2017. 3. 4	プログラム作成
// author		SyunMizuno


#include	"Factory.h"
#include	"../Allow/Allow.h"
#include	"../Attack/DragonBreath/DragonBreath.h"
#include	"../Attack/DragonFlame/DragonFlame.h"
#include	"../Attack/AttackMagic/AttackMagic.h"
#include	"../Attack/DragonRush/DragonRush.h"
#include	"../Attack/DragonStamp/DragonStamp.h"
#include	"../CameraControll/CameraControll.h"
#include	"../BossInformation/BossInformation.h"
#include	"../Enemy/Dragon/Dragon.h"
#include	"../Enemy/Wizard/Wizard.h"
#include	"../Enemy/Wolf/Wolf.h"
#include	"../PlayerInformation/PlayerInformation.h"
#include	"../Player/Player.h"
#include	"../GameClear/GameClear.h"
#include	"../GameOver/GameOver.h"
#include	"../GameStart/GameStart.h"
#include	"../Gauge/Gauge2.h"
#include	"../Gauge/Gauge3.h"
#include	"../ItemBox/ItemBox.h"
#include	"../LifeberOnHead/LifeberOnHead.h"
#include	"../Pose/Pose.h"
#include	"../Ring/Ring.h"
#include	"../Weapon/Stick/Stick.h"
#include	"../Weapon/Sword/Sword.h"
#include	"../Mission/Mission.h"


IObj* Factory::Create(_GAME_OBJ_FACTORY type, int val) {
	IObj* obj = NULL;

	switch (type) {
	case GAME_OBJ_3D:
		obj = new Obj3D;
		break;

	case GAME_OBJ_ALLOW:
		obj = new Allow;
		break;

	case GAME_OBJ_BREATH:
		obj = new DragonBreath;
		break;

	case GAME_OBJ_FLAME:
		obj = new DragonFlame;
		break;

	case GAME_OBJ_MAGIC:
		obj = new AttackMagic;
		break;

	case GAME_OBJ_RUSH:
		obj = new DragonRush;
		break;

	case GAME_OBJ_STAMP:
		obj = new DragonStamp;
		break;

	case GAME_OBJ_CAMERACONTROLL:
		obj = new CameraControll;
		break;

	case GAME_OBJ_BOSSINFORMATION:
		obj = new BossInformation;
		break;

	case GAME_OBJ_DRAGON:
		obj = new Dragon(val);
		break;

	case GAME_OBJ_WIZARD:
		obj = new Wizard(val);
		break;

	case GAME_OBJ_WOLF:
		obj = new Wolf(val);
		break;

	case GAME_OBJ_PLAYERINFORMATION:
		obj = new PlayerInformation;
		break;

	case GAME_OBJ_PLAYER:
		obj = new Player;
		break;

	case GAME_OBJ_GAMECLEAR:
		obj = new GameClear;
		break;

	case GAME_OBJ_GAMEOVER:
		obj = new GameOver;
		break;

	case GAME_OBJ_GAMESTART:
		obj = new GameStart;
		break;

	case GAME_OBJ_GAUGE2D:
		obj = new Gauge2(val);
		break;

	case GAME_OBJ_GAUGE3D:
		obj = new Gauge3(val);
		break;

	case GAME_OBJ_ITEMBOX:
		obj = new ItemBox;
		break;

	case GAME_OBJ_LIFEBERONHEAD:
		obj = new LifeberOnHead(val);
		break;

	case GAME_OBJ_POSE:
		obj = new Pose;
		break;

	case GAME_OBJ_RING:
		obj = new Ring;
		break;

	case GAME_OBJ_STICK:
		obj = new Stick;
		break;

	case GAME_OBJ_SWORD:
		obj = new Sword;
		break;

	case GAME_OBJ_MISSION:
		obj = new Mission;
		break;
	};

	return obj;
}