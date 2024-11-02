/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:02:19 by ryanagit          #+#    #+#             */
/*   Updated: 2024/11/02 14:51:28 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ListenSocket.hpp"

// ListenSocket クラスのコンストラクタ


ListenSocket::ListenSocket(int fd, const SocketAddress &server_addr, const Config &config)
    : Socket(fd, server_addr, config) {}
// // 新しい接続要求を受け付けるメソッド
// Result<ClientSocket *> ListenSocket::AcceptNewConnection() {
//     // 接続要求を処理するロジックを実装する必要があります
//     return nullptr; // ダミー値
// }