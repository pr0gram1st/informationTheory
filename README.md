<h1> Encoding using Huffman coding algorithm </h1>
<p>To encode some text file into binary sequence, e.g. "akzhol1337" -> "11111010100110011110100110110000".</p>
<p>We need to assign every unique character <strong>binary code</strong></p>
<p>for example: </p>
<code>
  <pre>
    a: 111
    j: 110
    b: 10
    c: 0
  </pre>
</code>
<p> This is a binary codes of characters for file containing "abcabcabababcccccj" </p>
<p> Our task is to make resulting binary sequence (addition of binary codes of all file characters) small as possible </p>
<p> Also, code of some character should not be prefix of another, (you'll see why later)</p>

<p> To do it so, we need to calculate frequency of every character in our file </p>
<p> for example: </p>
<code>
  <pre>
    j: 1 times
    b: 5 times
    a: 5 times
    c: 7 times
  </pre>
</code>

<p>Now, let's create <strong>Huffman Tree</strong></p>
