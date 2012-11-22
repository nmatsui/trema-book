= OpenFlow でデータセンターをつなげる

//lead{
データセンターを1台の巨大なコンピュータと見立てると、この巨大コンピュータ同士をつなぐネットワークが必要になります。たくさんのデータセンターを持つGoogleは、OpenFlow を使って低コストでこのネットワークを作り上げてしまいました。
//}

== Googleは OpenFlow を導入済み!

「Googleはデータセンター間のトラフィックの 100% を、すでに SDN でまかなっている」。2012 年 4 月、SDN や OpenFlow をテーマにした一大カンファレンスである Open Networking Summit において、Googleの上級副社長兼フェローであるウルス・ヘルツル氏が衝撃の発表を行いました。Googleはすでに OpenFlow を実戦投入しており、データセンター間のネットワークを OpenFlow で構築したというのです。インターネット界の巨人Googleは、OpenFlow をどのように活用しているのか？ヘルツル氏の講演内容をわかりやすく解説します。

Googleはインターネット上の情報を大量に収集するために、最新鋭のデータセンターをいくつも構築しました。たとえばGoogleが公開している資料からは、以下のすさまじいデータを発見できます。

 * データセンターの電力効率を示す PUE (1.0 に近いほど効率的) は 1.13
 * 代表的なサービスである Gmail の年間アベイラビリティは 99.984 %
 * データセンターで一日に処理するデータ量は数十ペタバイト以上

//noindent
Googleはこのように電力効率や安定性、そして処理能力を両立したデータセンターを低いコストで実現しています。

データセンターは効率化できたが、ほかに改善できる部分は？Googleはデータセンター同士をつなぐネットワーク (広域通信網、WAN) に目をつけました。OpenFlow を使えば、WAN のコストを下げられそうだ。Googleの次の狙いはネットワーク、とくに WAN の改善だったのです。

=== WAN のコストを下げるには？

世界各地にデータセンターを持つGoogleでは、WAN 回線のコスト削減が急務です。Googleはデータセンター同士をつなぐために、専用の WAN 回線をたくさん引いているからです。次に示すように、WAN 回線は容量や距離が増えるごとにコストがかさみます。

 * WAN 回線は容量が大きいほど高価になる。たとえば、あるキャリアで 500 km を接続するサービスの価格を比べると、 10 Gbps は 1Gbps のおよそ 1.5 倍になり、コストを下げるためには、なるべく容量の小さく安い回線を最大限まで使う必要がある
 * WAN 回線は距離が長いほど高価になる。Googleが他社と共同で引いた日米間約 1 万km、回線容量は 4.8 Tbps の海底ケーブルは、総建設費が約 320 億円にもなった。コストを下げるためには、あまり長いケーブルは引けない

WAN 回線に追加投資をしなくとも、効率的にデータを転送するにはどうすればいいでしょうか？従来のインターネットのルーティングでは、宛先に対して必ず最短となるパスを選んで転送を行います。どれくらい帯域を使うかに関わらず最短経路を選択してしまうため、トラフィックが集中する回線や、逆にまったく使われない回線が出てきます。これは大きな無駄で、確かにコスト削減の余地があります。

=== Googleの WAN 構成

続く詳しい分析のために、ここでGoogleの WAN 構成を見ておきましょう。その役割に応じて、Googleの WAN は次の二つのネットワークに分かれています。

 1. I-Scale：インターネットに直接つながるネットワーク(Googleを利用するユーザのトラフィックを運ぶ)
 2. G-Scale：データセンター間をつなぐバックボーンネットワーク(Google内部のトラフィックを運ぶ)

Googleは、このうち G-Scale に OpenFlow を適用しました。次の G-Scale の特徴により、効率化の余地が大きいと判断したためです。

 * インターネットにつながらない閉じたネットワークなので、どんなトラフィックが流れるかを予測しやすい
 * Google専用のネットワークなので、トラフィックごとの優先度を自由に決定できる

//noindent
これらの特徴を踏まえると、WAN 回線を効率的に使うための課題がはっきりします。

== WAN 回線を効率的に使うには？

@<img>{wan} のデータセンター A からデータセンター B へ、どれだけトラフィックを流せるかを考えてみましょう。普通のルーティングでは、最短経路となるリンク 1 のみを使うため、このリンクの帯域が上限となります。もしこの上限を超えるトラフィックを運びたい場合、通常はリンク帯域の増設が必要になり、コストが増えてしまいます。

//image[wan][データセンター A からデータセンター B へどのくらいトラフィックを流せる？][scale=0.6]

しかし、スイッチ C を経由する経路を足すことで、さらにトラフィックを流せるようになります (@<img>{multipath})。

//image[multipath][複数の経路を使ってトラフィックを転送する][scale=0.6]

