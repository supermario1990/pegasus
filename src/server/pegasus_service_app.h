// Copyright (c) 2017, Xiaomi, Inc.  All rights reserved.
// This source code is licensed under the Apache License Version 2.0, which
// can be found in the LICENSE file in the root directory of this source tree.

#pragma once

#include <dsn/dist/replication/meta_service_app.h>
#include <dsn/dist/replication/replication_service_app.h>
#include "pegasus_counter_updater.h"
#include "pegasus_perf_counter.h"

namespace pegasus {
namespace server {

class pegasus_replication_service_app : public ::dsn::replication::replication_service_app
{
public:
    pegasus_replication_service_app(dsn_gpid pid)
        : ::dsn::replication::replication_service_app::replication_service_app(pid),
          _updater_started(false)
    {
    }

    virtual ~pegasus_replication_service_app() {}

    virtual ::dsn::error_code start(int argc, char **argv) override
    {
        ::dsn::error_code ret = ::dsn::replication::replication_service_app::start(argc, argv);
        if (ret == ::dsn::ERR_OK) {
            pegasus_counter_updater::instance().start();
            _updater_started = true;
        }
        return ret;
    }

    virtual ::dsn::error_code stop(bool cleanup = false) override
    {
        ::dsn::error_code ret = ::dsn::replication::replication_service_app::stop();
        if (_updater_started) {
            pegasus_counter_updater::instance().stop();
        }
        return ret;
    }

private:
    bool _updater_started;
};

class pegasus_meta_service_app : public ::dsn::service::meta_service_app
{
public:
    pegasus_meta_service_app(dsn_gpid pid)
        : ::dsn::service::meta_service_app::meta_service_app(pid), _updater_started(false)
    {
    }

    virtual ~pegasus_meta_service_app() {}

    virtual ::dsn::error_code start(int argc, char **argv) override
    {
        ::dsn::error_code ret = ::dsn::service::meta_service_app::start(argc, argv);
        if (ret == ::dsn::ERR_OK) {
            pegasus_counter_updater::instance().start();
            _updater_started = true;
        }
        return ret;
    }

    virtual ::dsn::error_code stop(bool cleanup = false) override
    {
        ::dsn::error_code ret = ::dsn::service::meta_service_app::stop();
        if (_updater_started) {
            pegasus_counter_updater::instance().stop();
        }
        return ret;
    }

private:
    bool _updater_started;
};
}
} // namespace
