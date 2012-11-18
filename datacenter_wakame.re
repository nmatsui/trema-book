= データセンターを Trema で作る

//lead{
"Hello, Trema!"から始めたOpenFlowプログラミングも、いよいよデータセンターを作るまでになりました。Tremaとフリーソフトウェアの組み合わせで、なんと商用品質のIaaSを簡単に作れてしまいます。
//}

== Trema とフリーソフトウェアでデータセンターを作ろう

一台のコンピュータ上で、リソースの管理や基本サービスの提供にOSが必要になるように、データセンターにも同じ機能を提供するソフトウェア階層が必要です。IaaSはその一つで、ユーザやアプリケーションに対して仮想的なリソースをAPIを通じて提供します。たとえば「192.168.0.1/24のネットワークでつながったサーバが16台ほしい」というリクエストを受けると、IaaSはVMを16台ほど物理マシン上に起動し、専用の仮想ネットワークを作ってこのVM同士をつなぎます。このように、データセンターのリソースを仮想化して使いやすく提供するのがIaaSです。

従来、IaaSはアマゾンやグーグルなど大規模クラウド事業者が提供するサービスを借りるのが一般的でした。オンデマンドで仮想ネットワークを作り、使い終われば削除する機能や、他のユーザに影響を与えずにこうした構成変更を行う仕組みは各社の独自ノウハウで、その技術的な詳細は未公開だったのです。このため、すでにあるサーバリソースを活かして専用のIaaSを作りたいという、いわゆるプライベートクラウドの構築はハードルが高いものでした。

しかし、OpenFlowによってネットワークのAPIが整備されたことで、いわゆるオープンソースのIaaSが増えつつあります。たとえば、オープンソースなIaaSの代名詞ともなったOpenStackは、ネットワークの仮想化にOpenFlowを使う仕組みを提供しており、@<chap>{sliceable_switch}で紹介したスライス機能つきスイッチと連携できます@<fn>{quantum}。今回紹介するWakame-VDCもTremaを使ったオープンソースなIaaSの一種で、OpenStackと比較して非常に軽量にIaaSを実現した、今注目のIaaSプラットフォームです。

//footnote[quantum][OpenStackのTremaプラグイン: @<href>{https://github.com/nec-openstack/quantum-openflow-plugin}]

== Wakame-VDC

Tremaを利用した本格的なIaaSプラットフォームが、Wakame-VDCです。Tremaと同じくgithub上でオープンに開発しており、ライセンスはLGPLv3のフリーソフトウェアです。開発は22企業・団体が所属する任意団体、Wakame Software Foundation (WSF)が行っています。

