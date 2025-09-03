/*
 * File: Effect.h
 * Desc: Base interface for effects
 * Auth: AI Assistant
 * Date: 2025-09-03
 */

#ifndef LIGHTSYNC_EFFECT_H
#define LIGHTSYNC_EFFECT_H

class Effect {
public:
    virtual ~Effect() {}
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Tick() = 0;
};

#endif // LIGHTSYNC_EFFECT_H


