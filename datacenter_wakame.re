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

仮想ネットワークとはひとことで言うと、スライス間でのトラフィックの分離でした(@<chap>{sliceable_switch})。つまり、同じスライスにつながるホスト同士は通信を許可し、異なるスライスにつながるホスト同士での通信は遮断することで、実ネットワークをたくさんのユーザで安全に共有します。

このスライスを実現するには、ホストがつながる実ネットワークと、仮想ネットワークを構成するスライスとの間でのパケットの制御が必要です。これを行うのが、OpenFlowスイッチです(@<img>{packet_transformation})。ホストから実ネットワークを通じてOpenFlowスイッチに届いたパケットは、ホストの属するスライス向けにOpenFlowスイッチが書き換えて転送します。反対に、スライスからホストへ向かうパケットは、逆の処理を行うことで実ネットワーク上の目的のホストへと届けます。

//image[packet_transformation][OpenFlow スイッチは実ネットワークと仮想ネットワーク間のパケットを書き換えて転送][width=12cm]

Wakame-VDCと@<chap>{sliceable_switch}で紹介したスライス機能つきスイッチとの大きな違いは、ネットワーク仮想化のためのOpenFlowスイッチの使いどころです(@<img>{edge_network_virtualization})。スライス機能つきスイッチでは、パケットの制御をネットワーク上のOpenFlowスイッチで行います。一方Wakame-VDCでは、この制御を各ホスト上に起動したソフトウェアOpenFlowスイッチでホストごとに行います。このように、これら2つの一番の違いは"どこでOpenFlowスイッチを動かすか"という点です。

//image[edge_network_virtualization][スライス機能つきスイッチとWakame-VDCのネットワーク仮想化方式の違い][width=12cm]

これらの2方式には次のトレードオフがあります。Wakame-VDCは各ホスト上のソフトウェアOpenFlowスイッチで実⇔仮想ネットワーク間の制御を行うので、OpenFlowに未対応のスイッチで構成されたネットワークに持ってきてもそのまま動く、という利点があります。一方で、スライス機能スイッチはこの制御をネットワーク内のOpenFlowスイッチで行うので、ネットワークをOpenFlowスイッチで構成する必要があります。しかし、スライス機能つきスイッチにも性能面での利点があります。OpenFlowスイッチをすべて実機にすることで、ハードウェアのスピードでパケットを転送できるのです。一方でWakame-VDCはソフトウェア版のOpenFlowスイッチを使い、しかもホストをユーザのVMと共有するので、性能が落ちやすいという問題があります。これらのトレードオフはもちろん、どんなIaaSを作りたいかや予算などによって優先度が変わります。

=== トラフィック分離実現への課題

OpenFlow に対応していない一般的なスイッチは、MAC アドレスを識別して、パケットの転送が行われます。一般的なスイッチを使う場合でも、きちんと届け先の VM の MAC アドレスを、宛先フィールドに入れてあげれば、そのパケットを宛先まで届けてくれます。

#@# ブロードキャストと区別するためにユニキャストであることが分かる表現とする必要がある（ただしユニキャストという用語は使わない）。そのため「届け先の MAC アドレスを宛先にいれる」と表現している。by kazuya

複数ユーザを収容する IaaS では、異なるユーザが同じ IP アドレスを持っている場合がありますが、それでも問題はありません (@<img>{unicast})。Ethernet スイッチはパケット転送の際に、MAC アドレスを用いますが、IP アドレスは参照しないためです。ただし、各ホストの MAC アドレスはネットワーク全体でユニークでなければいけません。

//image[unicast][一般的な Ethernet スイッチでのトラフィック分離]

しかし、一般的な Ethernet のみでトラフィック分離をしようとする場合、@<chap>{router_part1} で説明した ARP パケットの取り扱いが問題になります。ARP リクエストを出すVMは、どのVMから応答がもらえるか知りません。そのため、ネットワーク全体に届くよう、ARP リクエストの宛先にはブロードキャストアドレス (FF:FF:FF:FF:FF:FF) を指定します。例えば @<img>{arp_entangle} のVM A が送った ARP リクエストは、全VMに届きます。このとき、同じ IP アドレスを持つVM B とVM D の両方が ARP リプライを返すので、最終的に正しく通信ができません。このため、ARP リクエストをどのように扱うかが、課題です。

//image[arp_entangle][ARP リプライが混乱する]

Wakame-VDC では、エッジに配置した OpenFlow スイッチの制御で、この課題を解決しています。どのように解決しているか詳しく見ていきましょう。

=== エッジで ARP を制御する

#@# まずは同じ OVS につながっている場合の説明。

Wakame-VDC では、OpenFlow スイッチを使い、同じユーザが使っているVM間でのみ ARP パケットが届くように制御します。他のユーザが使っている VM に ARP が届かないように、その VM と接続するポートで ARP パケットを破棄します(@<img>{arp_block})。

//image[arp_block][同じユーザのVMにだけ ARP リクエストを届ける]

@<img>{translate} のように、ホストをまたいで ARP リクエストを届ける場合には、宛先を書き換えて、物理スイッチを通過させます。VM A が出す ARP リクエストの宛先はブロードキャストアドレスなので、関係ないホストまでパケットが届いてしまいます。そのため、ホスト内にあるソフトウェアの OpenFlow スイッチで ARP パケットの宛先をホスト B に書き換えます。宛先を書き換えた ARP パケットは、物理スイッチを通過して、ホスト B に届きます。ホスト B 内のソフトウェア OpenFlow スイッチが書き換えたパケットを元に戻してから VM C へと ARP パケットを届けます。

//image[translate][ARP リクエストは、宛先を書き換えて、別のホストへと届けられる]

このように Wakame-VDC では、エッジの OpenFlow スイッチの制御だけで、仮想ネットワークを実現しています。物理スイッチは、宛先 MAC アドレスを見て転送するという、一般的なスイッチの処理を行なっているだけです。そのため、この部分を新たに OpenFlow スイッチに置き換える必要はなく、既存のネットワークをそのまま使えます。また、 ARP リクエストは最終的に宛先を書き戻して VM に届くので、VM は何も気にすることなく ARP に応答することができます。非常にシンプルな仕組みで、既存インフラ上に仮想ネットワークを実現しているのが Wakame-VDC の特徴です。

ここでは詳しく説明しませんが、Wakame-VDC では偽装パケットへの対策も行なっています。シンプルな仕組みで仮想ネットワークを実現しているため、宛先や送信元を偽装することで、別のユーザの VM へとパケットを送ることができてしまいます。このことを避けるために、エッジの OpenFlow スイッチでパケットを受信した際に、宛先や送信元のチェックを行い、偽装したパケットを破棄するようにしています。

=== Wakame-VDC 方式の利点

仮想マシンが生成する仮想ネットワーク用のパケットを、物理ネットワーク向けに変換する方法は、色々あります。

最も有名な方法は、GREに代表される、トンネリング技術を使う方法です。実際は、仮想ネットワーク用のパケットそのものは書き換えず、物理ネットワークを通れる別のパケットを用意し、そのペイロードに包む方法です。このような手法をEncapsulationと言います。

Wakame-VDC では、GREトンネルを使う方法も実験的に組み込まれていますが、より手軽で、高速な手法として、ARPパケットの制御を行う方法を紹介します。この仕組みは、ARPパケットのブロードキャストを制御し、目的となる仮想マシンのNICにだけ、的確にARPを届けることができれば、他の仮想マシンから見えなくなると言う原則に基づいています。

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
