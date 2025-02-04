/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Microsoft Corporation
 * 
 * -=- Robust Distributed System Nucleus (rDSN) -=- 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * Description:
 *     configuration of rDSN
 *
 * Revision history:
 *     Mar., 2015, @imzhenyu (Zhenyu Guo), first version
 *     xxxx-xx-xx, author, fix bug about xxx
 */

# include <dsn/internal/global_config.h>
# include <thread>
# include <dsn/internal/task_spec.h>
# include <dsn/internal/network.h>
# include <dsn/internal/singleton_store.h>
# include "library_utils.h"

# ifdef __TITLE__
# undef __TITLE__
# endif
# define __TITLE__ "ConfigFile"

namespace dsn {

static bool build_client_network_confs(
    const char* section, 
    /*out*/ network_client_configs& nss,
    network_client_configs* default_spec)
{
    nss.clear();

    const char* keys[128];
    int kcapacity = 128;
    int kcount = dsn_config_get_all_keys(section, keys, &kcapacity);
    dassert(kcount <= 128, "");

    for (int i = 0; i < kcapacity; i++)
    {
        std::string k = keys[i];
        if (k.length() <= strlen("network.client."))
            continue;

        if (k.substr(0, strlen("network.client.")) != std::string("network.client."))
            continue;

        auto k2 = k.substr(strlen("network.client."));
        if (rpc_channel::is_exist(k2.c_str()))
        {
            /*
            ;channel = network_provider_name,buffer_block_size
            network.client.RPC_CHANNEL_TCP = dsn::tools::asio_network_provider,65536
            network.client.RPC_CHANNEL_UDP = dsn::tools::asio_network_provider,65536
            */

            rpc_channel ch = rpc_channel::from_string(k2.c_str(), RPC_CHANNEL_TCP);

            // dsn::tools::asio_network_provider,65536
            std::list<std::string> vs;
            std::string v = dsn_config_get_value_string(section, k.c_str(), "", 
                "network channel configuration, e.g., dsn::tools::asio_network_provider,65536");
            utils::split_args(v.c_str(), vs, ',');

            if (vs.size() != 2)
            {
                printf("invalid client network specification '%s', should be '$network-factory,$msg-buffer-size'\n",
                    v.c_str()
                    );
                return false;
            }
            
            network_client_config ns;
            ns.factory_name = vs.begin()->c_str();
            ns.message_buffer_block_size = atoi(vs.rbegin()->c_str());

            if (ns.message_buffer_block_size == 0)
            {
                printf("invalid message buffer size specified: '%s'\n", vs.rbegin()->c_str());
                return false;
            }

            nss[ch] = ns;
        }
        else
        {
            printf("invalid rpc channel type: %s\n", k2.c_str());
            return false;
        }
    }

    if (default_spec)
    {
        for (auto& kv : *default_spec)
        {
            if (nss.find(kv.first) == nss.end())
            {
                nss[kv.first] = kv.second;
            }
        }
    }

    return true;
}


static bool build_server_network_confs(
    const char* section,
    /*out*/ network_server_configs& nss,
    network_server_configs* default_spec,
    const std::vector<int>& ports,
    bool is_template)
{
    nss.clear();

    const char* keys[128];
    int kcapacity = 128;
    int kcount = dsn_config_get_all_keys(section, keys, &kcapacity);
    dassert(kcount <= 128, "");

    for (int i = 0; i < kcapacity; i++)
    {
        std::string k = keys[i];
        if (k.length() <= strlen("network.server."))
            continue;

        if (k.substr(0, strlen("network.server.")) != std::string("network.server."))
            continue;

        auto k2 = k.substr(strlen("network.server."));
        std::list<std::string> ks;
        utils::split_args(k2.c_str(), ks, '.');
        if (ks.size() != 2)
        {
            printf("invalid network server config '%s', should be like 'network.server.12345.RPC_CHANNEL_TCP' instead\n", k.c_str());
            return false;
        }

        int port = atoi(ks.begin()->c_str());
        auto k3 = *ks.rbegin();

        if (is_template)
        {
            if (port != 0)
            {
                printf("invalid network server configuration '%s'\n", k.c_str());
                printf("port must be zero in [apps..default]\n");
                printf(" e.g., network.server.0.RPC_CHANNEL_TCP = NET_HDR_DSN, dsn::tools::asio_network_provider,65536\n");
                return false;
            }
        }
        else
        {
            if (std::find(ports.begin(), ports.end(), port) == ports.end())
            {
                continue;
            }
        }

        if (rpc_channel::is_exist(k3.c_str()))
        {
            /*            
            port = 0 for default setting in [apps..default]
            port.channel = header_format, network_provider_name,buffer_block_size
            network.server.port().RPC_CHANNEL_TCP = NET_HDR_DSN, dsn::tools::asio_network_provider,65536
            network.server.port().RPC_CHANNEL_UDP = NET_HDR_DSN, dsn::tools::asio_network_provider,65536
            */

            rpc_channel ch = rpc_channel::from_string(k3.c_str(), RPC_CHANNEL_TCP);
            network_server_config ns(port, ch);

            // NET_HDR_DSN, dsn::tools::asio_network_provider,65536
            std::list<std::string> vs;
            std::string v = dsn_config_get_value_string(section, k.c_str(), "", 
                "network channel configuration, e.g., NET_HDR_DSN, dsn::tools::asio_network_provider,65536");
            utils::split_args(v.c_str(), vs, ',');

            if (vs.size() != 3)
            {
                printf("invalid network specification '%s', should be '$message-format, $network-factory,$msg-buffer-size'\n",
                    v.c_str()
                    );
                return false;
            }

            if (!network_header_format::is_exist(vs.begin()->c_str()))
            {
                printf("invalid network specification, unkown message header format '%s'\n",
                    vs.begin()->c_str()
                    );
                return false;
            }

            ns.hdr_format = network_header_format(vs.begin()->c_str());
            ns.factory_name = *(++vs.begin());
            ns.message_buffer_block_size = atoi(vs.rbegin()->c_str());

            if (ns.message_buffer_block_size == 0)
            {
                printf("invalid message buffer size specified: '%s'\n", vs.rbegin()->c_str());
                return false;
            }

            nss[ns] = ns;
        }
        else
        {
            printf("invalid rpc channel type: %s\n", k3.c_str());
            return false;
        }
    }

    if (default_spec)
    {
        for (auto& kv : *default_spec)
        {
            network_server_config cs = kv.second;
            for (auto& port : ports)
            {
                cs.port = port;
                if (nss.find(cs) == nss.end())
                {
                    nss[cs] = cs;
                }
            }

            if (is_template)
            {
                cs.port = 0;
                if (nss.find(cs) == nss.end())
                {
                    nss[cs] = cs;
                }
            }
        }
    }

    return true;
}

service_app_spec::service_app_spec(const service_app_spec& r)
{
    index = r.index;
    id = r.id;
    config_section = r.config_section;
    role_name = r.role_name;
    name = r.name;
    role = r.role;
    type = r.type;
    arguments = r.arguments;
    ports = r.ports;
    pools = r.pools;
    delay_seconds = r.delay_seconds;
    ports_gap = r.ports_gap;
    run = r.run;
    dmodule = r.dmodule;
    dmodule_bridge_arguments = r.dmodule_bridge_arguments;
    network_client_confs = r.network_client_confs;
    network_server_confs = r.network_server_confs;
    count = r.count;
    data_dir = r.data_dir;
}

bool service_app_spec::init(
    const char* section, 
    const char* role_name_,
    service_app_spec* default_value,
    network_client_configs* default_client_nets,
    network_server_configs* default_server_nets
    )
{
    id = 0;
    index = 0;
    role_name = std::string(role_name_);
    name = role_name;
    config_section = std::string(section);

    if (!read_config(section, *this, default_value))
        return false;

    std::sort(ports.begin(), ports.end());
    ports_gap = ports.size() > 0 ? (*ports.rbegin() + 1 - *ports.begin()) : 0;

    if (!build_client_network_confs(
        section,
        this->network_client_confs,
        default_value ? &default_value->network_client_confs : default_client_nets
        ))
        return false;

    if (!build_server_network_confs(
        section,
        this->network_server_confs,
        default_value ? &default_value->network_server_confs : default_server_nets,
        ports,
        default_value == nullptr
        ))
        return false;

    return true;
}

network_server_config::network_server_config(const network_server_config& r)
: channel(r.channel), hdr_format(r.hdr_format)
{
    port = r.port;
    factory_name = r.factory_name;
    message_buffer_block_size = r.message_buffer_block_size;
}

network_server_config::network_server_config(int p, rpc_channel c)
    : channel(c), hdr_format(NET_HDR_DSN)
{
    port = p;
    factory_name = "dsn::tools::asio_network_provider";
    message_buffer_block_size = 65536;
}

bool network_server_config::operator < (const network_server_config& r) const
{
    return port < r.port || (port == r.port && channel < r.channel);
}

bool service_spec::init()
{
    // init common spec
    if (!read_config("core", *this, nullptr))
        return false;

    // init thread pools
    if (!threadpool_spec::init(threadpool_specs))
        return false;

    // init task specs
    if (!task_spec::init())
        return false;
    
    return true;
}

extern "C"
{
    typedef dsn_error_t (*dsn_app_bridge_t)(int, const char**);
}

void service_spec::load_app_shared_libraries(dsn::configuration_ptr config)
{
    std::vector<std::string> all_section_names;
    config->get_all_sections(all_section_names);

    int app_id = 0;
    std::vector< std::pair<std::string, std::string> > modules;
    for (auto it = all_section_names.begin(); it != all_section_names.end(); ++it)
    {
        if (it->substr(0, strlen("apps.")) == std::string("apps."))
        {
            std::string module = config->get_string_value(it->c_str(), "dmodule", "",
                "path of a dynamic library which implement this app role, and register itself upon loaded");
            if (module.length() > 0)
            {
                std::string bridge_args = config->get_string_value(it->c_str(), "dmodule_bridge_arguments", "",
                    "\n; when the service cannot automatically register its app types into rdsn \n"
                    "; through %dmoudule%'s dllmain or attribute(constructor), we require the %dmodule% \n"
                    "; implement an exporte function called \"dsn_error_t dsn_bridge(const char* args);\", \n"
                    "; which loads the real target (e.g., a python/Java/php module), that registers their \n"
                    "; app types and factories."
                    );

                modules.push_back(std::make_pair(module, bridge_args));
            }
        }
    }

    for (auto m : modules)
    {
        auto hmod = ::dsn::utils::load_dynamic_library(m.first.c_str());
        if (nullptr == hmod)
        {
            dassert(false, "cannot load shared library %s specified in config file",
                m.first.c_str());
            break;
        }

        // have dmodule_bridge_arguments?
        if (m.second.length() > 0)
        {
            dsn_app_bridge_t bridge_ptr = (dsn_app_bridge_t)::dsn::utils::load_symbol(hmod, "dsn_app_bridge");
            dassert(bridge_ptr != nullptr,
                "when dmodule_bridge_arguments is present (%s), function dsn_app_bridge must be implemented in module %s",
                m.second.c_str(),
                m.first.c_str()
                );

            ddebug("call %s.dsn_app_bridge(...%s...)",
                m.first.c_str(),
                m.second.c_str()
                );

            std::vector<std::string> args;
            std::vector<const char*> args_ptr;
            utils::split_args(m.second.c_str(), args);

            for (auto& arg: args)
            {
                args_ptr.push_back(arg.c_str());
            }

            bridge_ptr((int)args_ptr.size(), &args_ptr[0]);
        }
    }
}

#define mimic_app_role_name "dsn.app.mimic"

static void* mimic_app_create(const char*)
{
    return nullptr;
}

static dsn_error_t mimic_app_start(void* ctx, int argc, char** argv)
{
    return ::dsn::ERR_OK;
}

static void mimic_app_destroy(void* ctx, bool clean_up)
{
    return;
}

bool service_spec::init_app_specs()
{
    // register mimic app
    dsn_register_app_role(
        mimic_app_role_name,
        mimic_app_create,
        mimic_app_start,
        mimic_app_destroy
        );

    // init service apps
    service_app_spec default_app;
    if (!default_app.init("apps..default", ".default", nullptr,
        &this->network_default_client_cfs,
        &this->network_default_server_cfs
        ))
        return false;

    std::vector<std::string> all_section_names;
    config->get_all_sections(all_section_names);
    
    // check mimic app
    if (enable_default_app_mimic)
    {
        std::string mimic_section_name("apps.mimic");
        if (std::find(all_section_names.begin(), all_section_names.end(), mimic_section_name)
            == all_section_names.end())
        {
            config->set("apps.mimic", "type", mimic_app_role_name, "must be " mimic_app_role_name);
            config->set("apps.mimic", "pools", "THREAD_POOL_DEFAULT", "");
            all_section_names.push_back("apps.mimic");
        }
        else
        {
            auto type = config->get_string_value("apps.mimic", "type", "", "app type, must be " mimic_app_role_name);
            if (strcmp(type, mimic_app_role_name) != 0)
            {
                printf("invalid config value '%s' for [apps.mimic] type", type);
                return false;
            }
        }
    }

    // init all apps
    int app_id = 0;
    for (auto it = all_section_names.begin(); it != all_section_names.end(); ++it)
    {
        if (it->substr(0, strlen("apps.")) == std::string("apps.") && *it != std::string("apps..default"))
        {
            service_app_spec app;
            if (!app.init((*it).c_str(), it->substr(5).c_str(), &default_app))
                return false;

            // fix ports_gap when necessary
            int ports_gap = app.ports_gap;
            switch (rpc_io_mode)
            {
            case IOE_PER_NODE:
                ports_gap *= 1;
                break;
            case IOE_PER_QUEUE:
                {
                    int number_of_ioes = 0;
                    for (auto& pl : app.pools)
                    {
                        number_of_ioes += (this->threadpool_specs[pl].partitioned 
                            ? this->threadpool_specs[pl].worker_count : 1);
                    }
                    ports_gap *= number_of_ioes;
                }
                break;
            default:
                dassert(false, "unsupport io mode");
                break;
            }

            auto& store = ::dsn::utils::singleton_store<std::string, ::dsn::service_app_role>::instance();
            ::dsn::service_app_role role;
            if (!store.get(app.type, role))
            {
                printf("service type '%s' not registered\n", app.type.c_str());
                return false;
            }

            app.role = role;

            auto ports = app.ports;   
            auto nsc = app.network_server_confs;
            for (int i = 1; i <= app.count; i++)
            {
                char buf[16];
                sprintf(buf, "%u", i);
                app.name = (app.count > 1 ? (app.role_name + buf) : app.role_name);
                app.id = ++app_id;
                app.index = i;
                app.data_dir = utils::filesystem::path_combine(data_dir, app.name);

                // add app
                app_specs.push_back(app);
                dassert((int)app_specs.size() == app.id, "incorrect app id");

                // for next instance
                app.ports.clear();
                for (auto& p : ports)
                {
                    app.ports.push_back(p + i * ports_gap);
                }

                app.network_server_confs.clear();
                for (auto sc : nsc)
                {
                    sc.second.port += i * ports_gap;
                    app.network_server_confs[sc.second] = sc.second;
                }
            }
        }
    }

    return true;
}

int service_spec::get_ports_delta(int app_id, dsn_threadpool_code_t pool, int queue_index) const
{
    dassert(rpc_io_mode == IOE_PER_QUEUE, "only used for IOE_PER_QUEUE mode");

    auto& aps = app_specs[app_id - 1];
    int number_of_ioes = 0;
    for (auto& pl : aps.pools)
    {
        if (pl != pool)
        {
            number_of_ioes += (this->threadpool_specs[pl].partitioned
                ? this->threadpool_specs[pl].worker_count : 1);
        }
        else
        {
            number_of_ioes += (this->threadpool_specs[pl].partitioned
                ? (queue_index + 1) : 1);
            break;
        }
    }

    dassert(number_of_ioes >= 1, "given pool not started");

    return aps.ports_gap * (number_of_ioes - 1);
}


} // end namespace dsn
