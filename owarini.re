#@# ここに本文を書いてください ("= おわりに" のマークアップは、いりません)

2010 年のある朝のことです。上司に呼び出され、一言「明日から OpenFlow のプロジェクトを手伝ってくれないか？」OpenFlow についてよいイメージを持っていなかった私ですが、上司命令には逆らえず、やむなく OpenFlow の仕事についたのでした。もともと私は IP ネットワークの経路制御が専門で、自律分散的なアプローチが至上であると信じていました。そのため、OpenFlow の集中型のアプローチには抵抗があったのです。

しかし、実際に仕事を始めてみると、いろいろと OpenFlow の利点・欠点がわかってきました。本書にも書きましたが、他のシステムと連携しやすい点(@<chap>{sliceable_switch}) や、トラフィックエンジニアリングがやりやすい点 (@<chap>{google}) などは、集中型ゆえの特長と言えるでしょう。

OpenFlow に関していろいろ誤解が多いのは、これらの利点が正しく伝わっていないと、感じるようになりました。OpenFlow を「なんでもできる」と一言で説明がされることがありますが、この言い方が誤解を冗長させているのかもしれません。表面的な説明に終始するだけでは、正しくその魅力が伝わりません。

この本は、このような状況を変えたいという思いから生まれたものです。読者のみなさまが OpenFlow の魅力を知る手助けになれば幸いです。

最後に、本書の執筆に協力してくれたすべての方に感謝を申し上げます。いろいろな視点からのコメントをいただいた Trema 塾のメンバーには深く感謝します。本書に深みを感じていただけたならば、それは彼らのコメントが活きている証拠です。また、職場のメンバーにも感謝します。本書で紹介した多くは、彼らが生み出したものです。本書に対して有益なコメントも多数頂きました。また OpenFlow 黎明期からの仕様策定や、世界初の商用スイッチ、コントローラの製品化に関わってきた NEC のメンバーにも感謝します。彼らが道を切り拓かなければ、今の OpenFlow の盛り上がりはなかったかもしれません。Trema 開発チームも、彼らから多くのことを学びました。執筆の機会と様々なサポートを頂いた技術評論社の取口様にも感謝します。最後に、執筆の応援をしてくれた家族に感謝します。

//flushright{
2012年11月30日@<br>{}鈴木 一哉 (@<tt>{@countone})
//}