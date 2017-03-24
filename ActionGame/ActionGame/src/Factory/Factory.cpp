//	ファクトリークラス
// 2017. 3. 4	プログラム作成
// author		SyunMizuno


#include	"Factory.h"
#include	"../Object/Allow/Allow.h"
#include	"../Object/Attack/DragonBreath/DragonBreath.h"
#include	"../Object/Attack/DragonFlame/DragonFlame.h"
#include	"../Object/Attack/AttackMagic/AttackMagic.h"
#include	"../Object/Attack/DragonRush/DragonRush.h"
#include	"../Object/Attack/DragonStamp/DragonStamp.h"
#include	"../Object/CameraControll/CameraControll.h"
#include	"../Object/Character/Enemy/BossInformation/BossInformation.h"
#include	"../Object/Character/Enemy/Dragon/Dragon.h"
#include	"../Object/Character/Enemy/Wizard/Wizard.h"
#include	"../Object/Character/Enemy/Wolf/Wolf.h"
#include	"../Object/Character/Player/PlayerInformation/PlayerInformation.h"
#include	"../Object/Character/Player/Player.h"
#include	"../Object/UI/GameClear/GameClear.h"
#include	"../Object/UI/GameOver/GameOver.h"
#include	"../Object/UI/GameStart/GameStart.h"
#include	"../Object/Gauge/2D/Gauge2.h"
#include	"../Object/Gauge/3D/Gauge3.h"
#include	"../Object/ItemBox/ItemBox.h"
#include	"../Object/LifeberOnHead/LifeberOnHead.h"
#include	"../Object/UI/Pose/Pose.h"
#include	"../Object/Ring/Ring.h"
#include	"../Object/Weapon/Stick/Stick.h"
#include	"../Object/Weapon/Sword/Sword.h"
#include	"../Object/UI/Mission/Mission.h"

#include	"../Animator/Dragon/DragonAnimator.h"
#include	"../Animator/Player/PlayerAnimator.h"
#include	"../Animator/Wizard/WizardAnimator.h"
#include	"../Animator/Wolf/WolfAnimator.h"



/*									//
//			オブジェクト生成		//
//									*/
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


/*									//
//			アニメーター生成		//
//									*/
IAnimator* Factory::Create(_GAME_ANIMATOR_FACTORY type) {
	IAnimator* anim = NULL;

	switch (type) {
	case GAME_ANIMATOR_DRAGON:
		anim = new DragonAnimator;
		break;

	case GAME_ANIMATOR_PLAYER:
		anim = new PlayerAnimator;
		break;

	case GAME_ANIMATOR_WIZARD:
		anim = new WizardAnimator;
		break;

	case GAME_ANIMATOR_WOLF:
		anim = new WolfAnimator;
		break;
	};

	return anim;
}