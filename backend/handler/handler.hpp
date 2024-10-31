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
#include OATPP_CODEGEN_END(DTO) ///< END DTO codegen secton

#include OATPP_CODEGEN_BEGIN(DTO) ///< Begin DTO codegen section
class BashRequestDto : public oatpp::DTO {

  DTO_INIT(UserDto, DTO)

  DTO_FIELD(UInt32, time, "time");
  DTO_FIELD(Float32, cpu, "cpu");

};
#include OATPP_CODEGEN_END(DTO) ///< END DTO codegen secton
// 自定义请求处理程序
class InfoHandler : public oatpp::web::server::HttpRequestHandler
{
public:
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest> &request) override
    {
        O_UNUSED(request);
        auto user = UserDto::createShared();
        Machine mach = Machine();
        user->time = mach.get_local_time();
        user->cpu = getCpuUsingInfo();
        auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        oatpp::String json = jsonObjectMapper->writeToString(user);
        printf("%s\n", json->c_str()); ///< print json

        return ResponseFactory::createResponse(Status::CODE_200, json);
    }
};

class BashHandler : public oatpp::web::server::HttpRequestHandler
{
public:
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest> &request) override
    {
        O_UNUSED(request);
        auto jsonstr = request->readBodyToString();

        auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        oatpp::String json = jsonObjectMapper->readFromString(jsonstr);

        printf("%s\n", json->c_str()); ///< print json

        return ResponseFactory::createResponse(Status::CODE_200, json);
    }
};
#endif // HANDLER_H


