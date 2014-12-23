#ifndef __EVOLUTION_GRAPHIC_EXCEPTION_H__
#define __EVOLUTION_GRAPHIC_EXCEPTION_H__

#include "../evolution_graphic_result.h"

namespace EVOLUTION{
    namespace GRAPHIC{
        //-----------------------
        //Exception管理クラス
        //-----------------------
        class ExceptionManager{
        public:
            static void ExceptionAssert(GraphicException::_EXCEPTION exception);
        };
    }
}
#if defined(_DEBUG)
#define EVOLUTION_GRAPHIC_EXCEPTION_ASSERT(_Exception) EVOLUTION::GRAPHIC::ExceptionManager::ExceptionAssert(_Exception);
#else
#define EVOLUTION_GRAPHIC_EXCEPTION_ASSERT(_Exception) (void)(0);
#endif
#endif //!__EVOLUTION_GRAPHIC_EXCEPTION_H__