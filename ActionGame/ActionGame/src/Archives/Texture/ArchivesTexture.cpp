// テクスチャ名集約
// 2017. 1.22	プログラム作成
// Author		SyunMizuno



#include	"ArchivesTexture.h"
#include	"../../GameFramework/framework/Debug/Debug.h"



/*									//
//			コンストラクタ			//
//									*/
ArchivesTexture::ArchivesTexture() {
	Create();
}


/*									//
//			デストラクタ			//
//									*/
ArchivesTexture::~ArchivesTexture() {
}


/*									//
//				検索				//
//									*/
string ArchivesTexture::Find(const string name) {
	auto it = m_Textures.find(name);
	if (it == m_Textures.end()) {
		Debug::LogError("テクスチャ名" + name + "は見つかりませんでした");
		return "";
	}
	return it->second;
}


/*									//
//				作成				//
//									*/
void ArchivesTexture::Create() {
	Add("TitleLogo", "data/TEXTURE/TitleLogo/TitleLogo.png");

	Add("Title/Back", "data/TEXTURE/Title/Back.png");
	Add("Title/Selector", "data/TEXTURE/Title/ber.png");
	Add("Title/Name", "data/TEXTURE/Title/name.png");
	Add("Title/Begin", "data/TEXTURE/Title/begin.png");
	Add("Title/Option", "data/TEXTURE/Title/option.png");
	Add("Title/End", "data/TEXTURE/Title/stop.png");

	Add("Loading/GaugeFrame", "data/TEXTURE/Loading/Frame.png");
	Add("Loading/GaugeIn", "data/TEXTURE/Loading/In.png");
	Add("Loading/L", "data/TEXTURE/Loading/L.png");
	Add("Loading/o", "data/TEXTURE/Loading/o.png");
	Add("Loading/a", "data/TEXTURE/Loading/a.png");
	Add("Loading/d", "data/TEXTURE/Loading/d.png");
	Add("Loading/i", "data/TEXTURE/Loading/i.png");
	Add("Loading/n", "data/TEXTURE/Loading/n.png");
	Add("Loading/g", "data/TEXTURE/Loading/g.png");
	Add("Loading/dot", "data/TEXTURE/Loading/dot.png");

	Add("Particle/Default", "data/TEXTURE/Particle/ParticleCloudWhite.png");
	Add("Fade/Default", "data/TEXTURE/tip.png");
	Add("PressButton", "data/TEXTURE/Title/PressButton.png");
	Add("Toon", "data/TEXTURE/Shader/Toon.png");
	Add("Ring", "data/TEXTURE/Ring/ItemFont.png");
	Add("Player/Locus/1", "data/TEXTURE/Locus/Line.png");
	Add("Player/Locus/2", "data/TEXTURE/Locus/Line1.png");
	Add("Lifeber/Back", "data/TEXTURE/Life/LifeBack.png");
	Add("Lifeber/In", "data/TEXTURE/Life/LifeIn.png");
	Add("Lifeber/Out", "data/TEXTURE/Life/LifeOut.png");
	Add("Lifeber/Frame", "data/TEXTURE/Life/LifeFrame.png");
	Add("Particle/Magic", "data/TEXTURE/Particle/attack.png");
	Add("Particle/Cloud", "data/TEXTURE/Particle/ParticleCloudWhite.png");
	Add("Mission", "data/TEXTURE/Mission/Mission.png");
	Add("Start", "data/TEXTURE/Game/start.png");
	Add("Button/MARU", "data/TEXTURE/Button/Maru.png");
	Add("Dragon/Slash", "data/TEXTURE/Dragon/slash.png");
	Add("DeathEffect", "data/TEXTURE/wave.png");

	Add("Menu/Menu", "data/TEXTURE/Menu/Menu.png");
	Add("Menu/Selector", "data/TEXTURE/Menu/Selector.png");
	Add("Menu/Back", "data/TEXTURE/Menu/Back.png");
	Add("Menu/Option", "data/TEXTURE/Menu/Option.png");
	Add("Menu/Debug", "data/TEXTURE/Menu/Debug.png");
	Add("Menu/End", "data/TEXTURE/Menu/End.png");

	Add("Player/Lifeber", "data/TEXTURE/PlayerLife/life.png");
	Add("Player/Frame", "data/TEXTURE/PlayerLife/frame.png");
	Add("Player/MaxLifeber", "data/TEXTURE/PlayerLife/life_max.png");
	Add("Boss/Lifeber", "data/TEXTURE/PlayerLife/sub.png");
	Add("Boss/Frame", "data/TEXTURE/PlayerLife/frame.png");
	Add("Boss/MaxLifeber", "data/TEXTURE/PlayerLife/sub_max.png");

	Add("GameOver", "data/TEXTURE/Game/GameOver.png");

	Add("GameClear", "data/TEXTURE/Game/GameClear.png");
	Add("GameClear/Ber", "data/TEXTURE/Game/ClearBer.png");
	Add("Cartridge", "data/TEXTURE/Cartridge/cartridge.png");
	Add("SwordTip", "data/TEXTURE/Cartridge/SwordTip.png");
	Add("StickTip", "data/TEXTURE/Cartridge/StickTip.png");
}


/*									//
//				追加				//
//									*/
void ArchivesTexture::Add(string name, string filename) {
	m_Textures.insert(pair<string, string>(name, filename));
}