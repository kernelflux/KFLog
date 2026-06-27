#include <exception>
#include "comm/kflogger/kflogger.h"

namespace mars_boost {} namespace boost = mars_boost; namespace mars_boost {

    void throw_exception( std::exception const & e ) {
        kflog_fatal2(KFTSF"boost exception:%_", e.what());
    }
}
