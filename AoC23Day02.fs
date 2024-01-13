open System.IO

let ParsePull (x:string) =
    Array.fold(fun (r, g, b) (s:string) -> let info = s.Split(' ')
                                           let cubes = System.Int32.Parse info[0]
                                           match info[1][0] with 
                                             | 'r' -> (cubes, g, b)
                                             | 'g' -> (r, cubes, b)
                                             |  _  -> (r, g, cubes)) (0,0,0) (x.Split(", "))

let ParseGame (x:string) =
    let gamePull = x.Split(": ")
    let gameNum = System.Int32.Parse(Array.last(gamePull[0].Split(' ')))
    let pulls = gamePull[1].Split("; ") |> Array.map ParsePull 
    (gameNum, Array.fold(fun (a, b, c) (d, e, f) -> (max a d, max b e, max c f)) (0,0,0) pulls)

[<EntryPoint>]
let main argv =
    let games = File.ReadLines(argv[0]) |> Seq.map ParseGame
    printfn("Part 1: %A") <| Seq.fold(fun t (i, (a, b, c)) -> if a <= 12 && b <= 13 && c <=14 then t + i else t) 0 games
    printfn("Part 2: %A") <| Seq.fold(fun t (_, (a, b, c)) -> t + (a*b*c)) 0 games
    0
