= データセンター基盤を Trema で作る

//lead{
"Hello, Trema!"から始めたTremaも、いよいよデータセンターまで進みました。Tremaとフリーソフトウェアの組み合わせで、商用品質のIaaS(Infrastructure as a Service)を簡単に作れてしまいます。
//}

== フリーソフトウェアで IaaS を作ろう

1台のコンピュータ上で、リソースの管理や基本サービスの提供にOSが必要になるように、データセンターにも同じ機能を提供するソフトウェア基盤が必要です。IaaSはその1つで、ユーザやアプリケーションに対しAPIを通じて、仮想的なリソースを提供します。たとえば「192.168.0.1/24のネットワークでつながったサーバが16台ほしい」というリクエストを受けると、IaaSはVMを16台ほど物理マシン上に起動し、専用の仮想ネットワークを作ってこのVM同士をつなぎます。このように、データセンターのリソースを仮想化し、APIで使いやすく提供するソフトウェア基盤がIaaSです。

従来、IaaSは自分で作るものではなく、AmazonやGoogleなど大規模クラウド事業者が提供するサービスを使うのが一般的でした。オンデマンドで仮想ネットワークを作ったり、他のユーザに影響を与えずに構成変更したりする仕組みは各社の独自ノウハウで、その技術的な詳細は未公開だったからです。このため、すでにあるサーバリソースを活かして専用のIaaSを作りたいという、いわゆるプライベートクラウドの構築はハードルが高いものでした。

しかし、OpenFlowによってネットワークのAPIが整備されたことで、いわゆるオープンソースのIaaSが増えつつあります。たとえば、オープンソースなIaaSの代名詞ともなったOpenStackは、ネットワークの仮想化にOpenFlowを使う仕組みを提供しており、@<chap>{sliceable_switch}で紹介したスライス機能つきスイッチと連携できます@<fn>{quantum}。今回紹介するWakame-VDCは、Tremaを使ったオープンソースなIaaSの一種で、OpenStackと比較して非常に軽量な、今注目のIaaSプラットフォームです。

//footnote[quantum][OpenStackのTremaプラグイン: @<href>{https://github.com/nec-openstack/quantum-openflow-plugin}]

== Wakame-VDC

Tremaを利用した本格的なIaaSプラットフォームが、Wakame-VDCです。Tremaと同じくGitHub上でオープンに開発しており、ライセンスはLGPLv3のフリーソフトウェアです。開発は22企業・団体が所属する任意団体、Wakame Software Foundation(WSF)が行っています。

