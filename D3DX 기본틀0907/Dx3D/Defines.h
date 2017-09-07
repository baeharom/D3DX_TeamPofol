#pragma once

//=================상수 디파인=========================
#define WINSIZEX 1200 //창 사이즈
#define WINSIZEY 800

#define MAX_LOADSTRING 100
//===============유틸리티 매크로======================
#define RANDOM(num) (rand()%((num)+1))//(int)(((long)rand() * (num)) / (RAND_MAX + 1))
#define RANDOMA(s,e) ((s) + (RANDOM((e) - (s))))
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

#define MULTIPLY(A, B) (A) * (B)
#define SAFE_DELETE(p) if (p) { delete (p); (p) = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete [] (p); (p) = NULL; }
#define SAFE_RELEASE(p) if (p) (p)->Release();
#define SAFE_ADDREF(p) if (p) (p)->AddRef();

#define SINGLETON(class_name)\
	private:\
		class_name();\
		~class_name();\
	public:\
		static class_name* GetInstance()\
		{\
			static class_name instance;\
			return &instance;\
		}

#define SYNTHESIZE(varType, varName, funName) \
	protected:\
		varType varName;\
	public:\
		inline varType Get##funName() const { return varName; }\
		inline void Set##funName(varType var) { varName = var; }

#define SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
	protected:\
		varType varName;\
	public:\
		inline varType& Get##funName() { return varName; }\
		inline void Set##funName(varType& var) { varName = var; }

#define SYNTHESIZE_ADD_REF(varType, varName, funName)\
	protected:\
		varType varName;\
	public:\
		inline varType Get##funName() { return varName; }\
		inline void Set##funName(varType var) {\
			if (varName != var) {\
				SAFE_RELEASE(varName);\
				SAFE_ADDREF(var);\
				varName = var;\
			}\
		}
//===========================================================
