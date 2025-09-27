#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "Utils.hpp"

using namespace geode::prelude;

class $modify(LevelEditorLayerHook, LevelEditorLayer) {
    GameObject* createObject(int p0, CCPoint p1, bool p2) {
        GameObject* ret = LevelEditorLayer::createObject(p0, p1, p2);

        if (!p2 && ret && ret->m_isStartPos) setupStartpos(static_cast<StartPosObject*>(ret));
        
        return ret;
    }

    void setupStartpos(StartPosObject* startPos) {
        std::array<std::pair<float, GameObject*>, 7> closestObjs;
        closestObjs.fill({-FLT_MAX, nullptr});

        float endX = startPos->getPositionX();

        for (auto obj : CCArrayExt<GameObject>(m_objects)) {
            if (obj->m_isDecoration) continue;

            auto entry = Utils::objToEntry.find(obj->m_objectID);
            if (entry == Utils::objToEntry.end()) continue;

            float objX = obj->getPositionX();

            if (closestObjs[entry->second].first <= objX && endX >= objX && !obj->m_isNoTouch) {
                closestObjs[entry->second] = {objX, obj};
            }
        }

        bool specialFlip = false;

        for (int i = 0; i < 7; i++) {
            if (i == 6) continue;
            
            auto& pair = closestObjs[i];
            if (i == 2 && closestObjs[i].first < closestObjs[6].first) {
                specialFlip = true;
                pair = closestObjs[6];
            }
            
            // rawr this is a great way of doing things !
            if (!Settings::enabledModes[i]) pair.second = nullptr;
            
            int id = pair.second ? pair.second->m_objectID : 0;
            switch (i) {
                case 0: {
                    if (!pair.second) {
                        startPos->m_startSettings->m_startMode = m_levelSettings->m_startMode;
                    }
                    else {
                        startPos->m_startSettings->m_startMode = Utils::portalToGamemode[id];
                        // clean startpos compat ;3c
                        startPos->m_isIceBlock = static_cast<EffectGameObject*>(pair.second)->m_cameraIsFreeMode;
                    }
                break; }
                case 1: {
                    startPos->m_startSettings->m_startSpeed = !pair.second 
                        ? m_levelSettings->m_startSpeed
                        : Utils::portalToSpeed[id];
                break; }
                case 2: {
                    if (!pair.second) {
                        startPos->m_startSettings->m_isFlipped = m_levelSettings->m_isFlipped;
                    }

                    bool flip = specialFlip ? Utils::getPadDirection(pair.second->m_fRotationX) : id == 11;

                    if (Settings::advancedGravity) {
                        for (auto obj : CCArrayExt<GameObject>(m_objects)) {
                            if (obj->m_isDecoration) continue;
                            
                            float objX = obj->getPositionX();
                            if (objX < pair.first || endX < objX) continue;
                            // saw this in bindings and i think its funny so :3c
                            switch (obj->m_objectID) {
                                case 84:
                                case 1022:
                                case 2926:
                                case 1751: {
                                    flip = !flip;
                                break; }
                                default: break;
                            }
                        }
                    }

                    startPos->m_startSettings->m_isFlipped = flip;
                break; }
                case 3: {
                    startPos->m_startSettings->m_startDual = !pair.second 
                        ? m_levelSettings->m_startDual
                        : id == 286;
                break; }
                case 4: {
                    startPos->m_startSettings->m_startMini = !pair.second 
                        ? m_levelSettings->m_startMini
                        : id == 101;
                break; }
                case 5: {
                    startPos->m_startSettings->m_mirrorMode = !pair.second 
                        ? m_levelSettings->m_mirrorMode
                        : id == 45;
                break; }
            }
        }
    }
};