Wakame-VDCの情報は次のURLから入手できます。

 * Wakame-VDC開発リポジトリ: @<href>{https://github.com/axsh/wakame-vdc}
 * Wikiドキュメント: @<href>{http://wakame.jp/wiki/}
 * Wakame Software Foundation: @<href>{http://wakame.jp/document/175/}

Wakame-VDCの特長は、九州電力など多くの企業のプライベートクラウド構築基盤として実績がある、商用ソフトウェアであるということです。基本的なIaaSの機能である、ネットワーク、ホストおよびストレージの仮想化と管理機能はもちろん、Ruby on Railsを使った便利なWebインターフェースも提供します(@<img>{wakame_screenshot})。

//image[wakame_screenshot][Wakame-VDCのWebインターフェース][width=13cm]

もう1つの特長は、非常に小さいコード規模でIaaSを実現していることです。重量級のIaaSプラットフォームであるOpenStackは、今や60万行ものソースコードを抱え、その規模は日増しに拡大しています。一方でWakame-VDCはTremaのRubyライブラリによる記述性を活かし、Webインターフェースやテストコードも含めてたった4万行のRubyコードですべての機能を実現しています。このため、Wakame-VDCはソースコード全体の把握や改造が簡単です。

== エッジによるネットワーク仮想化

ネットワークの仮想化とは、ユーザ間のトラフィックを分離することです(@<chap>{sliceable_switch})。トラフィック分離により、同じユーザが使うVM同士は通信を許可し、異なるユーザが使うVM間の通信を遮断します。Wakame-VDCは、これらの制御にOpenFlowを使います。

@<chap>{sliceable_switch}で紹介したスライス機能付きスイッチとの大きな違いは、Wakame-VDCは、既存のネットワークで動作するように設計されている点です。スライス機能付きスイッチでは、パケットの制御をネットワーク上のOpenFlowスイッチで行います。このため、ネットワーク中のスイッチがすべてOpenFlowに対応している必要がありました。一方 Wakame-VDC では、この制御を各ホスト上に起動したソフトウェアOpenFlowスイッチ(エッジスイッチ)だけで行います(@<img>{edge_network_virtualization})。こうすることで、既に構築されているネットワークの上で仮想ネットワークを実現できます。

//image[edge_network_virtualization][スライス機能付きスイッチとWakame-VDCのネットワーク仮想化方式の違い][width=12cm]

=== 普通のスイッチでのトラフィック分離とその課題

Wakame-VDCはどのようにして、OpenFlowに対応していない普通のスイッチを使い、トラフィックを分離しているのでしょうか？

実は、パケットが単一の宛先を指定したものである場合、つまり、いわゆるユニキャストである場合は、普通のスイッチでもユーザごとのトラフィックを分離できます(@<img>{unicast})。普通のスイッチは、パケットの宛先MACアドレスを識別してパケットを転送します。IaaS で使う場合でも、宛先となるVMのMACアドレスを宛先フィールドに入れれば、きちんと宛先まで届きます。複数のユーザを収容するIaaSでは、異なるユーザが同じIPアドレスを持っている場合がありますが、それでも問題はありません。スイッチが転送に使うのはMACアドレスであって、IPアドレスは参照しないからです。ただし、普通のスイッチがトラフィック分離できるのは、あくまで宛先の MAC アドレスがわかっているユニキャストの通信だけです。

問題になるのは、ブロードキャストパケットの扱いです。ブロードキャストパケット中の宛先MACアドレスには、どのユーザの VM も同じアドレス (FF:FF:FF:FF:FF:FF) を使います。そのため、宛先MACアドレスを見るだけの普通のスイッチでは、ブロードキャストパケットをユーザごとに区別することができません。

//image[unicast][普通のスイッチを使っても、ユニキャストトラフィックを分離できる][width=12cm]

ブロードキャストをうまく扱えないことで困るのは、ARP リクエストです。宛先のMACアドレスを問い合わせる ARP リクエストは、どのVMから応答がもらえるかわからないため、宛先にはブロードキャストアドレスを使います。そのため、たとえば@<img>{arp_entangle}のVM Aが送ったARPリクエストは、ユーザに関係なく全てのVMに届いてしまいます。このとき、同じIPアドレスを持つVM BとDの両方がARPリプライを返すので、VM Aはどちらが正しいMACアドレスかわからず混乱します。

//image[arp_entangle][普通のスイッチをそのまま IaaS に使うと、ARPリプライが混乱する][width=12cm]

Wakame-VDCは、エッジスイッチの制御で、この課題を解決しています。

=== エッジスイッチでARPを制御する

Wakame-VDCでは、エッジスイッチを使い、同じユーザのVM間でのみARPリクエストが届くように制御します(@<img>{arp_block})。他のユーザが使っているVMにARPリクエストが届かないように、そのVMと接続するポートでARPリクエストを破棄します。

//image[arp_block][エッジスイッチで同じユーザのVMにだけARPリクエストを届ける][width=12cm]

ホストをまたいだARPリクエストは@<img>{translate}の様に処理します。エッジスイッチは、ホストA中のVM Aが出したARPリクエストをVM Cに届けるため、その宛先をホストBに書き換えます。宛先を書き換えられたARPパケットは、物理スイッチを通過して、ホストBに届きます。ホストB内のエッジスイッチは、届いたARPパケットの宛先を元に戻し、VM Aと同じユーザXのVM Cへと送ります。

//image[translate][エッジスイッチでARPリクエストの宛先を書き換えて、別のホストへと届ける][width=12cm]

このようにWakame-VDCでは、エッジスイッチの制御だけで仮想ネットワークを実現しています。ホスト間を結ぶスイッチは、宛先MACアドレスを見て転送するという一般的な処理をすればよいだけなので、OpenFlowスイッチである必要はありません。また、ARPリクエストは最終的に宛先を書き戻してVMに届くので、VMは何も気にすることなくARPに応答できます。このように、既存のネットワークやVMに影響を与えることなく仮想ネットワークを実現できるのがWakame-VDCの特徴です。

== 全体アーキテクチャ

ここまでは、Wakame-VDCの仮想ネットワーク機能について説明しましたが、Wakame-VDCはそれ以外にもストレージの仮想化やWebインターフェースといったIaaSプラットフォームに必要な機能を一通り備えています(@<img>{wakame_overview})。

//image[wakame_overview][Wakame-VDCの全体アーキテクチャ][width=12cm]

データセンタ全体のリソース管理は、"神様"であるData Center Manager (DCMGR)が一括して行います。Web インターフェースから指示を受けると、実際に仮想マシンの準備や、仮想ネットワークの設定などの作業指示を各エージェントへと送ります。この作業指示の送信には、メッセージキュープロトコルである AMQP を用いています。

VMやストレージなどのリソースは、それぞれ専用のエージェントが管理します。各エージェントは、AMQP を用いて作業指示を受け取り、実際のリソースに指示の内容を反映します。このうち Hyper Visor Agent (HVA) と呼ばれるエージェントが、仮想マシンの起動と仮想ネットワークのセットアップを担当します。

各物理ホストに常駐するHVAは、同一ホスト上に配置されているエッジスイッチをOpenFlowで制御します。つまり、HVAはOpenFlowコントローラに相当し、ここがTremaを使っている部分です。

Wakame-VDCのアーキテクチャがユニークな点は、各物理ホスト上に分散配置したHVAそれぞれが、担当するエッジスイッチを制御する点です。普通のTremaアプリケーションでは、1つのコントローラでたくさんのOpenFlowスイッチを集中制御します。Wakame-VDCも集中制御であることには変わりませんが、DCMGRの指示の下で動作するHVAを分散配置し、エッジスイッチの制御をそれぞれのHVAに任せている点がユニークです。

== なぜTremaを選んだのか

Wakame-VDCはこのようなTremaを使った実装にたどりつくまでに、さまざまな試行錯誤をしてきたそうです。Wakame-VDCの開発の中心、株式会社あくしゅ代表の山崎泰宏氏にうかがいました。

//quote{
「もともとWakame-VDCは、エッジスイッチとしてLinux BridgeのNetfilterを利用していました。物理サーバのホストOSに備わっているパケット制御の機構を用いて、仮想ネットワークを実現していたのです。すでに2010年には、いくつかの制約はあるものの、物理ネットワークの上に仮想ネットワークを自由に組めるようになっていました。アーキテクチャも当時と変わっていません。

しかし、OSの機能を使ったネットワーク仮想化には性能的な問題がありました。通信が発生するごとに、物理サーバのCPUを少なからず使ってしまうのです。物理サーバには、お客様の仮想マシンも動いています。将来10Gbps以上の広帯域に移行した場合、ネットワークにこれ以上CPUを食われるのはまずいと考えました。

その時に出会ったのが、OpenFlowでした。OpenFlowスイッチをエッジスイッチとして使えば、ホストOS上で行なっているネットワーク処理を簡単に外部に出せます。移行の第一段階としてソフトウェアスイッチを使えば、Netfilterによる実装との変更もそれほど大がかかりではありません。こうすることによって段階的に、CPUでのネットワーク処理を物理OpenFlowスイッチにオフロードできると考えました。

OpenFlowでコントローラを書こうというときに出会ったのがTremaです。当時からWakame-VDCはRubyで書いていたので、Tremaとの相性はぴったりでした。早速Tremaのソースコードを読み、その簡潔なデザインやオープンなコミュニティがすぐに気に入りました。必要な機能をすぐに実装できたことと、われわれの書いたパッチがTremaにマージされたことで、とてもスムーズにWakame-VDCはOpenFlow化できたのです。」
//}

== まとめ

Tremaを使ったデータセンター基盤の例として、オープンソースかつ商用のIaaSプラットフォームである、Wakame-VDCの設計を見てきました。

 * Wakame-VDCは全体で約4万行のRubyコードという非常にコンパクトな実装になっており、全体の把握や改造が容易である
 * ネットワークの仮想化はエッジスイッチで行っており、これによって既存のOpenFlow化されていないネットワーク上で仮想ネットワークを構築できる
 * エッジスイッチでは、ARPリクエストのようなブロードキャストを制御することで、スライス間のトラフィックをきれいに分離する

さて次章は、ユースケース編の最終章です。データセンターを作る話からさらに進み、作ったデータセンターをOpenFlowでつなげるというGoogleの事例を紹介します。