このように複数の経路を使うと、どれだけトラフィックを流せるでしょうか？もしもデータセンター A と B ですべてのリンクを占有できるなら、それぞれのリンク帯域の上限まで流せます。たとえば、リンク 1 にはリンク帯域の 10 Gbps までトラフィックを流せます。スイッチ C を通る経路のように複数のリンクを使う場合、流せるトラフィック量は帯域が最小のリンクによって決まります。つまり、スイッチ C を通る経路は 6Gbps までのトラフィックを流せます。結局、@<img>{multipath} のネットワークでは、データセンター A からデータセンター B へ、最大で 16Gbps のトラフィックを流せることになります。

しかし、実際に複数の経路を使ってトラフィックを流すには、次の 3 つの課題をクリアしなければなりません。

=== 課題 1：空いた帯域を調べるには？

実際に流せるトラフィック量は、各リンクの利用状況によります。@<img>{multipath} では、データセンター A からデータセンター B へは 16Gbps までトラフィックを流せると説明しました。しかし、普通は他のデータセンター間にもトラフィックがあるので、使えるのは 16Gbps のうち空いている帯域だけです。そこで実際にどの程度トラフィックを流せるかを知るためには、各リンクの空き帯域の情報が必要です。

しかし、離れた場所にあるリンクの空き帯域はどうやれば分かるでしょうか？データセンター A は、直接つながるスイッチ A がやり取りしているトラフィック量を調べることで、リンク 1 とリンク 2 の空き帯域が分かります。しかし、データセンター A から離れたリンク 3 の空き帯域を知るには、別の方法が必要です。もちろん、実際のネットワークにはもっとたくさんのリンクがあるので、すべてのリンクの空き帯域情報が必要です。

=== 課題 2：空いた帯域をどう分け合うか？

空いた帯域はデータセンター間で仲良く分け合わなければなりません。たとえば @<img>{conflict} のネットワークにおいて、リンク 1 の空き帯域が 10Gbps であるとします。もしデータセンター X とデータセンター Y がそれぞれが勝手に、データセンター Z へ 10Gbps のトラフィックを流そうとすると、このうち半分のトラフィックだけがリンク 1 を通過できます。あふれたトラフィックは、スイッチが破棄してしまいます。

これを避けるためにはデータセンター間での調停者が必要です。つまり、空いた帯域を指定した比率で分け合いないさいとか、特定のトラフィックだけがこの空き帯域を使ってよい、などと上から決定してやる仕組みが必要です。

//image[conflict][どのトラフィックが空き帯域を使うかを、うまく調停する必要がある][scale=0.7]

=== 課題 3：複数の経路にどう振り分ける？

1つのデータセンターから出るトラフィックを複数の経路へ振り分けたい場合もあります。たとえば、@<img>{traffic} のデータセンターから、リンク 1 経由の経路で 10Gbps、リンク 2 経由の経路で 6Gbps の帯域がそれぞれ利用できるとします。OpenFlow スイッチは、これらの帯域を超えないように各サービスのトラフィックを振り分ける必要があります。

このためには、サービスごとのトラフィック量を知っている必要があります。しかし、OpenFlow スイッチが認識できるのはヘッダの情報だけで、サービスごとのトラフィック情報までは分かりません。そこで、何らかの方法でこれを教えてもらう必要があります。

//image[traffic][サービスごとのトラフィックを、それぞれどの経路に流すかを決定する必要がある][scale=0.6]

== G-Scale での WAN 回線の効率的な利用法

それではGoogleのデータセンター間ネットワークである G-Scale が、これら 3 つの課題をどのように解決したかを見ていきましょう。

=== トラフィックエンジニアリングサーバの導入

G-Scale 全体の空き帯域を一括して管理するのが、トラフィックエンジニアリングサーバ (TE サーバ) です (@<img>{te_server})。TE サーバは、次のようにして各リンクの空き帯域の情報を集め、空き帯域を各データセンター間のトラフィックに割り当てます。

//image[te_server][TE サーバは空き帯域を把握し、各データセンターのトラフィックへ割り当てる][scale=0.6]

 * リンクの空き帯域は、リンクの帯域とそのリンクに流れているトラフィック量を調べれば、これらの差分から計算できる。一般的なスイッチは、これらの情報を常に収集しているので、TE サーバはすべてのスイッチに問い合わせて空き帯域を計算する
 * 空き帯域の割り当ての詳細については、詳しいアルゴリズムが公表されていないのでわからない。しかし、ビジネスポリシーや各データセンターの稼働状況などなど、さまざまなパラメータによって決めていることが予想できる

//noindent
このように空き帯域情報の収集と割り当てを「神様」である TE サーバが一括して行うようにすることで、前節に上げた課題 1, 2 を同時に解決しています。

=== スイッチの構成

G-Scale を構成するスイッチ 1 つひとつは、実は BGP 処理部と OpenFlow コントローラ、そして OpenFlow スイッチから成ります。この 3 つを一組として @<img>{wan_equip} のようにデータセンターの BGP ルータと接続し、それぞれが次の動作をします。

//image[wan_equip][G-Scale を構成するスイッチは、BGP 処理部、OpenFlow コントローラ、そして OpenFlow スイッチから成る][scale=0.6]

