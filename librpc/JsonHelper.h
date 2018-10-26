/**
 * @CopyRight:
 * FISCO-BCOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FISCO-BCOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
 * (c) 2016-2018 fisco-dev contributors.
 *
 * @brief: unit test for Session
 *
 * @file JsonHelper.h
 * @author: caryliao
 * @date 2018-10-26
 */
#pragma once

#include <libethcore/Common.h>
#include <libp2p/Common.h>
#include <json/json.h>

namespace dev
{
namespace rpc
{
Json::Value toJson(
		dev::eth::Transaction const& _t, std::pair<h256, unsigned> _location, dev::eth::BlockNumber _blockNumber);
dev::eth::TransactionSkeleton toTransactionSkeleton(Json::Value const& _json);

}  // namespace rpc

}  // namespace dev
