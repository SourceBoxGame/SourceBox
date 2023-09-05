#ifndef CQSCRIPT_H
#define CQSCRIPT_H
#ifdef _WIN32
#pragma once
#endif

#include "qscript.h"
#include "qscript_language.h"
#include "qscript_structs.h"



class CQScript : public IQScript
{
public:
	CQScript();
	virtual void Initialize();
	virtual bool Connect(CreateInterfaceFn factory);
	virtual InitReturnVal_t Init();
	virtual void Shutdown();
	virtual QScriptModule CreateModule(const char* name);
	virtual QScriptFunction CreateModuleFunction(QScriptModule module, const char* name, const char* args, QCFunc func);
	virtual const char* GetArgString(QScriptArgs args, int argnum);
	virtual char* GetArgPermaString(QScriptArgs args, int argnum);
	virtual int GetArgInt(QScriptArgs args, int argnum);
	virtual float GetArgFloat(QScriptArgs args, int argnum);
	virtual bool GetArgBool(QScriptArgs args, int argnum);
	virtual QScriptCallback GetArgCallback(QScriptArgs args, int argnum);
	virtual void LoadMods(const char* filename);
	virtual void LoadModsInDirectory(const char* folder, const char* filename);
	virtual void CallCallback(QScriptCallback callback, QScriptArgs args);
	virtual QScriptArgs CreateArgs(const char* types, ...);
	virtual void FreeArgs(QScriptArgs a);
private:
	virtual void LoadFilesInDirectory(const char* folder, const char* filename);
	void AddScriptingInterface(const char* name, CreateInterfaceFn factory);

	CUtlVector<QModule*>* m_modules;
	CUtlVector<IBaseScriptingInterface*>* m_interfaces;
	
};


#endif