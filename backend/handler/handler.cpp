
#include "handler.h"

using namespace std;
std::shared_ptr<OutgoingResponse> Handle::handle(const std::shared_ptr<IncomingRequest> &request) override
{
    O_UNUSED(request);
    auto user = UserDto::createShared();
    Machine mach = Machine();
    user->time = mach.get_local_time();
    user->cpu = 2;
    getOsInfo();
    auto jsonObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    oatpp::String json = jsonObjectMapper->writeToString(user); 
    printf("%s\n", json->c_str()); ///< print json

    return ResponseFactory::createResponse(Status::CODE_200, json);
    
}