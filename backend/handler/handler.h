#ifndef HANDLER_H
#define HANDLER_H

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/server/HttpRequestHandler.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include <stdio.h>


#include "cpu.h"
#include "machine.hpp"

#define O_UNUSED(x) (void)x;

#include OATPP_CODEGEN_BEGIN(DTO) ///< Begin DTO codegen section
class UserDto : public oatpp::DTO {

  DTO_INIT(UserDto, DTO)

  DTO_FIELD(UInt32, time, "time");
  DTO_FIELD(Float32, cpu, "cpu");

};
#include OATPP_CODEGEN_END(DTO) ///< END DTO codegen section

// 自定义请求处理程序
class Handler : public oatpp::web::server::HttpRequestHandler
{
public:
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest> &request);
    // 处理传入的请求，并返回响应
};

#endif // HANDLER_H

