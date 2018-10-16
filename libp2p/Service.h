/*
    This file is part of FISCO-BCOS.

    FISCO-BCOS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FISCO-BCOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file Service.h
 *  @author molan
 *  @modify first draft
 *  @date 20180910
 *  @author chaychen
 *  @modify realize encode and decode, add timeout, code format
 *  @date 20180911
 */

#pragma once
#include "Host.h"
#include "P2PInterface.h"
#include "P2PMsgHandler.h"
#include <libdevcore/Common.h>
#include <libdevcore/Exceptions.h>
#include <libdevcore/FixedHash.h>
#include <memory>

namespace dev
{
namespace p2p
{
class Service : public P2PInterface, public std::enable_shared_from_this<Service>
{
public:
    Service(std::shared_ptr<Host> _host, std::shared_ptr<P2PMsgHandler> _p2pMsgHandler)
      : m_host(_host), m_p2pMsgHandler(_p2pMsgHandler)
    {
        m_ioService = _host->ioService();
        ///< Set m_p2pMsgHandler to host
        ///< When a new session created, host set handler to the new session.
        _host->setP2PMsgHandler(m_p2pMsgHandler);
    }

    virtual ~Service() { m_ioService = NULL; }

    Message::Ptr sendMessageByNodeID(NodeID const& nodeID, Message::Ptr message) override;

    void asyncSendMessageByNodeID(NodeID const& nodeID, Message::Ptr message,
        CallbackFunc callback = [](P2PException e, Message::Ptr msg) {},
        Options const& options = Options()) override;

    Message::Ptr sendMessageByTopic(std::string const& topic, Message::Ptr message) override;

    void asyncSendMessageByTopic(std::string const& topic, Message::Ptr message,
        CallbackFunc callback, Options const& options) override;

    void asyncMulticastMessageByTopic(std::string const& topic, Message::Ptr message) override;

    void asyncMulticastMessageByNodeIDList(NodeIDs const& nodeIDs, Message::Ptr message) override;

    void asyncBroadcastMessage(Message::Ptr message, Options const& options) override;

    void registerHandlerByProtoclID(int16_t protocolID, CallbackFuncWithSession handler) override;

    void registerHandlerByTopic(std::string const& topic, CallbackFuncWithSession handler) override;

    void setTopicsByNode(
        NodeID const& _nodeID, std::shared_ptr<std::vector<std::string>> _topics) override;

    std::shared_ptr<std::vector<std::string>> getTopicsByNode(NodeID const& _nodeID) override;

    ///< Only connected node
    SessionInfos sessionInfos() const override;

    SessionInfos sessionInfosByProtocolID(int16_t _protocolID) const override;

    bool isConnected(NodeID const& _nodeID) const override { return m_host->isConnected(_nodeID); }

private:
    void onTimeoutByTopic(const boost::system::error_code& error,
        std::shared_ptr<SessionFace> oriSession, NodeIDs& nodeIDsToSend, Message::Ptr message,
        CallbackFunc callback, Options const& options, uint32_t totalTimeout);
    void onTimeoutByNode(
        const boost::system::error_code& error, uint32_t seq, std::shared_ptr<SessionFace> p);

    NodeIDs getPeersByTopic(std::string const& topic);

    bool isSessionInNodeIDList(NodeID const& targetNodeID, NodeIDs const& nodeIDs);

    std::shared_ptr<Host> m_host;
    boost::asio::io_service* m_ioService;

    std::shared_ptr<P2PMsgHandler> m_p2pMsgHandler;
    std::atomic<uint32_t> m_seq = {0};  ///< The message identify is generated by the service by
                                        ///< autoincrement.
};

}  // namespace p2p

}  // namespace dev