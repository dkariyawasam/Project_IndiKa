from pathlib import Path
import runpy,re
root=Path(__file__).resolve().parents[2]
x=runpy.run_path(str(root/'docs/league-terminals/verify.py'))
w,chars=x['w'],x['chars']
chunks=re.findall(r'\.string "(.*)"',(root/'data/maps/RocketLeague_Lobby/text.inc').read_text())
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/ViridianForest/text.inc').read_text().split('ViridianForest_Text_ErikaLookingForRoot::')[1].split('ViridianForest_Text_ErikaShownRoot::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/PalletTown_ProfessorOaksLab/text.inc').read_text().split('PalletTown_ProfessorOaksLab_Text_RivalFedUpWithWaiting::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/PalletTown_ProfessorOaksLab/text.inc').read_text().split('PalletTown_ProfessorOaksLab_Text_OakExplainCatching::')[1].split('PalletTown_ProfessorOaksLab_Text_OakCompleteMonGuideWasMyDream::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/MtMoon_B1F/text.inc').read_text().split('MtMoon_Text_BrockSawHelixShard::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/CeruleanCity_Gym/text.inc').read_text().split('CeruleanCity_Gym_Text_MistyOfferTrade::')[1].split('CeruleanCity_Gym_Text_MistyDeclineTrade::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/CeruleanCity_Gym/text.inc').read_text().split('CeruleanCity_Gym_Text_TakeThisWithYou::')[1].split('CeruleanCity_Gym_Text_ReceivedCascadeBoardFromMisty::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/PokemonTower_3F/text.inc').read_text().split('PokemonTower_3F_Text_SabrinaComeBackWithPokemon::')[1].split('PokemonTower_3F_Text_SabrinaTaughtDreamEater::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/SaffronCity_Gym/text.inc').read_text().split('SaffronCity_Gym_Text_GymGuyPostVictory::')[1].split('SaffronCity_Gym_Text_GymStatue::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/DiglettsCave_Southside_B1F/text.inc').read_text().split('DiglettsCave_Southside_B1F_Text_LtSurgeTaughtVoltTackle::')[0])
chunks+=re.findall(r'gText_(?:SabrinaTrialPhase[45]|ErikaTrialPhase2|BrockTrialPhase2)\[\] = _\("(.*)"\)',(root/'src/strings.c').read_text())
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/CinnabarIsland/text.inc').read_text().split('CinnabarIsland_Text_RivalMansion::')[1].split('CinnabarIsland_Text_BlaineLivedHereSinceBeforeLab::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/Route20/text.inc').read_text().split('Route20_Text_MarinerPostBattle::')[1].split('Route20_Text_NoraIntro::')[0])
chunks+=re.findall(r'\.string "(.*)"',(root/'data/maps/CeladonCity_GameCorner/text.inc').read_text().split('CeladonCity_GameCorner_Text_SwitchBehindPosterPushIt::')[1].split('CeladonCity_GameCorner_Text_PosterStillGuarded::')[0])
for name in ['ViridianCity_Gym','RocketLeague_ChampionsRoom']:
 chunks+=re.findall(r'\.string "(.*)"',(root/f'data/maps/{name}/text.inc').read_text().split('ViridianCity_Gym_Text_ExplainEarthBadgeTakeThis::')[-1].split('ViridianCity_Gym_Text_GymGuyPostVictory::')[0])
for chunk in chunks:
 for line in re.split(r'\\[npl]|\$',chunk):
  line=re.sub(r'\{(?:PLAYER|RIVAL)\}', 'WWWWWWW', line)
  assert sum(w[chars[c]] for c in line)<=208,line
print('PASS: Story guidance and journal wording fits the text width')
