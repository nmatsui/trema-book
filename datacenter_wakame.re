= データセンターを Trema で作る

//lead{
執筆中です。
//}

== Trema と OSS でデータセンターを作ろう

一台のコンピュータ上で、リソースの管理や基本サービスの提供に OS が必要になるように、データセンターにも同じ機能を提供するソフトウェア階層が必要です。IaaS はその一つで、ユーザやアプリケーションに対して仮想的なリソースの貸し借りを行います。たとえば「192.168.0.1/24 のネットワークでつながった高性能サーバ 16 台がほしい」というリクエストを受けると、IaaS はスペックの高い VM を物理マシン上に起動し、専用の仮想ネットワークを作って VM 同士をつなぎます。このように、データセンターのリソースを仮想化して使いやすく提供するのが IaaS です。

IaaS のネットワーク仮想化には OpenFlow が適しています。(理由をここに)。IaaS では、必要なとき仮想ネットワークを作り、使い終われば削除します。また IaaS では多くのユーザを収容しているため、仮想ネットワーク作成・削除は、他のユーザに影響を与えないように行う必要があります。

== Wakame-VDC

Tremaを利用した本格的なIaaSプラットフォームが、Wakame-VDCです。Tremaと同じくgithub上でオープンに開発しており、ライセンスはLGPLv3のフリーソフトウェアです。開発は22企業・団体が所属する任意団体、Wakame Software Foundation (WSF)が行っています。