: BGP 処理部
  トラフィックがどのデータセンター宛なのかを決定するのが、BGP 処理部です。G-Scale がトラフィックを宛先のデータセンターに届けるためには、データセンター内にあるホストの IP アドレスを知る必要があります。そのために、BGP 処理部はデータセンターから IP アドレスに関する情報 (経路情報と呼ぶ) を受け取り、他の BGP 処理部と交換します。このようにすべての経路情報を収集することで、パケット中の宛先アドレスからどのデータセンターに送るべきかを判断します。

: OpenFlow コントローラ
  空き帯域の割り当てを受け、トラフィックを流す経路を決定するのが OpenFlow コントローラです。OpenFlow コントローラは、TE サーバに対して、宛先のデータセンターまでの空き帯域の割り当てを要求します。そして、割り当てられた空き帯域にトラフィックが流れるように、OpenFlow スイッチにフローエントリを設定します。

: OpenFlow スイッチ
  OpenFlow コントローラからの指示に従い、実際にトラフィックを転送するのが OpenFlow スイッチです。大量のトラフィックを扱う必要があるため、10Gbps Ethernet のインターフェイスを 100 ポート以上持つ大規模な OpenFlow スイッチを複数台使います。この規模のスイッチはまだ市場には存在しないため、Googleは 1 からこのスイッチを開発したそうです。自前での開発にはそれなりのコストが掛かりますが、WAN の効率化により削減できるコストの方が大きいとGoogleは判断したのでしょう。

なお、OpenFlow コントローラが TE サーバから空き帯域を割り当ててもらうためには、サービスごとのトラフィック量を知る必要があります。これが課題の 3 ですが、残念ながらその方法は未公表です。実際にトラフィックが流れれば、スイッチの統計情報からその使用帯域はわかるものの、空き帯域の割り当てはトラフィックが流れる前に行う必要があります。そのため、G-Scale では、トラフィックの発生元のアプリケーションから教えてもらうか、過去の統計情報から推定するか、どちらかを採用していると推測できます。

====[column] BGP はどのように動作する？

G-Scale は経路情報の収集のために内部で BGP を用いています。BGP はもともと、インターネットを構成する異なる組織の間で経路情報を交換するためのプロコトルです。G-Scale では、各データセンターの BGP ルータと G-Scale 内の BGP 処理部が BGP を使って経路情報を交換します。BGP 処理部は、各データセンターから経路情報を受け取り、その情報を元にしてパケットを届けるデータセンターを決定します。

//image[bgp][BGP を用いてデータセンター A 内のアドレスを BGP 処理部に通知する][scale=0.6]

たとえば、データセンター A 内のホストが 192.168.1.0/24 のアドレスを持つとします (@<img>{bgp})。データセンター A 内の BGP ルータはこのアドレスに対する経路情報を、BGP を使って BGP 処理部に通知します。経路情報を受け取った BGP 処理部は、やはり BGP を使って他の BGP 処理部へと経路情報を転送します。これを繰り返すことで、宛先が 192.168.1.1 であるパケットはデータセンター A へと送り届ければよい、ということを G-Scale 全体で共有します。

====[/column]

== まとめ

Googleがネットワークを有効に活用し全体コストを下げるために、とくにデータセンター間トラフィックの転送に OpenFlow をどのように活用しているかを紹介しました。

 * 無駄なリンク帯域の増設によるコストを避けるために、既存のリンクを使って帯域をかせぐ方法:GoogleはWAN全体の空き帯域の監視と割り当てを行うTEサーバを導入することで、複数経路を使った転送を効率化している
 * BGPとOpenFlowを使って経路を決定する方法:データセンター内のホストまでの経路情報の交換にはBGP、また経路の切り替えにはOpenFlowスイッチとコントローラを使うことで、データセンター間の経路を決定している

Googleのような大規模事業者が実際に OpenFlow を動かしているという事実は、これから OpenFlow に取り組もうとしている技術者にとって大きな励みとなるでしょう。Googleのような大規模環境での運用は難しいですが、彼らのチャレンジからは多くのことが学べます。みなさんの課題解決に、この章の内容が少しでも役立てばと思います。

== 参考文献

: Open Networking Summit 2012 ウルス・ヘルツル氏の講演 (@<href>{http://youtu.be/VLHJUfgxEO4})
  今回取り上げたウルス・ヘルツル氏の講演内容はすべて Youtube で見られます。Googleがどのようなチャレンジを行ったか、さらに詳細を知りたい人にとっては必見の動画です。
: Google を支える技術 (西田圭介著／技術評論社)
  この章ではネットワークのコスト削減について取り上げましたが、この本ではデータセンターの運用コストなどの分析を、Googleが発表した論文をもとに行っています。また、Googleのデータセンターを支えるソフトウェア技術についても学べます。
: インターネットルーティングアーキテクチャ (サム・ハラビ他著／ソフトバンククリエイティブ)
  より深く BGP について学びたい人は、この本を読むとよいでしょう。具体的な例が多く紹介されており、BGP の動作をイメージしながら理解できます。