Wakame-VDCの情報は次のURLから入手できます。

 * Wakame-VDC開発リポジトリ: @<href>{https://github.com/axsh/wakame-vdc}
 * Wikiドキュメント: @<href>{http://wakame.jp/wiki/}
 * Wakame Software Foundation: @<href>{http://wakame.jp/document/175/}

Wakame-VDCの特長は、九州電力など多くの企業のプライベートクラウド構築基盤として実績がある、商用ソフトウェアであるということです。基本的なIaaSの機能である、ネットワーク、ホストおよびストレージの仮想化と管理機能はもちろん、Ruby on Railsを使った便利なWebインタフェースも提供します(@<img>{wakame_screenshot})。

//image[wakame_screenshot][Wakame-VDCのWebインタフェース][width=13cm]

もうひとつの特長は、非常に小さいコード規模でIaaSを実現していることです。重量級のIaaSプラットフォームであるOpenStackは、今や60万行ものソースコードを抱え、その規模は日増しに拡大しています。一方でWakame-VDCはTremaのRubyライブラリによる記述性を活かし、Webインタフェースやテストコードも含めてたった4万行ですべての機能を実現しています。このため、Wakame-VDCはソースコード全体の把握や改造が簡単です。

Wakame-VDCはアーキテクチャ的にもユニークです。普通のTremaアプリケーションでは、ひとつのコントローラプロセスがたくさんのOpenFlowスイッチを制御するという集中制御になっていました。Wakame-VDCでは、Tremaで書いたコントローラをIaaSを提供する物理ホストに分散配置し、これらをひとつのマネージャプロセスが制御するというアーキテクチャになっています。Wakame-VDCも集中制御であることには変わりませんが、その下では分散したTremaがネットワークを仮想化します。これを集中制御によるネットワーク仮想化と対比して、エッジによるネットワーク仮想化と呼びます。

== エッジによるネットワーク仮想化

Wakame-VDC は、仮想ネットワークの実現に OpenFlow を用いています。仮想ネットワークとはひとことで言うと、スライス間でのトラフィックの分離でした(@<chap>{sliceable_switch})。同じスライスにつながるホスト同士は通信を許可し、異なるスライスにつながるホスト同士での通信は遮断します。Wakame-VDC ではこれらの制御を OpenFlow を用いて実現します。

@<chap>{sliceable_switch} で紹介したスライス機能つきスイッチとの大きな違いは、Wakame-VDC は、一般的なネットワークで動作するように設計されている点です。スライス機能つきスイッチでは、パケットの制御をネットワーク上のOpenFlowスイッチで行います。この方式を使う場合には、ネットワーク中のスイッチがすべて OpenFlow に対応している必要があります。一方 Wakame-VDC では、この制御を各ホスト上に起動したソフトウェア OpenFlow スイッチだけで行います。このようにすることで、Wakame-VDC は、既に構築されているネットワークの上で仮想ネットワークを実現できます。

//image[edge_network_virtualization][スライス機能つきスイッチとWakame-VDCのネットワーク仮想化方式の違い][width=12cm]

=== 普通のスイッチでのネットワーク仮想化

Wakame-VDCはどのようにして、OpenFlowに対応していない普通のスイッチで仮想ネットワークを実現しているのでしょうか。OpenFlowに対応していない一般的なスイッチは、MACアドレスを識別してパケットを転送します。IaaSの仮想ネットワーク上でも、宛先となるVMのMACアドレスを宛先フィールドに入れれば、きちんと宛先まで届きます。複数のユーザを収容するIaaSでは、異なるユーザが同じIPアドレスを持っている場合がありますが、それでも問題はありません(@<img>{unicast})。スイッチが転送に使うのはMACアドレスであって、IPアドレスは参照しないからです。

#@# ブロードキャストと区別するためにユニキャストであることが分かる表現とする必要がある（ただしユニキャストという用語は使わない）。そのため「届け先の MAC アドレスを宛先にいれる」と表現している。by kazuya

//image[unicast][一般的なイーサネットスイッチでのパケット転送][width=12cm]

しかし、@<chap>{router_part1}で説明したARPパケットの取り扱いが問題になります。ARPリクエストを出すVMは、どのVMから応答をもらえるか知らないため、ネットワーク全体に届くよう、ARPリクエストの宛先にブロードキャストアドレス(FF:FF:FF:FF:FF:FF)を指定します。たとえば、@<img>{arp_entangle}のVM Aが送ったARPリクエストは、全てのVMに届いてしまいます。このとき、同じIPアドレスを持つVM BとDの両方がARPリプライを返すので、VM Aはどちらが正しいMACアドレスかわからず混乱します。このように、ARPリクエストをどのように扱うかが課題です。

//image[arp_entangle][一般的なイーサネットスイッチを仮想ネットワークに使うと、ARP リプライが混乱する][width=12cm]

Wakame-VDCは、エッジに配置したOpenFlowスイッチ (エッジスイッチ) の制御で、この課題を解決しています。

=== エッジスイッチで ARP を制御する

Wakame-VDC では、エッジスイッチを使い、同じユーザが使っているVM間でのみ ARP パケットが届くように制御します。他のユーザが使っている VM に ARP が届かないように、その VM と接続するポートで ARP パケットを破棄します(@<img>{arp_block})。

//image[arp_block][同じユーザのVMにだけ ARP リクエストを届ける]

@<img>{translate} のように、ホストをまたいだ ARP リクエストは宛先を書き換えて、普通のスイッチを通過させます。VM A が出す ARP リクエストの宛先はブロードキャストアドレスなので、関係ないホストまでパケットが届いてしまいます。そのため、エッジスイッチで ARP パケットの宛先をホスト B に書き換えます。宛先を書き換えた ARP パケットは、物理スイッチを通過して、ホスト B に届きます。ホスト B 内のエッジスイッチが書き換えたパケットを元に戻してから VM C へと ARP パケットを届けます。

//image[translate][ARP リクエストは、宛先を書き換えて、別のホストへと届けられる]

このように Wakame-VDC では、エッジスイッチの制御だけで、仮想ネットワークを実現しています。普通のスイッチは、宛先 MAC アドレスを見て転送するという、普通の処理を行なっているだけです。そのため、この部分を新たに OpenFlow スイッチに置き換える必要はなく、既存のネットワークをそのまま使えます。また、 ARP リクエストは最終的に宛先を書き戻して VM に届くので、VM は何も気にすることなく ARP に応答することができます。非常にシンプルな仕組みで、既存インフラ上に仮想ネットワークを実現しているのが Wakame-VDC の特徴です。

ここでは詳しく説明しませんが、Wakame-VDC では偽装パケットへの対策も行なっています。シンプルな仕組みで仮想ネットワークを実現しているため、宛先や送信元を偽装することで、別のユーザの VM へとパケットを送ることができてしまいます。このことを避けるために、エッジスイッチで、受信パケットの宛先や送信元をチェックし、偽装パケットを破棄します。

=== エッジによるネットワーク仮想化の利点と欠点

エッジで制御を行う Wakame-VDC はOpenFlowに未対応のスイッチで構成されたネットワークに持ってきてもそのまま動く、という利点があります。スライス機能スイッチを使う場合は、ネットワーク内の各スイッチをすべて OpenFlow 対応にする必要がありました。これに対して、既存のネットワークを変えずに導入できる点が、Wakame-VDC の大きな特徴です。

一方、導入の際には、性能面について考慮を行う必要があります。Wakame-VDC では、エッジスイッチは、ユーザの VM と同じホストマシン上で動作します。CPU リソースを共有しているため、大量のパケットを処理しなければいけない状況では、ユーザ VM に影響を与える可能性があります。しかし、最近では高性能な CPU を安価に使えるので、特殊な使い方をしなければ、大抵のケースでは問題にならないようです。

== Wakame-VDCの全体構成

ここまでは、Wakame-VDCの仮想ネットワークについて注目した説明でした。Wakame-VDCはネットワークの仮想化だけでなく、ストレージの仮想化やWebインタフェームを含むIaaSプラットフォームです。全体の構成は@<img>{wakame_overview}のようになります。

//image[wakame_overview][Wakame-VDCの全体アーキテクチャ]

Web UIから指示を受けて、Data Center Manager (dcmgr)と呼ばれるデータセンター全体のリソース管理をする中枢部分に司令が飛ぶと、実際に仮想マシンの準備や、仮想ネットワークの設定などが行われます。

必要な作業指示は、メッセージキュープロトコルであるAMQPのネットワークに流され、物理リソースの管理をしているエージェントに届き、処理されます。エージェントは、物理サーバに分散してインストールされており、各々が指示を受け取るようになっています。

特に、仮想マシンを収容するサーバで動作するエージェントは、Hyper Visor Agent (hva)と呼ばれており、これが仮想マシンを起動し、仮想ネットワークをセットアップします。

Wakame-VDCは、各物理サーバに常駐するhvaが、Tremaフレームワークを通じて、hvaと対になるようにローカルに配置されているOVSと連携します。つまり、Wakame-VDCは、分散するOpenFlowコントローラへ指示を出して、各コントローラが担当するOVSの設定をOpenFlowプロトコルで書き換えていると表現することができます。この辺りも、Tremaを活用した例としても特徴的な部分ではないでしょうか。

== なぜWakame-VDCはTremaを選んだか

Wakame-VDCはこのようなTremaを使った実装にたどりつくまでに、さまざまな試行錯誤をしてきたそうです。Wakame-VDCの開発の中心、株式会社あくしゅ代表の山崎泰宏氏にうかがいました。

//quote{
「もともとWakame-VDCは、エッジスイッチとしてLinux BridgeのNetfilterを利用していました。物理サーバのホストOSに備わっているパケット制御の機構を用いて、仮想ネットワークを実現していたのです。すでに2010年には、いくつかの制約はあるものの、物理ネットワークの上に仮想ネットワークを自由に組めるようになっていました。アーキテクチャも当時と変わっていません。

しかし、OSの機能を使ったネットワーク仮想化には性能的な問題がありました。通信が発生するごとに、物理サーバのCPUをかなり使ってしまうのです。物理サーバには、お客様の仮想マシンも動いています。将来10Gbps以上の広帯域に移行した場合、ネットワークにこれ以上CPUを食われるのはまずいと考えました。

その時に出会ったのが、OpenFlowでした。OpenFlowスイッチをエッジスイッチとして使えば、ホストOS上で行なっているネットワーク処理を簡単に外部に出すことができます。移行の第一段階としてソフトウェアスイッチを使えば、Netfilterによる実装との変更もそれほど大がかかりではありません。CPUでのネットワーク処理を、段階的に物理OpenFlowスイッチにオフロードできると考えました。

OpenFlowでコントローラを書こうというときに出会ったのがTremaです。当時からWakame-VDCはRubyで書いていたので、Tremaとの相性はぴったりでした。早速Tremaのソースコードを読み、その簡潔なデザインやオープンなコミュニティがすぐに気に入りました。必要な機能をすぐに実装できたことと、我々の書いたパッチがTremaにマージされたことで、とてもスムーズにWakame-VDCはOpenFlow化できたのです。」
//}

== まとめ

Tremaを使ったデータセンター基盤の例として、オープンソースかつ商用のIaaSプラットフォームである、Wakame-VDCの設計を見てきました。

 * Wakame-VDCは全体で約4万行という非常にコンパクトな実装になっており、全体の把握や改造が容易です。
 * ネットワークの仮想化はエッジスイッチで行っており、これによって既存のOpenFlow化されていないインフラ上で仮想ネットワークを構築できます。
 * エッジスイッチでは、ARPリクエストのようなブロードキャストを制御することで、スライス間のトラフィックをきれいに分離します。

さて次章は、ユースケース編の最終章です。データセンターを作る話からさらに進んで、作ったデータセンターをOpenFlowでつなげたというグーグルの事例を紹介します。