Wakame-VDCの情報は次のURLから入手できます。

 * Wakame-VDC開発リポジトリ: @<href>{https://github.com/axsh/wakame-vdc}
 * Wikiドキュメント: @<href>{http://wakame.jp/wiki/}
 * Wakame Software Foundation: @<href>{http://wakame.jp/document/175/}

Wakame-VDCの特長は、九州電力など多くの企業のプライベートクラウド構築基盤として実績がある、商用ソフトウェアであるということです。基本的なIaaSの機能である、ネットワーク、ホストおよびストレージの仮想化と管理機能はもちろん、Ruby on Railsを使った便利なWebインタフェースも提供します(@<img>{wakame_screenshot})。

//image[wakame_screenshot][Wakame-VDCのWebインタフェース][width=13cm]

Wakame-VDCはアーキテクチャ的にもユニークな点があります。普通のTremaアプリケーションでは、ひとつのコントローラプロセスがたくさんのOpenFlowスイッチを制御するという集中制御になっていました。Wakame-VDCでは、Tremaで書いたコントローラをIaaSを提供する物理ホストに分散配置し、これらをひとつのマネージャプロセスが制御するというアーキテクチャになっています。Wakame-VDCも集中制御であることには変わりませんが、その下では分散したTremaがネットワークを仮想化します。これを集中制御によるネットワーク仮想化と対比して、エッジによるネットワーク仮想化と呼びます。

== エッジによるネットワーク仮想化

仮想ネットワークとはひとことで言うと、スライス間でのトラフィックの分離でした(@<chap>{sliceable_switch})。つまり、同じスライスにつながるホスト同士は通信を許可し、異なるスライスにつながるホスト同士での通信は遮断することで、実ネットワークをたくさんのユーザで安全に共有します。

このスライスを実現するには、ホストがつながる実ネットワークと、仮想ネットワークを構成するスライスとの間でのパケットの相互変換が必要です。この変換を実際に行うのが、OpenFlowスイッチです(@<img>{packet_transformation})。ホストから実ネットワークを通じてOpenFlowスイッチに届いたパケットは、ホストの属するスライス用にOpenFlowスイッチが書き換えます。反対に、スライスからホストへ向かうパケットは、逆の変換をすることで無事に実ネットワーク上の目的のホストへと届けます。

//image[packet_transformation][OpenFlow スイッチは実ネットワークと仮想ネットワークのパケットを相互に変換][width=12cm]

Wakame-VDCと@<chap>{sliceable_switch}で紹介したスライス機能つきスイッチとの大きな違いは、ネットワーク仮想化のためのOpenFlowスイッチの使いどころです(@<img>{edge_network_virtualization})。スライス機能つきスイッチでは、パケットの変換をネットワーク上のOpenFlowスイッチで行います。一方Wakame-VDCでは、この変換を各ホスト上に起動したソフトウェアOpenFlowスイッチでホストごとに行います。このように、これら2つの一番の違いは"どこでOpenFlowスイッチを動かすか？"ということです。

//image[edge_network_virtualization][スライス機能つきスイッチとWakame-VDCのネットワーク仮想化方式の違い][width=12cm]

これらの2方式には次のトレードオフがあります。Wakame-VDCは各ホスト上のソフトウェアOpenFlowスイッチで実⇔仮想ネットワーク間の変換を行うので、OpenFlowに未対応のスイッチで構成されたネットワークに持ってきてもそのまま動く、という利点があります。一方で、スライス機能スイッチはこの変換をネットワーク内のOpenFlowスイッチで行うので、ネットワークをOpenFlowスイッチで構成する必要があります。しかし、スライス機能つきスイッチにも性能面での利点があります。OpenFlowスイッチをすべて実機にすることで、ハードウェアのスピードでパケットを転送できるのです。一方でWakame-VDCはソフトウェア版のOpenFlowスイッチを使い、しかもホストをユーザのVMと共有するので、性能が落ちやすいという問題があります。これらのトレードオフはもちろん、どんなIaaSを作りたいかや予算などによって優先度が変わります。

=== トラフィック分離実現への課題

一般的な Ethernet スイッチから構成されるネットワークでは、MAC アドレスを識別して、パケットの転送が行われます。そのため、きちんと届け先の VM の MAC アドレスを、宛先フィールドに入れてあげれば、ネットワークはそのパケットを宛先まで届けてくれます。

#@# ブロードキャストと区別するためにユニキャストであることが分かる表現とする必要がある（ただしユニキャストという用語は使わない）。そのため「届け先の MAC アドレスを宛先にいれる」と表現している。by kazuya

複数ユーザを収容する IaaS では、異なるユーザが同じ IP アドレスを持っている場合がありますが、それでも問題はありません (@<img>{unicast})。Ethernet スイッチはパケット転送の際に、MAC アドレスを用いますが、IP アドレスは参照しないためです。ただし、各ホストの MAC アドレスはネットワーク全体でユニークでなければいけません。

//image[unicast][一般的な Ethernet スイッチでのトラフィック分離]

しかし、一般的な Ethernet のみでトラフィック分離をしようとする場合、@<chap>{router_part1} で説明した ARP パケットの取り扱いが問題になります。ARP リクエストを出すホストは、どのホストから応答がもらえるか知りません。そのため、ネットワーク全体に届くよう、ARP リクエストの宛先にはブロードキャストアドレス (FF:FF:FF:FF:FF:FF) を指定します。例えば @<img>{arp_entangle} のホスト A が送った ARP リクエストは、全ホストに届きます。このとき、同じ IP アドレスを持つホスト B とホスト D の両方が ARP リプライを返すので、最終的に正しく通信ができません。このため、ARP リクエストをどのように扱うかが、課題です。

//image[arp_entangle][ARP リプライが混乱する]

Wakame-VDC では、エッジに配置した OpenFlow スイッチの制御で、この課題を解決しています。どのように解決しているか詳しく見ていきましょう。

=== エッジで ARP を制御する

#@# まずは同じ OVS につながっている場合の説明。

Wakame-VDC では、OpenFlow スイッチを使い、同じユーザが使っているホスト間でのみ ARP パケットが届くように制御を行います。他のユーザが使っているホストに ARP が届かないように、接続するポートでそのパケットを破棄します。

//image[arp_block][同じユーザのホストにだけ ARP リクエストを届ける]

//image[translate][ARP リクエストは、宛先を書き換えて、別のホストへと届けられる]

Wakame-VDCは、エッジスイッチにOpen vSwitch (OVS)を利用しています。一つの例として、ARPがどのように届くのかを順を追って説明していきます。

（紙芝居）

このように、２台の仮想マシンの間にあるOVS間では、ARPのブロードキャストは物理ネットワークでは使われません。ARPは、OVSに指定されたルールによって物理ネットワークをユニキャストで通過するように変換されます。受け取った側のOVSは、元のARPに戻します。それから、ARPを目的の仮想マシンのNICだけに、あたかもブロードキャストされたかのように届けるのです。

これで、送った側の仮想マシンにも、受け取った側の仮想マシンにも、今までのARPと同じ挙動をしたように見せることができます。他の仮想マシンには一切分かりません。もし、他の仮想マシンが、ARPを偽装したとしても、SRCとDSTが一致しないものはDROPしてしまいます。

=== パケットの変換方法

仮想マシンが生成する仮想ネットワーク用のパケットを、物理ネットワーク向けに変換する方法は、色々あります。

最も有名な方法は、GREに代表される、トンネリング技術を使う方法です。実際は、仮想ネットワーク用のパケットそのものは書き換えず、物理ネットワークを通れる別のパケットを用意し、そのペイロードに包む方法です。このような手法をEncapsulationと言います。

Wakame-VDCでは、GREトンネルを使う方法も実験的に組み込まれていますが、より手軽で、高速な手法として、ARPパケットの制御を行う方法を紹介します。この仕組みは、ARPパケットのブロードキャストを制御し、目的となる仮想マシンのNICにだけ、的確にARPを届けることができれば、他の仮想マシンから見えなくなると言う原則に基づいています。

== Wakame-VDCの構成概要

ここまでは、仮想ネットワークについて注目した説明でした。Wakame-VDCは、全体の論理構成を図hogehogeのように、まとめています。

(全体の図)

Web UIから指示を受けて、Data Center Manager (dcmgr)と呼ばれるデータセンター全体のリソース管理をする中枢部分に司令が飛ぶと、実際に仮想マシンの準備や、仮想ネットワークの設定などが行われます。

必要な作業指示は、メッセージキュープロトコルであるAMQPのネットワークに流され、物理リソースの管理をしているエージェントに届き、処理されます。エージェントは、物理サーバに分散してインストールされており、各々が指示を受け取るようになっています。

特に、仮想マシンを収容するサーバで動作するエージェントは、Hyper Visor Agent (hva)と呼ばれており、これが仮想マシンを起動し、仮想ネットワークをセットアップします。

Wakame-VDCは、各物理サーバに常駐するhvaが、Tremaフレームワークを通じて、hvaと対になるようにローカルに配置されているOVSと連携します。つまり、Wakame-VDCは、分散するOpenFlowコントローラへ指示を出して、各コントローラが担当するOVSの設定をOpenFlowプロトコルで書き換えていると表現することができます。この辺りも、Tremaを活用した例としても特徴的な部分ではないでしょうか。

== なぜTremaを使い始めたのか

もともとWakame-VDCは、エッジスイッチとしてLinux BridgeのNetfilterを利用していました。物理サーバのホストOSに備わっているパケット制御の機構を用いて、ネットワークの操作をしていたのです。すでに、2010年11月には、いくつかの制約はあるものの、物理ネットワークの上に、仮想ネットワークを自由に組めるようになっていました。アーキテクチャも当時と変わっていません。

しかし、ホストOSに組み込まれた機能を使っているため、通信する度に、物理サーバのCPUを使ってしまいます。物理サーバには、仮想マシンも収容されていますので、仮想マシンのためにもCPUが使いたいのですが、今後はネットワークにもCPUを食われるようになりそうな懸念があったのです。未来のCPUの機能改善や、性能向上に期待しつつも、我々に出来ることは何かを模索していました。ネットワークの処理をオフロードする必要があると考えたのです。

その時に出会ったのが、OpenFlowでした。スイッチを制御する標準的なプロトコルとして、採用されれば、今ホストOS上で行なっている処理を、外部に出しても、OpenFlowを使って指示をすることはできます。オフロードを目論む私達にはぴったりの技術でした。あとは、OpenFlowコントローラとして、作りこみが必要だったので、フレームワークを探していたところ、Tremaが期待の星のように目の前に現れたのです。Wakame-VDCは、全てRubyで記述されていたのも幸いでした。早速Tremaのソースコードを読み、まずはフレームワークとしてのデザインが気に入りました。それからどう使っていくか、方針を決め、取り掛かったのが、2011年の秋頃でした。その後、Tremaの機能改善のため、パッチを送るなどして、何とかWakame-VDCへの組み込みが終わり、今は応用範囲を、より完全な仮想ネットワークへと進展させています。

== まとめ

Wakame-VDCがTremaフレームワークを使って得られた他の効果もあります。それは、ソースコードのパッチを送って以来、Trema開発者の方々と仲良くなれた事です。たまに一緒に飲みに行きますが、集まればOpenFlowの未来や、ギークな激論で盛り上がるかと思いきや、誰も一切そんな話なんてしません。ただの楽しい飲んだくれでございます。

飲み会はともかく、ソースコードのパッチを送る事はとても大切です。取り分け、オープンソースライセンスのコードは、自分で改善を施す事ができて初めて本来の意味を持ちます。ダウンロードして来て、使えるか、使えないかを判断するだけではいけません。もしあなたが、本当に技術者であり、開発者なのであれば、使えないと放棄する前に、使えるようにするためのコードを生み出さねばなりません。そうする事で世の中は一歩ずつ前に進んでいきます。さあ、パッチを書きましょう！あなたの力が世界を変えるのです。
